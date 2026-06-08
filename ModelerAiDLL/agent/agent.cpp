// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"
#include "agent/agent.h"
#include "agent/prompt.h"
#include "agent/session_store.h"
#include "agent/user_rules.h"
#include "bootstrap.h"
#include "bridge/log.h"
#include "kb/kb_loader.h"
#include "mainthread/mainthread.h"
#include "provider/provider.h"
#include "settings/effective_settings.h"
#include "third_party/json.h"
#include "tools/tool_base.h"
#include "tools/tool_registry.h"

#include <atomic>
#include <chrono>
#include <memory>
#include <optional>
#include <string>
#include <thread>

namespace ModelerAi::agent {

namespace {

std::string makeEnvelope(const char* type, const std::string& turnId, nlohmann::json payload)
{
    nlohmann::json env;
    env["t"]  = type;
    env["id"] = turnId;
    env["p"]  = std::move(payload);
    return env.dump();
}

nlohmann::json statsPayload(const SessionStats& s)
{
    nlohmann::json p;
    p["input_tokens"]                  = s.input_tokens;
    p["output_tokens"]                 = s.output_tokens;
    p["cache_read_input_tokens"]       = s.cache_read_input_tokens;
    p["cache_creation_input_tokens"]   = s.cache_creation_input_tokens;
    p["turns"]                         = s.turns;
    p["cost_usd"]                      = s.cost_usd;
    return p;
}

// Safety cap on the LLM ↔ tools ping-pong is now user-configurable —
// see EffectiveSettings::max_tool_iterations. Default 25; range 1-500.
// Snapshotted on the main thread in startTurn and passed into the
// worker's loop.

// Product is freeware — no prompt quota. The cost-ceiling guardrail in
// EffectiveSettings::cost_stop_usd is the user-controlled runaway-cost
// protection.

// How long the worker thread waits for the user to respond to a tool
// approval modal or ask_user_question prompt before giving up. Generous
// for ask_user_question (humans take time); the same for approval is fine.
constexpr auto kInteractionTimeout = std::chrono::minutes(10);

// Name of the bundled ask_user_question tool — special-cased in the
// dispatcher because its "run" doesn't fit the synchronous ITool contract
// (it blocks on user input via the InteractionBroker).
constexpr const char* kAskUserQuestionName = "ask_user_question";

// Forward declaration — definition further down. Both runAskUserQuestion
// and runWithApproval delegate to it for the normal sync path.
ToolCallRecord runOneToolCall(tools::ToolRegistry* registry,
                              const provider::ProviderToolCall& call);

ToolCallRecord runAskUserQuestion(const provider::ProviderToolCall& call,
                                  InteractionBroker* broker,
                                  const std::function<void(const std::string&)>& pushFn,
                                  const std::string& turnId)
{
    ToolCallRecord rec;
    rec.call_id   = call.call_id;
    rec.name      = call.name;
    rec.args_json = call.args_json.empty() ? std::string("{}") : call.args_json;

    if (!broker) {
        rec.ok            = false;
        rec.error_message = "Interaction broker not available.";
        rec.result_json   = R"({"error":"no_broker"})";
        return rec;
    }

    std::string question;
    nlohmann::json optionsJson = nlohmann::json::array();
    try {
        auto j = nlohmann::json::parse(rec.args_json);
        if (j.contains("question") && j["question"].is_string()) {
            question = j["question"].get<std::string>();
        }
        if (j.contains("options") && j["options"].is_array()) {
            optionsJson = j["options"];
        }
    } catch (...) {
        rec.ok            = false;
        rec.error_message = "Couldn't parse ask_user_question args.";
        rec.result_json   = R"({"error":"bad_args"})";
        return rec;
    }
    if (question.empty()) {
        rec.ok            = false;
        rec.error_message = "ask_user_question requires a non-empty 'question'.";
        rec.result_json   = R"({"error":"empty_question"})";
        return rec;
    }

    const std::string& interactionId = call.call_id;
    broker->registerPending(interactionId);

    nlohmann::json p;
    p["turn_id"]        = turnId;
    p["call_id"]        = call.call_id;
    p["interaction_id"] = interactionId;
    p["question"]       = question;
    p["options"]        = std::move(optionsJson);
    pushFn(makeEnvelope("ask_user_question_request", turnId, std::move(p)));

    bridge::consolePrint(("[ModelerAI] ask_user_question paused, waiting for user… id=" + interactionId + "\n"));
    auto resp = broker->waitFor(interactionId, kInteractionTimeout);
    bridge::consolePrint(("[ModelerAI] ask_user_question resolved id=" + interactionId
                          + " has_resp=" + std::string(resp.has_value() ? "yes" : "no") + "\n"));

    if (!resp.has_value()) {
        rec.ok            = false;
        rec.error_message = "User did not respond in time.";
        rec.result_json   = R"({"error":"timeout"})";
        return rec;
    }
    // Response shape: { answer: string, ... }. Pass through as the tool
    // result so the LLM can incorporate whatever the user said.
    rec.ok          = true;
    rec.result_json = resp->dump();
    return rec;
}

ToolCallRecord runWithApproval(const provider::ProviderToolCall& call,
                               tools::ITool* tool,
                               ToolPolicy policy,
                               InteractionBroker* broker,
                               const std::function<void(const std::string&)>& pushFn,
                               const std::string& turnId,
                               tools::ToolRegistry* registry,
                               Agent* agent)
{
    ToolCallRecord rec;
    rec.call_id   = call.call_id;
    rec.name      = call.name;
    rec.args_json = call.args_json.empty() ? std::string("{}") : call.args_json;

    auto syntheticDeny = [&](const char* reason, const std::string& message) {
        rec.ok            = false;
        rec.error_message = message;
        nlohmann::json err;
        err["error"]   = reason;
        err["message"] = message;
        rec.result_json = err.dump();
        return rec;
    };

    if (policy == ToolPolicy::AlwaysDeny) {
        return syntheticDeny("denied_by_policy",
            "User policy denies '" + std::string(tool->name()) + "'.");
    }
    if (policy == ToolPolicy::AutoAllow || tool->category() == tools::Category::ReadOnly) {
        // No prompt — run directly.
        return runOneToolCall(registry, call);
    }

    // Mutating tool, Ask policy → push approval request, block.
    if (!broker) return syntheticDeny("no_broker", "Interaction broker not available.");

    const std::string& interactionId = call.call_id;
    broker->registerPending(interactionId);

    auto categoryStr = [](tools::Category c) {
        switch (c) {
            case tools::Category::ReadOnly:        return "read_only";
            case tools::Category::Mutating:        return "mutating";
            case tools::Category::Filesystem:      return "filesystem";
            case tools::Category::Lifecycle:       return "lifecycle";
            case tools::Category::RuntimeReadOnly: return "runtime_read_only";
        }
        return "unknown";
    };

    nlohmann::json p;
    p["turn_id"]        = turnId;
    p["call_id"]        = call.call_id;
    p["interaction_id"] = interactionId;
    p["name"]           = std::string(tool->name());
    p["description"]    = std::string(tool->description());
    p["category"]       = categoryStr(tool->category());
    p["args"]           = rec.args_json;
    pushFn(makeEnvelope("tool_approval_request", turnId, std::move(p)));

    bridge::consolePrint(("[ModelerAI] tool_approval paused, waiting for user… name="
                          + std::string(tool->name()) + " id=" + interactionId + "\n"));
    auto resp = broker->waitFor(interactionId, kInteractionTimeout);
    bridge::consolePrint(("[ModelerAI] tool_approval resolved id=" + interactionId
                          + " has_resp=" + std::string(resp.has_value() ? "yes" : "no") + "\n"));

    if (!resp.has_value()) {
        return syntheticDeny("approval_timeout",
            "User did not respond to the approval prompt for '"
            + std::string(tool->name()) + "'.");
    }
    std::string decision = resp->value("decision", std::string("deny"));
    if (decision == "always_allow" && agent) {
        agent->setToolPolicy(tool->name(), ToolPolicy::AutoAllow);
    }
    if (decision == "deny" || decision == "denied") {
        return syntheticDeny("user_denied",
            "User denied the call to '" + std::string(tool->name()) + "'.");
    }
    // approve or always_allow → run for real.
    return runOneToolCall(registry, call);
}

// Run one tool and populate a ToolCallRecord with the result. Tool errors
// (throw / unknown name / failed validation) become ToolResult{ok=false}
// so the LLM gets feedback it can recover from instead of crashing the turn.
ToolCallRecord runOneToolCall(tools::ToolRegistry* registry,
                              const provider::ProviderToolCall& call)
{
    ToolCallRecord rec;
    rec.call_id   = call.call_id;
    rec.name      = call.name;
    rec.args_json = call.args_json.empty() ? std::string("{}") : call.args_json;

    if (!registry) {
        rec.ok            = false;
        rec.error_message = "No tool registry initialized.";
        rec.result_json   = R"({"error":"no_registry"})";
        return rec;
    }
    tools::ITool* t = registry->find(call.name);
    if (!t) {
        rec.ok            = false;
        rec.error_message = "Tool '" + call.name + "' is not registered.";
        rec.result_json   = nlohmann::json{ {"error", "unknown_tool"} }.dump();
        return rec;
    }

    // Scrub invalid UTF-8 from the streamed args before dispatching. SSE
    // input_json_delta fragments can split a multi-byte UTF-8 sequence
    // across two deltas, leaving an isolated continuation byte (e.g.
    // 0xA6) in the accumulated buffer. nlohmann::json::parse rejects
    // that; downstream nlohmann::json::dump on the error message would
    // re-throw and the agent loop stalls.
    //
    // Strategy: replace any invalid byte / orphaned continuation /
    // truncated sequence with U+FFFD (\xEF\xBF\xBD). Args are almost
    // always ASCII (queries, names, paths); the rare emoji or smart
    // quote becomes a replacement char in the worst case, which
    // tools handle gracefully. This is at the dispatch chokepoint so
    // every tool benefits without per-tool code.
    auto scrubInvalidUtf8 = [](const std::string& s) -> std::string {
        std::string out;
        out.reserve(s.size());
        const unsigned char* p = reinterpret_cast<const unsigned char*>(s.data());
        size_t n = s.size();
        size_t i = 0;
        while (i < n) {
            unsigned char c = p[i];
            if (c < 0x80) { out.push_back(static_cast<char>(c)); ++i; continue; }
            int expected = 0;
            if      ((c & 0xE0) == 0xC0) expected = 2;
            else if ((c & 0xF0) == 0xE0) expected = 3;
            else if ((c & 0xF8) == 0xF0) expected = 4;
            else { out.append("\xEF\xBF\xBD", 3); ++i; continue; }
            if (i + expected > n) { out.append("\xEF\xBF\xBD", 3); i = n; continue; }
            bool valid = true;
            for (int k = 1; k < expected; ++k) {
                if ((p[i + k] & 0xC0) != 0x80) { valid = false; break; }
            }
            if (!valid) { out.append("\xEF\xBF\xBD", 3); ++i; continue; }
            for (int k = 0; k < expected; ++k) out.push_back(static_cast<char>(p[i + k]));
            i += expected;
        }
        return out;
    };
    if (!rec.args_json.empty()) {
        std::string scrubbed = scrubInvalidUtf8(rec.args_json);
        if (scrubbed != rec.args_json) {
            bridge::consolePrint("[ModelerAI] tool dispatch: scrubbed invalid UTF-8 from args ("
                + std::to_string(rec.args_json.size() - scrubbed.size())
                + " byte delta) — likely SSE delta-boundary split a multi-byte sequence\n");
        }
        rec.args_json = std::move(scrubbed);
    }

    try {
        tools::ToolResult r = t->run(rec.args_json);
        rec.ok = r.ok;
        if (r.ok) {
            // Tool returned structured success — pass result_json straight
            // through (already JSON per the ITool contract).
            rec.result_json = r.result_json.empty() ? "{}" : r.result_json;
        } else {
            nlohmann::json err;
            err["error"]   = r.error_code.empty() ? "tool_failed" : r.error_code;
            err["message"] = r.error_message;
            rec.result_json   = err.dump();
            rec.error_message = r.error_message;
        }
    } catch (const std::exception& e) {
        rec.ok            = false;
        rec.error_message = std::string("Tool threw: ") + e.what();
        nlohmann::json err;
        err["error"]   = "tool_exception";
        err["message"] = rec.error_message;
        rec.result_json = err.dump();
    } catch (...) {
        rec.ok            = false;
        rec.error_message = "Tool threw a non-std exception.";
        rec.result_json   = R"({"error":"tool_exception"})";
    }
    return rec;
}

} // namespace

Agent::Agent(provider::IProvider* provider, OutboundPush push)
    : provider_(provider), push_(std::move(push))
{
}

void Agent::startTurn(std::string turnId,
                      std::string userText,
                      std::string condensedModel,
                      std::vector<Attachment> attachments)
{
    bridge::consolePrint("[ModelerAI] Agent::startTurn entering\n");

    // Cost-ceiling guardrail. Reads
    // the user-configured warn / stop USD thresholds from settings.
    // Zero on a threshold means "disabled" for that threshold.
    {
        auto eff = settings::resolveEffectiveSettings();
        double sofar = 0.0;
        { std::lock_guard<std::mutex> lk(statsMutex_); sofar = stats_.cost_usd; }

        if (eff.cost_stop_usd > 0.0 && sofar >= eff.cost_stop_usd) {
            bridge::consolePrint(("[ModelerAI] cost stop threshold hit: "
                                  + std::to_string(sofar) + " >= "
                                  + std::to_string(eff.cost_stop_usd) + "\n"));
            nlohmann::json p;
            p["code"]      = "cost_stop_exceeded";
            p["message"]   = "Session cost ($" + std::to_string(sofar)
                             + ") has reached your hard-stop ceiling ($"
                             + std::to_string(eff.cost_stop_usd) + "). "
                             "Use /clear to reset session stats, or raise / disable "
                             "the ceiling in Settings → Cost.";
            p["retriable"] = false;
            p["sofar"]     = sofar;
            p["stop_at"]   = eff.cost_stop_usd;
            push_(makeEnvelope("error", turnId, std::move(p)));
            return;
        }
        if (eff.cost_warn_usd > 0.0 && sofar >= eff.cost_warn_usd) {
            // Non-blocking: push a warning the viewer renders inline,
            // but still proceed with the turn.
            nlohmann::json p;
            p["key"] = "cost_warning";
            nlohmann::json v;
            v["sofar"]   = sofar;
            v["warn_at"] = eff.cost_warn_usd;
            v["stop_at"] = eff.cost_stop_usd;
            p["value"] = std::move(v);
            push_(makeEnvelope("state_update", turnId, std::move(p)));
        }
    }

    cancelRequested_.store(false);
    turnInFlight_.store(true);

    // Snapshot history under the lock, then build a turn-local
    // Conversation copy the worker thread owns by-value.
    auto convCopy = std::make_shared<Conversation>();
    {
        std::lock_guard<std::mutex> lk(historyMutex_);
        Message userMsg{
            Role::User,
            userText,
            std::move(attachments),  // attachments ride on the User message
            {},
            std::nullopt,
            std::chrono::system_clock::now()
        };
        history_.messages.push_back(userMsg);
        *convCopy = history_;
    }

    // Load user rules + KB and assemble the split system prompt. The
    // STABLE prefix (persona + rules + user_rules + catalogs + KB) is
    // what gets marked cacheable by the Anthropic adapter. The mode hint
    // lives in the VARIABLE suffix so mode flips don't invalidate the
    // cache.
    std::string userRules    = loadUserRules();
    // Always-on hand-written topics. The strategy is "thin quickrefs in
    // always-on, full bodies on demand via read_topic". Each quickref is
    // signatures + critical-only gotchas. Pulling the full bodies in
    // previously cost ~15K tokens per turn — wasteful when the model only
    // needs the cheat sheet most of the time. Each quickref points at its
    // full reference topic by id so the model knows what to fetch when it
    // needs more.
    std::string kbBlock;
    auto appendTopic = [&](std::string_view topicId) {
        std::string body = kb::buildKbBlockForTopic(topicId);
        if (body.empty()) return;
        if (!kbBlock.empty()) kbBlock.append("\n\n");
        kbBlock.append(body);
    };
    // Post-reset (2026-06-02): always-on layer trimmed to ONE topic, the
    // minimal signatures-only modelerai-quickref. Per-class schemas + topic
    // routing are being rebuilt modeler-led; nothing lands here without
    // having survived a real learning-session prompt test. See
    // docs/learning-sessions.md.
    appendTopic("modelerai-quickref");          // current tool surface, signatures only
    std::string catalogIndex = kb::loadCatalog("index");

    SystemPromptInputs spi;
    spi.condensedModel = condensedModel;
    spi.kbBlock        = kbBlock;
    spi.catalogIndex   = catalogIndex;
    spi.userRules      = userRules;
    spi.mode           = mode();
    auto parts = buildSystemPrompt(spi);
    convCopy->system_prompt_stable   = std::move(parts.stable_prefix);
    convCopy->system_prompt_variable = std::move(parts.variable_suffix);

    // Debug-pane push: if the user has show_debug_info enabled, ship the
    // assembled system prompt + a few headline fields to the viewer so
    // the debug drawer can render them. Kept behind the flag so the
    // payload (potentially large) isn't on the wire by default.
    {
        auto effForDebug = settings::resolveEffectiveSettings();
        if (effForDebug.show_debug_info) {
            nlohmann::json p;
            p["key"] = "debug_turn";
            nlohmann::json v;
            v["turn_id"]              = turnId;
            v["mode"]                 = modeName(mode());
            v["provider_id"]          = provider_ ? provider_->id() : "";
            v["active_model_id"]      = provider_ ? provider_->active_model_id() : "";
            {
                provider::EffortLevel el = provider_ ? provider_->effort() : provider::EffortLevel::Off;
                const char* es = "off";
                if      (el == provider::EffortLevel::Low)    es = "low";
                else if (el == provider::EffortLevel::Medium) es = "medium";
                else if (el == provider::EffortLevel::High)   es = "high";
                v["effort"] = es;
            }
            v["user_text_first_120"]  = userText.substr(0, 120);
            v["system_prompt_stable"] = convCopy->system_prompt_stable;
            v["system_prompt_var"]    = convCopy->system_prompt_variable;
            v["condensed_size_bytes"] = (long long)condensedModel.size();
            p["value"] = std::move(v);
            push_(makeEnvelope("state_update", turnId, std::move(p)));
        }
    }

    auto turnIdShared = std::make_shared<std::string>(std::move(turnId));
    auto pushFn       = push_;
    auto provider     = provider_;
    auto self         = this;
    auto registry     = bootstrap::toolRegistry();
    auto currentMode  = mode();  // snapshot under the lock once for the turn

    // Snapshot the user-configurable tool-iteration cap on the main thread
    // (settings touches the FlexSim SDK; worker thread can't read it
    // directly). Default 25; user can change via Settings -> Usage.
    int maxIterations = settings::resolveEffectiveSettings().max_tool_iterations;
    if (maxIterations < 1) maxIterations = 1;
    if (maxIterations > 500) maxIterations = 500;

    std::thread([self, provider, pushFn, turnIdShared, convCopy, registry, currentMode, maxIterations]() {
        bridge::consolePrint("[ModelerAI] turn worker thread started\n");

        // ---- per-turn aggregates (accumulated across iterations) ----
        SessionStats turnStatsDelta;
        std::string  lastMessageId;
        bool         erroredOut = false;

        // We do NOT mutate self->history_ inside the loop until success;
        // the loop builds up a private "extra history" of assistant +
        // tool messages and we commit at the end. This keeps a mid-loop
        // crash from leaving the persistent history in a half-written state.
        std::vector<Message> extraHistory;

        // The conversation actually sent to the provider — starts as the
        // snapshot the user message was just appended to, grows as the
        // loop adds assistant + tool messages.
        Conversation working = *convCopy;

        // tools[] is computed once per loop iteration — cheap; registry
        // contents could in principle change (when we add hot-reload of
        // user-authored tools in a later milestone).
        for (int iteration = 0; iteration < maxIterations; ++iteration) {

            // Mode-aware tool filtering. Ask mode hides mutating tools
            // from the LLM entirely (saves tokens + clearer "this mode
            // is safe"). Plan + Apply expose everything; the dispatch
            // layer below handles per-call gating.
            std::vector<agent::ToolDef> toolDefs;
            if (registry) {
                auto all = registry->toolDefs();
                if (currentMode == AgentMode::Ask) {
                    for (const auto& td : all) {
                        tools::ITool* t = registry->find(td.name);
                        if (t && t->category() == tools::Category::ReadOnly) {
                            toolDefs.push_back(td);
                        }
                    }
                } else {
                    toolDefs = std::move(all);
                }
            }

            // ---- per-iteration scratch ----
            std::string                              accumulated;     // text streamed
            std::vector<provider::ProviderToolCall>  collectedCalls;  // tool_use blocks
            std::optional<provider::TurnResult>      done;
            std::optional<provider::ProviderError>   err;

            provider::StreamCallback onChunk = [&](const provider::StreamChunk& c) {
                accumulated.append(c.text);
                nlohmann::json p;
                p["turn_id"] = *turnIdShared;
                p["text"]    = c.text;
                pushFn(makeEnvelope("assistant_chunk", *turnIdShared, std::move(p)));
            };

            provider::ToolCallCallback onToolCall = [&](const provider::ProviderToolCall& c) {
                bridge::consolePrint(("[ModelerAI] tool_use received: " + c.name + " call_id=" + c.call_id + "\n"));
                collectedCalls.push_back(c);
                // Tell the viewer "we're about to run this." Two views:
                // args_summary is the short preview (~80 chars) shown inline;
                // args_full is the unabridged JSON the viewer reveals when the
                // user clicks to expand. 64K cap on the full version is a
                // runaway-payload safety guard, not a UX choice.
                std::string argsFull    = c.args_json.empty() ? std::string("{}") : c.args_json;
                std::string argsSummary = argsFull;
                if (argsSummary.size() > 80) argsSummary = argsSummary.substr(0, 80) + "…";
                if (argsFull.size() > 65536) argsFull = argsFull.substr(0, 65536) + "…[truncated at 64K]";
                nlohmann::json p;
                p["turn_id"]      = *turnIdShared;
                p["call_id"]      = c.call_id;
                p["name"]         = c.name;
                p["args_summary"] = argsSummary;
                p["args_full"]    = argsFull;
                pushFn(makeEnvelope("tool_call_start", *turnIdShared, std::move(p)));
            };

            provider::DoneCallback onDone = [&](const provider::TurnResult& r) {
                done = r;
            };

            provider::ErrorCallback onError = [&](const provider::ProviderError& e) {
                err = e;
            };

            provider->stream_turn(working, toolDefs, onChunk, onToolCall, onDone, onError,
                                  &self->cancelRequested_);

            if (err.has_value()) {
                bridge::consolePrint(("[ModelerAI] turn error code=" + err->code
                                      + " msg=" + err->message + "\n"));
                nlohmann::json p;
                p["code"]      = err->code;
                p["message"]   = err->message;
                p["retriable"] = err->retriable;
                pushFn(makeEnvelope("error", *turnIdShared, std::move(p)));

                // Preserve the partial assistant text from the iteration
                // that was interrupted. `accumulated` is per-iteration
                // scratch (resets each loop) and will go out of scope after
                // we break, so capture it now into extraHistory. The
                // post-loop commit block flushes extraHistory regardless of
                // erroredOut, so this survives both plain user-Stop and the
                // model-switch cancel path. Earlier iterations' completed
                // assistant+tool messages were already pushed into
                // extraHistory at the bottom of the iteration and survive
                // unchanged.
                if (err->code == "cancelled" && !accumulated.empty()) {
                    const char* marker =
                        self->modelSwitchCancel_.load()
                            ? "\n\n_[Interrupted — model switched]_"
                            : "\n\n_[Interrupted]_";
                    Message asst{
                        Role::Assistant,
                        accumulated + marker,
                        {}, {}, std::nullopt,
                        std::chrono::system_clock::now()
                    };
                    extraHistory.push_back(std::move(asst));
                    bridge::consolePrint("[ModelerAI] partial reply preserved with interrupted marker\n");
                }
                erroredOut = true;
                break;
            }
            if (done.has_value()) {
                turnStatsDelta.input_tokens                += done->input_tokens;
                turnStatsDelta.output_tokens               += done->output_tokens;
                turnStatsDelta.cache_read_input_tokens     += done->cache_read_input_tokens;
                turnStatsDelta.cache_creation_input_tokens += done->cache_creation_input_tokens;
                turnStatsDelta.cost_usd                    += done->cost_usd;
                lastMessageId = done->message_id;
            }

            // No tool calls this iteration → normal text turn complete.
            if (collectedCalls.empty()) {
                Message asst{
                    Role::Assistant, accumulated, {}, {}, std::nullopt,
                    std::chrono::system_clock::now()
                };
                extraHistory.push_back(asst);
                break;
            }

            // Tool calls came back. Append the assistant message that
            // contains them, then run each tool and append the tool-result
            // message, then loop to let the LLM continue with the results.
            std::vector<ToolCallRecord> asstCalls;
            std::vector<ToolCallRecord> resultRecs;
            asstCalls.reserve(collectedCalls.size());
            resultRecs.reserve(collectedCalls.size());
            for (const auto& c : collectedCalls) {
                ToolCallRecord stub;
                stub.call_id   = c.call_id;
                stub.name      = c.name;
                stub.args_json = c.args_json.empty() ? std::string("{}") : c.args_json;
                asstCalls.push_back(stub);

                ToolCallRecord result;

                // Look up the tool to determine category, policy, and
                // whether it's our special ask_user_question primitive.
                tools::ITool* t = registry ? registry->find(c.name) : nullptr;

                if (c.name == kAskUserQuestionName) {
                    // Pause-and-wait for inline user input.
                    result = runAskUserQuestion(c, &self->interactionBroker(),
                                                pushFn, *turnIdShared);
                }
                else if (currentMode == AgentMode::Plan
                         && t && t->category() != tools::Category::ReadOnly)
                {
                    // Plan mode + mutating tool: intercept. Return a
                    // synthetic "would have run" result so the LLM can
                    // describe its plan to the user without mutating.
                    result.call_id = c.call_id;
                    result.name    = c.name;
                    result.args_json = stub.args_json;
                    result.ok      = true;
                    nlohmann::json r;
                    r["plan_mode"]         = true;
                    r["would_have_called"] = c.name;
                    r["with_args"]         = stub.args_json;
                    r["note"]              = "Plan Mode is active. Describe what this would accomplish; do not assume it ran.";
                    result.result_json = r.dump();
                }
                else if (!t) {
                    // Unknown tool (registry doesn't know it). Fall back
                    // to the simple sync path so the error propagates
                    // cleanly to the LLM.
                    result = runOneToolCall(registry, c);
                }
                else {
                    // Apply mode (or Plan with a read-only tool): go
                    // through the approval-aware path. Read-only tools
                    // bypass the modal inside runWithApproval.
                    ToolPolicy pol = self->toolPolicy(c.name);
                    result = runWithApproval(c, t, pol, &self->interactionBroker(),
                                             pushFn, *turnIdShared, registry, self);
                }

                // Push tool_call_done so the viewer can render the outcome.
                // summary is the ~240-char inline preview; summary_full is
                // the unabridged JSON (capped at 64K for safety) the viewer
                // reveals on click-to-expand.
                nlohmann::json p;
                p["turn_id"] = *turnIdShared;
                p["call_id"] = result.call_id;
                p["ok"]      = result.ok;
                std::string summaryFull = result.ok ? result.result_json : result.error_message;
                std::string summary     = summaryFull;
                if (summary.size() > 240)     summary     = summary.substr(0, 240) + "…";
                if (summaryFull.size() > 65536) summaryFull = summaryFull.substr(0, 65536) + "…[truncated at 64K]";
                p["summary"]      = summary;
                p["summary_full"] = summaryFull;
                pushFn(makeEnvelope("tool_call_done", *turnIdShared, std::move(p)));

                resultRecs.push_back(result);
            }

            Message asst{
                Role::Assistant, accumulated, {}, asstCalls, std::nullopt,
                std::chrono::system_clock::now()
            };
            Message toolMsg{
                Role::Tool, "", {}, resultRecs, std::nullopt,
                std::chrono::system_clock::now()
            };
            extraHistory.push_back(asst);
            extraHistory.push_back(toolMsg);
            working.messages.push_back(asst);
            working.messages.push_back(toolMsg);

            if (iteration + 1 == maxIterations) {
                bridge::consolePrint("[ModelerAI] WARN: tool-loop max iterations reached\n");
                nlohmann::json p;
                p["code"]      = "tool_loop_max";
                p["message"]   = "Tool loop hit the " + std::to_string(maxIterations)
                                + "-iteration cap and was aborted to prevent runaway costs. "
                                + "If this was a legitimate multi-step build, raise the cap "
                                + "in Settings -> Usage -> Max tool iterations (range 1-500). "
                                + "The current session's stats are preserved.";
                p["retriable"]    = false;
                p["limit_hit"]    = maxIterations;
                p["setting_path"] = "Settings > Usage > Max tool iterations";
                pushFn(makeEnvelope("error", *turnIdShared, std::move(p)));
                erroredOut = true;
            }
        }

        // ---- commit history + counters ----
        // Always flush extraHistory — completed tool round-trips and the
        // (possibly [Interrupted]-marked) partial assistant text deserve to
        // persist even on cancel/error. That's how /export captures what
        // the user actually saw, and how the next turn's context-window
        // includes the work already done this turn.
        {
            std::lock_guard<std::mutex> lk(self->historyMutex_);
            for (auto& m : extraHistory) self->history_.messages.push_back(std::move(m));
        }
        // (Free-quota counter increment removed — product is freeware.)

        // Accumulate stats regardless — even on error, the tokens cost
        // real money and the user should see them.
        SessionStats snapshot;
        {
            std::lock_guard<std::mutex> lk(self->statsMutex_);
            self->stats_.input_tokens                += turnStatsDelta.input_tokens;
            self->stats_.output_tokens               += turnStatsDelta.output_tokens;
            self->stats_.cache_read_input_tokens     += turnStatsDelta.cache_read_input_tokens;
            self->stats_.cache_creation_input_tokens += turnStatsDelta.cache_creation_input_tokens;
            self->stats_.cost_usd                    += turnStatsDelta.cost_usd;
            if (!erroredOut) self->stats_.turns      += 1;
            snapshot = self->stats_;
        }

        // Fire one repaintall() per turn so every view (3D, tree, charts)
        // catches up with whatever the AI did this turn — creates, deletes,
        // connections, property edits like Location/Rotation/Size/Color
        // that wouldn't otherwise show until the next event. Dispatched
        // through mainthread because we're on the worker thread here and
        // FlexSim's tree mutations are main-thread-only. Failure is
        // swallowed; we don't fail a successful turn over a repaint.
        try {
            ModelerAi::mainthread::runAndWait([]() {
                try { executestring("repaintall();", nullptr, nullptr, Variant()); } catch (...) {}
            });
        } catch (...) {}

        // assistant_done — closes the bubble in the viewer.
        {
            nlohmann::json p;
            p["turn_id"]    = *turnIdShared;
            p["message_id"] = lastMessageId;
            pushFn(makeEnvelope("assistant_done", *turnIdShared, std::move(p)));
        }
        {
            nlohmann::json p;
            p["key"]   = "session_stats";
            p["value"] = statsPayload(snapshot);
            pushFn(makeEnvelope("state_update", *turnIdShared, std::move(p)));
        }
        // Persist if we have a session key (best-effort).
        {
            Conversation snap;
            std::string key;
            {
                std::lock_guard<std::mutex> lk(self->historyMutex_);
                snap = self->history_;
                key  = self->session_key_;
            }
            if (!key.empty()) {
                bool savedOk = saveSession(key, snap);
                if (!savedOk) {
                    bridge::consolePrint(("[ModelerAI] WARN: saveSession failed for key=" + key + "\n"));
                } else {
                    bridge::consolePrint(("[ModelerAI] session saved key=" + key
                                          + " messages=" + std::to_string(snap.messages.size()) + "\n"));
                }
                // Push a transient indicator to the viewer either way so
                // the user sees confirmation (or failure) after a turn.
                nlohmann::json sp;
                sp["key"] = "session_save_status";
                nlohmann::json v;
                v["ok"]            = savedOk;
                v["messages"]      = (int)snap.messages.size();
                v["session_key"]   = key;
                v["saved_at_ms"]   = (long long)
                    std::chrono::duration_cast<std::chrono::milliseconds>(
                        std::chrono::system_clock::now().time_since_epoch()).count();
                sp["value"] = std::move(v);
                pushFn(makeEnvelope("state_update", *turnIdShared, std::move(sp)));
            }
        }

        self->turnInFlight_.store(false);
        self->cancelRequested_.store(false);
        self->modelSwitchCancel_.store(false);
    }).detach();
}

bool Agent::cancelCurrentTurn()
{
    if (!turnInFlight_.load()) return false;
    cancelRequested_.store(true);
    bridge::consolePrint("[ModelerAI] cancelCurrentTurn requested\n");
    return true;
}

bool Agent::cancelForModelSwitch()
{
    if (!turnInFlight_.load()) return false;
    // Set the flag BEFORE tripping cancelRequested so the worker sees
    // both atomically when it processes the cancel.
    modelSwitchCancel_.store(true);
    cancelRequested_.store(true);
    bridge::consolePrint("[ModelerAI] cancelForModelSwitch requested (partial will be preserved)\n");
    return true;
}

int Agent::clearConversation()
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    int n = (int)history_.messages.size();
    history_.messages.clear();
    history_.id.clear();
    return n;
}

SessionStats Agent::stats() const
{
    std::lock_guard<std::mutex> lk(statsMutex_);
    return stats_;
}

void Agent::resetStats()
{
    std::lock_guard<std::mutex> lk(statsMutex_);
    stats_ = SessionStats{};
}

Conversation Agent::conversationCopy() const
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    return history_;
}

void Agent::replaceConversation(Conversation loaded)
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    // Preserve session_key — caller already set it (or will).
    std::string keptKey = session_key_;
    history_ = std::move(loaded);
    session_key_ = keptKey;
}

std::string Agent::sessionKey() const
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    return session_key_;
}

void Agent::setSessionKey(std::string key)
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    session_key_ = std::move(key);
}

void Agent::setSessionStrategy(std::string strategy, std::string customKey)
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    session_strategy_   = std::move(strategy);
    session_custom_key_ = std::move(customKey);
}

std::string Agent::sessionStrategy() const
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    return session_strategy_;
}

std::string Agent::sessionCustomKey() const
{
    std::lock_guard<std::mutex> lk(historyMutex_);
    return session_custom_key_;
}

// ----- mode + policy public surface -----

AgentMode Agent::mode() const
{
    std::lock_guard<std::mutex> lk(modeMutex_);
    return mode_;
}

void Agent::setMode(AgentMode m)
{
    std::lock_guard<std::mutex> lk(modeMutex_);
    mode_ = m;
}

ToolPolicy Agent::toolPolicy(std::string_view name) const
{
    std::lock_guard<std::mutex> lk(policyMutex_);
    auto it = toolPolicies_.find(std::string(name));
    if (it == toolPolicies_.end()) return ToolPolicy::Ask;
    return it->second;
}

void Agent::setToolPolicy(std::string_view name, ToolPolicy p)
{
    std::lock_guard<std::mutex> lk(policyMutex_);
    toolPolicies_[std::string(name)] = p;
}

// ----- wire-format helpers -----

const char* modeName(AgentMode m)
{
    switch (m) {
        case AgentMode::Ask:   return "ask";
        case AgentMode::Plan:  return "plan";
        case AgentMode::Apply: return "apply";
    }
    return "ask";
}

AgentMode parseMode(std::string_view s)
{
    if (s == "plan")  return AgentMode::Plan;
    if (s == "apply") return AgentMode::Apply;
    return AgentMode::Ask;
}

const char* policyName(ToolPolicy p)
{
    switch (p) {
        case ToolPolicy::Ask:        return "ask";
        case ToolPolicy::AutoAllow:  return "auto_allow";
        case ToolPolicy::AlwaysDeny: return "always_deny";
    }
    return "ask";
}

ToolPolicy parsePolicy(std::string_view s)
{
    if (s == "auto_allow"  || s == "always_allow") return ToolPolicy::AutoAllow;
    if (s == "always_deny" || s == "deny")         return ToolPolicy::AlwaysDeny;
    return ToolPolicy::Ask;
}

} // namespace ModelerAi::agent
