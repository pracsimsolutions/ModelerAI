// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/anthropic.h"
#include "provider/http_client.h"
#include "provider/sse_parser.h"
#include "agent/message.h"
#include "bootstrap.h"
#include "bridge/log.h"
#include "bridge/diag.h"
#include "keystore/keystore.h"
#include "third_party/json.h"

#include <windows.h>   // Sleep (for the 429 retry backoff)

#include <algorithm>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

namespace ModelerAi::provider {

namespace {

constexpr const char* kDefaultModel = "claude-haiku-4-5";

// Per-model max_tokens defaults. Sized so the model can emit a large
// batch of tool_use blocks in one response without truncating (each
// tool_use is ~100-250 output tokens; capping at ~10-14 calls hurts
// batching throughput). Override with /effort when thinking is enabled
// — see thinkingBudgetTokens() below.
int defaultMaxTokens(const std::string& modelId)
{
    if (modelId.find("haiku") != std::string::npos) return 4096;  // was 1024 — room for ~20-30 tool_use batch
    if (modelId.find("opus")  != std::string::npos) return 8192;  // was 4096 — Opus often plans larger builds
    return 8192; // sonnet + anything else (was 2048 — was the reported "caps at 10" cause)
}

int thinkingBudgetTokens(EffortLevel e)
{
    switch (e) {
        case EffortLevel::Off:    return 0;
        case EffortLevel::Low:    return 2000;
        case EffortLevel::Medium: return 8000;
        case EffortLevel::High:   return 16000;
    }
    return 0;
}

// Key resolution order:
//   1. DPAPI keystore entry for "anthropic" (the in-app paste-once UX).
//   2. ANTHROPIC_API_KEY environment variable (the original v0 path).
// Empty result = no key configured. The first non-empty wins.
std::string readApiKey()
{
    if (auto* ks = bootstrap::keystore()) {
        std::string s;
        if (ks->get("anthropic", s) && !s.empty()) return s;
    }
    const char* k = std::getenv("ANTHROPIC_API_KEY");
    if (!k) return "";
    return k;
}

nlohmann::json messageToJson(const agent::Message& m)
{
    nlohmann::json j;
    switch (m.role) {
        case agent::Role::User:      j["role"] = "user"; break;
        case agent::Role::Assistant: j["role"] = "assistant"; break;
        case agent::Role::Tool:      j["role"] = "user"; break;  // tool_result via user role
        case agent::Role::System:    j["role"] = "user"; break;  // shouldn't happen
    }

    // Assistant message that contained tool_use blocks: content is an
    // array of {text} + {tool_use, id, name, input}. Anthropic requires
    // the original tool_use blocks to be replayed exactly when sending
    // the follow-up turn so the tool_result blocks have something to
    // reference.
    if (m.role == agent::Role::Assistant && !m.tool_calls.empty()) {
        nlohmann::json content = nlohmann::json::array();
        if (!m.text.empty()) {
            content.push_back({{"type", "text"}, {"text", m.text}});
        }
        for (const auto& c : m.tool_calls) {
            nlohmann::json block;
            block["type"]  = "tool_use";
            block["id"]    = c.call_id;
            block["name"]  = c.name;
            // input must be a JSON object; parse from stored string.
            try {
                block["input"] = nlohmann::json::parse(
                    c.args_json.empty() ? std::string("{}") : c.args_json);
            } catch (...) {
                block["input"] = nlohmann::json::object();
            }
            content.push_back(std::move(block));
        }
        j["content"] = std::move(content);
        return j;
    }

    // Tool-result message: replayed as a user-role message whose content
    // is an array of {type: "tool_result", tool_use_id, content}. One
    // entry per tool that ran in the round preceding this message.
    if (m.role == agent::Role::Tool) {
        nlohmann::json content = nlohmann::json::array();
        for (const auto& c : m.tool_calls) {
            nlohmann::json block;
            block["type"]        = "tool_result";
            block["tool_use_id"] = c.call_id;
            // content can be a string OR an array of typed blocks.
            // Plain string is simplest + universally supported.
            block["content"]     = c.result_json.empty() ? "{}" : c.result_json;
            if (!c.ok) block["is_error"] = true;
            content.push_back(std::move(block));
        }
        if (content.empty()) content.push_back({{"type","text"},{"text",""}});
        j["content"] = std::move(content);
        return j;
    }

    // User message with image attachments: emit as a multi-block content
    // array. Anthropic accepts image/jpeg, image/png, image/gif, image/webp
    // as base64 sources. Order matters for the model — put images BEFORE
    // text so an instruction like "describe this" reads naturally against
    // the preceding image, which Anthropic's own examples recommend.
    if (m.role == agent::Role::User && !m.attachments.empty()) {
        nlohmann::json content = nlohmann::json::array();
        for (const auto& a : m.attachments) {
            // Defensive: skip media types Anthropic won't accept rather
            // than fail the whole request.
            const std::string& mt = a.media_type;
            if (mt != "image/jpeg" && mt != "image/png" &&
                mt != "image/gif"  && mt != "image/webp") continue;
            nlohmann::json block;
            block["type"] = "image";
            block["source"] = {
                {"type",       "base64"},
                {"media_type", mt},
                {"data",       a.data_base64}
            };
            content.push_back(std::move(block));
        }
        if (!m.text.empty()) {
            content.push_back({{"type", "text"}, {"text", m.text}});
        }
        // Defensive: if we filtered everything, fall through to a plain text
        // message so we don't ship an empty content array (which Anthropic
        // rejects with 400).
        if (!content.empty()) {
            j["content"] = std::move(content);
            return j;
        }
    }

    // Plain text message.
    j["content"] = m.text;
    return j;
}

// Per-content-block state we accumulate during streaming. Anthropic
// interleaves text and tool_use blocks — each has its own integer index
// and we don't know the type until content_block_start fires.
struct StreamingBlock {
    std::string type;            // "text" or "tool_use"
    std::string tool_use_id;
    std::string tool_use_name;
    std::string tool_input_json; // accumulates partial_json deltas
};

struct UsageSnapshot {
    int input_tokens = 0;
    int output_tokens = 0;
    int cache_read_input_tokens = 0;
    int cache_creation_input_tokens = 0;
};

void mergeUsage(UsageSnapshot& s, const nlohmann::json& u)
{
    if (!u.is_object()) return;
    if (u.contains("input_tokens") && u["input_tokens"].is_number_integer()) {
        s.input_tokens = u["input_tokens"].get<int>();
    }
    if (u.contains("output_tokens") && u["output_tokens"].is_number_integer()) {
        s.output_tokens = u["output_tokens"].get<int>();
    }
    if (u.contains("cache_read_input_tokens") && u["cache_read_input_tokens"].is_number_integer()) {
        s.cache_read_input_tokens = u["cache_read_input_tokens"].get<int>();
    }
    if (u.contains("cache_creation_input_tokens") && u["cache_creation_input_tokens"].is_number_integer()) {
        s.cache_creation_input_tokens = u["cache_creation_input_tokens"].get<int>();
    }
}

double estimateCost(const UsageSnapshot& u, const ModelInfo& m)
{
    constexpr double kMtok = 1'000'000.0;
    return (u.input_tokens                * m.input_usd_per_mtok       / kMtok)
         + (u.output_tokens               * m.output_usd_per_mtok      / kMtok)
         + (u.cache_creation_input_tokens * m.cache_write_usd_per_mtok / kMtok)
         + (u.cache_read_input_tokens     * m.cache_read_usd_per_mtok  / kMtok);
}

} // namespace

Anthropic::Anthropic() : active_model_id_(kDefaultModel) {}

std::string Anthropic::id() const { return "anthropic"; }
std::string Anthropic::display_name() const { return "Anthropic (Claude)"; }

std::vector<ModelInfo> Anthropic::models() const
{
    // Pricing as of knowledge cutoff (Jan 2026). Verify against current
    // anthropic.com/pricing before relying on these for business decisions.
    // Cache-write pricing is approximated as 1.25x input; cache-read at 0.1x.
    return {
        ModelInfo{
            /*id*/                       "claude-haiku-4-5",
            /*display_name*/             "Claude Haiku 4.5",
            /*context_tokens*/           200000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            /*supports_thinking*/        true,
            /*accepts_temperature*/      true,
            /*input_usd_per_mtok*/       1.00,
            /*output_usd_per_mtok*/      5.00,
            /*cache_write_usd_per_mtok*/ 1.25,
            /*cache_read_usd_per_mtok*/  0.10,
        },
        ModelInfo{
            /*id*/                       "claude-sonnet-4-6",
            /*display_name*/             "Claude Sonnet 4.6",
            /*context_tokens*/           1000000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            /*supports_thinking*/        true,
            /*accepts_temperature*/      true,
            /*input_usd_per_mtok*/       3.00,
            /*output_usd_per_mtok*/      15.00,
            /*cache_write_usd_per_mtok*/ 3.75,
            /*cache_read_usd_per_mtok*/  0.30,
        },
        ModelInfo{
            /*id*/                       "claude-opus-4-8",
            /*display_name*/             "Claude Opus 4.8",
            /*context_tokens*/           1000000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            // Opus 4.8 uses adaptive thinking (automatic, no `thinking`
            // budget field) — not extended thinking. And it rejects the
            // `temperature` parameter outright. See provider.h notes.
            /*supports_thinking*/        false,
            /*accepts_temperature*/      false,
            /*input_usd_per_mtok*/       5.00,
            /*output_usd_per_mtok*/      25.00,
            /*cache_write_usd_per_mtok*/ 6.25,
            /*cache_read_usd_per_mtok*/  0.50,
        },
    };
}

bool Anthropic::has_api_key() const
{
    // Reuses the same resolution chain (keystore -> env var) so the
    // Settings badge reflects reality without us duplicating the logic.
    return !readApiKey().empty();
}

std::string Anthropic::active_model_id() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return active_model_id_;
}

bool Anthropic::set_active_model(std::string_view id)
{
    std::string target(id);
    for (const auto& m : models()) {
        if (m.id == target) {
            std::lock_guard<std::mutex> lk(stateMutex_);
            active_model_id_ = target;
            // If the new model doesn't support thinking, reset effort.
            if (!m.supports_thinking && effort_ != EffortLevel::Off) {
                effort_ = EffortLevel::Off;
            }
            return true;
        }
    }
    return false;
}

EffortLevel Anthropic::effort() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return effort_;
}

void Anthropic::set_effort(EffortLevel level)
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    // Find the active model and only honor effort if it supports thinking.
    for (const auto& m : models()) {
        if (m.id == active_model_id_) {
            if (!m.supports_thinking && level != EffortLevel::Off) return;
            break;
        }
    }
    effort_ = level;
}

TestKeyResult Anthropic::test_key()
{
    TestKeyResult r;
    std::string apiKey = readApiKey();
    if (apiKey.empty()) {
        r.error_message = "ANTHROPIC_API_KEY is not set.";
        return r;
    }

    // 1-token Haiku completion - cheapest possible validation call.
    std::vector<HttpHeader> headers = {
        {"x-api-key",         apiKey},
        {"anthropic-version", "2023-06-01"},
    };

    nlohmann::json req;
    req["model"]      = "claude-haiku-4-5";
    req["max_tokens"] = 1;
    req["messages"]   = nlohmann::json::array({
        nlohmann::json{{"role", "user"}, {"content", "."}}
    });
    std::string body = req.dump();

    int status = streamPost(
        "api.anthropic.com",
        "/v1/messages",
        headers,
        body,
        [](std::string_view) {});

    r.http_status = status;
    if (status == 0) {
        r.error_message = "Network error contacting Anthropic.";
        return r;
    }
    if (status >= 200 && status < 300) {
        r.ok = true;
        return r;
    }
    if (status == 401 || status == 403) {
        r.error_message = "Invalid API key.";
    } else if (status == 429) {
        r.error_message = "Rate limited or monthly cap reached.";
    } else {
        r.error_message = "HTTP " + std::to_string(status);
    }
    return r;
}

void Anthropic::stream_turn(
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    StreamCallback                     onChunk,
    ToolCallCallback                   onToolCall,
    DoneCallback                       onDone,
    ErrorCallback                      onError,
    CancelToken                        cancel)
{
    // Snapshot the active model + effort under the lock so a mid-turn
    // switch doesn't change behavior partway through.
    std::string modelId;
    EffortLevel effortLevel;
    {
        std::lock_guard<std::mutex> lk(stateMutex_);
        modelId     = active_model_id_;
        effortLevel = effort_;
    }
    // Look up the ModelInfo for pricing + capability flags.
    ModelInfo modelInfo{};
    bool foundModel = false;
    for (const auto& m : models()) {
        if (m.id == modelId) { modelInfo = m; foundModel = true; break; }
    }
    if (!foundModel) {
        if (onError) {
            onError(ProviderError{ "unknown_model",
                "Active model '" + modelId + "' not in catalog.", false });
        }
        return;
    }

    bridge::consolePrint(("[ModelerAI] Anthropic::stream_turn model=" + modelId + "\n"));

    std::string apiKey = readApiKey();
    if (apiKey.empty()) {
        bridge::consolePrint("[ModelerAI] ERR: ANTHROPIC_API_KEY not set\n");
        if (onError) {
            onError(ProviderError{
                "no_api_key",
                "ANTHROPIC_API_KEY is not set. Set it in your environment "
                "before launching FlexSim, then restart FlexSim.",
                false});
        }
        return;
    }

    int maxTokens = defaultMaxTokens(modelId);
    int budget = thinkingBudgetTokens(effortLevel);

    nlohmann::json req;
    req["model"]      = modelId;
    req["stream"]     = true;
    // When thinking is enabled, max_tokens must exceed the thinking budget.
    // We give the model headroom for its visible reply on top of the budget.
    req["max_tokens"] = (budget > 0) ? (budget + maxTokens) : maxTokens;

    if (budget > 0 && modelInfo.supports_thinking) {
        nlohmann::json thinking;
        thinking["type"]          = "enabled";
        thinking["budget_tokens"] = budget;
        req["thinking"] = std::move(thinking);
        bridge::consolePrint(("[ModelerAI] effort enabled budget=" + std::to_string(budget) + "\n"));
        // Extended thinking requires temperature = 1.0 per Anthropic.
        // Omit the field to use the API default.
    } else if (modelInfo.accepts_temperature) {
        // Lower temperature for code-generation calls. Multiple 2024
        // studies (Spracklen et al., Importing Phantoms) found
        // hallucination rate falls roughly linearly with temperature
        // for code identifiers. 0.3 keeps enough variance for
        // reasoning + planning while substantially reducing the
        // "make up a plausible-looking command name" failure mode.
        req["temperature"] = 0.3;
    }
    // else: Opus 4.8 path — neither extended thinking nor temperature.
    // The model picks its own thinking budget via adaptive thinking (the
    // `effort` parameter defaults to "high" if unset) and rejects any
    // temperature value.

    auto systemArray = nlohmann::json::array();
    if (!conv.system_prompt_stable.empty()) {
        systemArray.push_back({
            {"type",          "text"},
            {"text",          conv.system_prompt_stable},
            {"cache_control", { {"type", "ephemeral"} }}
        });
    }
    if (!conv.system_prompt_variable.empty()) {
        systemArray.push_back({
            {"type", "text"},
            {"text", conv.system_prompt_variable}
        });
    }
    if (!systemArray.empty()) {
        req["system"] = std::move(systemArray);
    }

    auto& msgs = req["messages"] = nlohmann::json::array();
    for (const auto& m : conv.messages) {
        if (m.role == agent::Role::System) continue;
        msgs.push_back(messageToJson(m));
    }

    // tools[] — Anthropic shape is [{name, description, input_schema}, ...].
    // The agent passes the registry's tool defs; empty vector means "no
    // tools for this turn" and the field is omitted to avoid sending
    // empty arrays the API treats as meaningful.
    if (!tools.empty()) {
        nlohmann::json toolsArr = nlohmann::json::array();
        for (const auto& td : tools) {
            nlohmann::json t;
            t["name"]        = td.name;
            t["description"] = td.description;
            try {
                t["input_schema"] = nlohmann::json::parse(
                    td.params_schema_json.empty()
                        ? std::string(R"({"type":"object","properties":{}})")
                        : td.params_schema_json);
            } catch (...) {
                // Malformed schema → fall back to an empty-object schema so
                // the call doesn't outright fail, but log so the author can
                // fix the offending tool.
                bridge::consolePrint(("[ModelerAI] WARN: tool '" + td.name
                                      + "' has invalid params_schema_json; using empty\n"));
                t["input_schema"] = {{"type","object"},{"properties", nlohmann::json::object()}};
            }
            toolsArr.push_back(std::move(t));
        }
        req["tools"] = std::move(toolsArr);
    }

    std::string body = req.dump();

    std::vector<HttpHeader> headers = {
        {"x-api-key",          apiKey},
        {"anthropic-version",  "2023-06-01"},
        {"accept",             "text/event-stream"},
    };

    UsageSnapshot usage;
    std::string messageId;

    // Anthropic interleaves text and tool_use content blocks within a
    // single message. Each has an integer `index`; we track per-index
    // state here so partial_json deltas for a tool's input get assembled
    // correctly even when multiple tool calls are emitted in one turn.
    std::map<int, StreamingBlock> blocksByIndex;

    SseParser parser([&](const SseEvent& ev) {
        if (ev.name == "message_start") {
            try {
                auto j = nlohmann::json::parse(ev.data);
                if (j.contains("message") && j["message"].is_object()) {
                    auto& msg = j["message"];
                    if (msg.contains("id") && msg["id"].is_string()) {
                        messageId = msg["id"].get<std::string>();
                    }
                    if (msg.contains("usage")) {
                        mergeUsage(usage, msg["usage"]);
                    }
                }
            } catch (...) {}
        }
        else if (ev.name == "content_block_start") {
            // Initialize state for the block index. tool_use blocks bring
            // along their id + name on this event; text blocks just need
            // to be tagged so subsequent deltas know they're text.
            try {
                auto j = nlohmann::json::parse(ev.data);
                int idx = j.value("index", 0);
                StreamingBlock b;
                if (j.contains("content_block") && j["content_block"].is_object()) {
                    const auto& cb = j["content_block"];
                    if (cb.contains("type") && cb["type"].is_string()) {
                        b.type = cb["type"].get<std::string>();
                    }
                    if (cb.contains("id") && cb["id"].is_string()) {
                        b.tool_use_id = cb["id"].get<std::string>();
                    }
                    if (cb.contains("name") && cb["name"].is_string()) {
                        b.tool_use_name = cb["name"].get<std::string>();
                    }
                }
                blocksByIndex[idx] = std::move(b);
            } catch (...) {}
        }
        else if (ev.name == "content_block_delta") {
            try {
                auto j = nlohmann::json::parse(ev.data);
                int idx = j.value("index", 0);
                auto it = blocksByIndex.find(idx);
                if (j.contains("delta") && j["delta"].is_object()) {
                    auto& d = j["delta"];
                    std::string dtype = (d.contains("type") && d["type"].is_string())
                        ? d["type"].get<std::string>() : "";

                    if (dtype == "text_delta" && d.contains("text") && d["text"].is_string()) {
                        if (onChunk) onChunk(StreamChunk{ d["text"].get<std::string>(), false });
                    }
                    else if (dtype == "input_json_delta"
                             && d.contains("partial_json")
                             && d["partial_json"].is_string()
                             && it != blocksByIndex.end())
                    {
                        // Accumulate the streaming JSON; we don't parse it
                        // until content_block_stop so partial chunks don't
                        // cause repeated parse failures.
                        it->second.tool_input_json.append(d["partial_json"].get<std::string>());
                    }
                    else if (dtype.empty() && d.contains("text") && d["text"].is_string()) {
                        // Legacy / fallback shape.
                        if (onChunk) onChunk(StreamChunk{ d["text"].get<std::string>(), false });
                    }
                    // thinking_delta: silent reasoning, billed as output but
                    // not displayed.
                }
            } catch (...) {}
        }
        else if (ev.name == "content_block_stop") {
            // When a tool_use block closes, finalize and emit the call.
            try {
                auto j = nlohmann::json::parse(ev.data);
                int idx = j.value("index", 0);
                auto it = blocksByIndex.find(idx);
                if (it != blocksByIndex.end() && it->second.type == "tool_use") {
                    if (onToolCall) {
                        ProviderToolCall pc;
                        pc.call_id  = it->second.tool_use_id;
                        pc.name     = it->second.tool_use_name;
                        pc.args_json = it->second.tool_input_json.empty()
                                       ? std::string("{}")
                                       : it->second.tool_input_json;
                        onToolCall(pc);
                    }
                    blocksByIndex.erase(it);
                }
            } catch (...) {}
        }
        else if (ev.name == "message_delta") {
            try {
                auto j = nlohmann::json::parse(ev.data);
                if (j.contains("usage")) {
                    mergeUsage(usage, j["usage"]);
                }
            } catch (...) {}
        }
        else if (ev.name == "message_stop") {
            if (onDone) {
                TurnResult r;
                r.message_id = messageId;
                r.input_tokens = usage.input_tokens;
                r.output_tokens = usage.output_tokens;
                r.cache_read_input_tokens = usage.cache_read_input_tokens;
                r.cache_creation_input_tokens = usage.cache_creation_input_tokens;
                r.cost_usd = estimateCost(usage, modelInfo);
                r.model_id = modelId;
                onDone(r);
            }
        }
        else if (ev.name == "error") {
            try {
                auto j = nlohmann::json::parse(ev.data);
                std::string msg = "Provider error";
                std::string code = "provider_error";
                if (j.contains("error") && j["error"].is_object()) {
                    auto& e = j["error"];
                    if (e.contains("message") && e["message"].is_string()) {
                        msg = e["message"].get<std::string>();
                    }
                    if (e.contains("type") && e["type"].is_string()) {
                        code = e["type"].get<std::string>();
                    }
                }
                if (onError) onError(ProviderError{ code, msg, true });
            } catch (...) {
                if (onError) onError(ProviderError{ "provider_error", "Anthropic returned an error", true });
            }
        }
    });

    // ----------------------------------------------------------------------
    // Retry loop for 429s. Anthropic's rate-limit window is the per-minute
    // input/output-token budget; in long tool loops we burn through it fast.
    // Anthropic sets `retry-after` (seconds) on every 429. Honor that, with
    // a per-attempt sleep cap of 60s and a max of 3 attempts so we don't
    // hang the user behind an account-cap problem that won't clear.
    // ----------------------------------------------------------------------
    constexpr int kMaxAttempts        = 3;
    constexpr int kPerAttemptSleepCap = 60;  // seconds
    ResponseHeaderCapture capture;
    capture.wantHeaders = {
        "retry-after",
        "anthropic-ratelimit-input-tokens-remaining",
        "anthropic-ratelimit-input-tokens-reset",
        "anthropic-ratelimit-output-tokens-remaining",
        "anthropic-ratelimit-requests-remaining",
    };

    int  status = 0;
    int  attempts = 0;
    auto isCancelled = [&]() { return cancel && cancel->load(); };

    while (attempts < kMaxAttempts) {
        ++attempts;
        capture.responseHeaders.clear();
        // Drop any stale half-parsed bytes from a prior failed attempt
        // BEFORE re-streaming. The original event-handler closure stays
        // alive; only the internal buffers are cleared.
        if (attempts > 1) parser.reset();

        bridge::consolePrint("[ModelerAI] POST https://api.anthropic.com/v1/messages (streaming, attempt "
                              + std::to_string(attempts) + ")\n");
        // Capture up to ~8KB of raw stream bytes alongside the SSE parse so
        // that when the server returns a non-2xx (which doesn't speak SSE —
        // it's a JSON error envelope like `{"type":"error","error":{...}}`),
        // we can surface what Anthropic actually said. Without this the
        // user sees only "HTTP status=400" with no clue what's wrong.
        constexpr size_t kErrorBodyCap = 8192;
        std::string rawBody;
        rawBody.reserve(1024);
        status = streamPost(
            "api.anthropic.com",
            "/v1/messages",
            headers,
            body,
            [&](std::string_view chunk) {
                parser.feed(chunk);
                if (rawBody.size() < kErrorBodyCap) {
                    size_t take = std::min(chunk.size(), kErrorBodyCap - rawBody.size());
                    rawBody.append(chunk.data(), take);
                }
            },
            cancel,
            &capture);
        parser.flush();
        bridge::consolePrint(("[ModelerAI] HTTP status=" + std::to_string(status) + "\n"));
        // Surface error-body on any non-2xx so the next attempt + retry
        // decision is informed AND the user has something actionable.
        if (status > 0 && (status < 200 || status >= 300) && !rawBody.empty()) {
            bridge::consolePrint("[ModelerAI] HTTP error body: " + rawBody + "\n");
            diag::info("Anthropic " + std::to_string(status) + " body: " + rawBody);
        }

        // Don't retry on cancel or non-retryable HTTP.
        if (status == -1)                         break;
        if (status >= 200 && status < 300)        break;
        if (status > 0 && status != 429 && status < 500) break; // 4xx (non-429) — not retryable
        // status == 0 (TCP/network failure) DOES retry — flaky connections
        // are common and the agent currently surfaces them as "Network
        // error reaching Anthropic" forcing the user to retype. Two extra
        // attempts with short backoff catches transients without harming
        // the genuine offline case.

        if (attempts >= kMaxAttempts) break;

        // Decide how long to wait. Prefer Anthropic's retry-after; fall back
        // to exponential backoff (5s, 15s) if absent. Network failures
        // (status == 0) use shorter backoff — 2s then 5s — since they're
        // usually transient and we don't have a retry-after to honor.
        int sleepSec = 0;
        auto it = capture.responseHeaders.find("retry-after");
        if (it != capture.responseHeaders.end() && !it->second.empty()) {
            try { sleepSec = std::stoi(it->second); } catch (...) {}
        }
        if (sleepSec <= 0) {
            if (status == 0)        sleepSec = (attempts == 1) ? 2 : 5;
            else                    sleepSec = (attempts == 1) ? 5 : 15;
        }
        if (sleepSec > kPerAttemptSleepCap) sleepSec = kPerAttemptSleepCap;

        bridge::consolePrint(("[ModelerAI] HTTP " + std::to_string(status)
                               + " — retrying in " + std::to_string(sleepSec)
                               + "s (attempt " + std::to_string(attempts)
                               + "/" + std::to_string(kMaxAttempts) + ")\n"));

        // Sleep in short bites so the cancel flag still cuts through.
        for (int i = 0; i < sleepSec * 10 && !isCancelled(); ++i) {
            Sleep(100);
        }
        if (isCancelled()) { status = -1; break; }
    }

    if (status == -1) {
        // Cancel flag tripped mid-stream — surface as a non-retriable
        // "cancelled" error so the agent loop can commit partial output
        // and stop the iteration.
        if (onError) {
            onError(ProviderError{ "cancelled",
                "Turn cancelled by user.", false });
        }
        return;
    }
    if (status == 0) {
        if (onError) {
            onError(ProviderError{ "network",
                "Network error reaching Anthropic. Check your internet connection.",
                true });
        }
        return;
    }
    if (status < 200 || status >= 300) {
        if (onError) {
            std::string msg;
            if (status == 429) {
                msg = "Anthropic rate limit hit and " + std::to_string(kMaxAttempts)
                    + " automatic retries didn't clear it.";
                auto rIt = capture.responseHeaders.find("retry-after");
                if (rIt != capture.responseHeaders.end() && !rIt->second.empty()) {
                    msg += " Server suggests retry-after=" + rIt->second + "s.";
                }
                auto remIt = capture.responseHeaders.find("anthropic-ratelimit-input-tokens-remaining");
                if (remIt != capture.responseHeaders.end()) {
                    msg += " (input tokens remaining: " + remIt->second + ")";
                }
                msg += " Slow down (let conversation cool ~1 min), shrink the tool loop, or check your "
                       "Anthropic account tier (Settings → Usage). https://docs.anthropic.com/en/api/rate-limits";
            } else {
                msg = "Anthropic returned HTTP " + std::to_string(status);
            }
            onError(ProviderError{ "http_" + std::to_string(status), msg, status == 429 || status >= 500 });
        }
        return;
    }

    bridge::consolePrint(("[ModelerAI] usage input=" + std::to_string(usage.input_tokens)
                          + " output=" + std::to_string(usage.output_tokens)
                          + " cache_read=" + std::to_string(usage.cache_read_input_tokens)
                          + " cache_write=" + std::to_string(usage.cache_creation_input_tokens)
                          + "\n"));
}

} // namespace ModelerAi::provider
