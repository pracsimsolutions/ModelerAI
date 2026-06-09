// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/builtin_chat.h"
#include "provider/http_client.h"
#include "provider/openai_chat_helpers.h"
#include "provider/sse_parser.h"
#include "agent/message.h"
#include "bootstrap.h"
#include "bridge/log.h"
#include "keystore/keystore.h"
#include "third_party/json.h"

#include <cstdlib>
#include <map>
#include <string>
#include <vector>

namespace ModelerAi::provider {

namespace {

constexpr const char* kCompletionsPath = "/chat/completions";

std::string joinUrl(const std::string& base, const std::string& suffix)
{
    if (base.empty()) return suffix;
    if (base.back() == '/' && !suffix.empty() && suffix.front() == '/')
        return base + suffix.substr(1);
    if (base.back() != '/' && !suffix.empty() && suffix.front() != '/')
        return base + "/" + suffix;
    return base + suffix;
}

// Map our generic EffortLevel to OpenAI's reasoning_effort string for
// o-series models. Off → "" (omit field). Used only for models where
// ModelInfo.supports_thinking is true.
std::string effortToReasoning(EffortLevel e)
{
    switch (e) {
        case EffortLevel::Off:    return "";
        case EffortLevel::Low:    return "low";
        case EffortLevel::Medium: return "medium";
        case EffortLevel::High:   return "high";
    }
    return "";
}

int defaultMaxTokens(const ModelInfo& m)
{
    // Heuristic mirroring the Anthropic adapter: cheaper models get tighter
    // caps. For thinking models we let the helper add headroom — here we
    // only set the visible-reply budget.
    if (m.input_usd_per_mtok <= 0.25) return 1024;
    if (m.input_usd_per_mtok <= 3.00) return 2048;
    return 4096;
}

} // namespace

BuiltinChatCompletions::BuiltinChatCompletions(BuiltinChatConfig cfg)
    : cfg_(std::move(cfg))
    , active_model_id_(cfg_.default_model)
{
    // Defensive: if default_model wasn't set or doesn't match anything in
    // the curated list, fall back to the first model so we never sit in an
    // invalid state.
    if (active_model_id_.empty() && !cfg_.models.empty()) {
        active_model_id_ = cfg_.models.front().id;
    }
}

std::string BuiltinChatCompletions::id() const           { return cfg_.id; }
std::string BuiltinChatCompletions::display_name() const { return cfg_.display_name; }
std::vector<ModelInfo> BuiltinChatCompletions::models() const { return cfg_.models; }

namespace {

// Keystore-first, env-var fallback. Same resolution order as the Anthropic
// adapter so the security story is consistent across all built-ins.
std::string readApiKey(const BuiltinChatConfig& cfg)
{
    if (auto* ks = bootstrap::keystore()) {
        std::string s;
        if (ks->get(cfg.id, s) && !s.empty()) return s;
    }
    if (cfg.env_var.empty()) return "";
    const char* k = std::getenv(cfg.env_var.c_str());
    if (!k) return "";
    return k;
}

} // namespace

bool BuiltinChatCompletions::has_api_key() const
{
    return !readApiKey(cfg_).empty();
}

std::string BuiltinChatCompletions::active_model_id() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return active_model_id_;
}

bool BuiltinChatCompletions::set_active_model(std::string_view id)
{
    std::string target(id);
    for (const auto& m : cfg_.models) {
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

EffortLevel BuiltinChatCompletions::effort() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return effort_;
}

void BuiltinChatCompletions::set_effort(EffortLevel level)
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    // Honor effort only if the active model supports thinking.
    for (const auto& m : cfg_.models) {
        if (m.id == active_model_id_) {
            if (!m.supports_thinking && level != EffortLevel::Off) return;
            break;
        }
    }
    effort_ = level;
}

TestKeyResult BuiltinChatCompletions::test_key()
{
    TestKeyResult r;
    std::string apiKey = readApiKey(cfg_);
    if (apiKey.empty()) {
        r.error_message = cfg_.env_var + " is not set and no key in keystore.";
        return r;
    }

    std::string url = joinUrl(cfg_.base_url, kCompletionsPath);
    std::vector<HttpHeader> headers = {
        {"Authorization", "Bearer " + apiKey},
    };

    // Pick the cheapest model in the curated list for the test call.
    std::string testModel = cfg_.models.empty() ? cfg_.default_model : cfg_.models.front().id;
    for (const auto& m : cfg_.models) {
        if (m.input_usd_per_mtok > 0 && m.input_usd_per_mtok < 5.0) { testModel = m.id; break; }
    }

    nlohmann::json req;
    req["model"] = testModel;
    // Reasoning models (o1, o3, ...) require max_completion_tokens.
    bool isReasoningModel = (testModel.size() >= 2
                             && testModel[0] == 'o'
                             && testModel[1] >= '0' && testModel[1] <= '9');
    req[isReasoningModel ? "max_completion_tokens" : "max_tokens"] = 1;
    req["messages"]   = nlohmann::json::array({
        nlohmann::json{{"role", "user"}, {"content", "."}}
    });
    std::string body = req.dump();

    int status = streamPostUrl(url, headers, body, [](std::string_view) {});
    r.http_status = status;
    if (status == 0) {
        r.error_message = "Network error contacting " + cfg_.base_url + ".";
        return r;
    }
    if (status >= 200 && status < 300) {
        r.ok = true;
        return r;
    }
    if (status == 401 || status == 403) r.error_message = "Invalid API key.";
    else if (status == 404)             r.error_message = "Endpoint not found — base URL may be stale.";
    else if (status == 429)             r.error_message = "Rate limited or cap reached.";
    else                                r.error_message = "HTTP " + std::to_string(status);
    return r;
}

void BuiltinChatCompletions::stream_turn(
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    StreamCallback                     onChunk,
    ToolCallCallback                   onToolCall,
    DoneCallback                       onDone,
    ErrorCallback                      onError,
    CancelToken                        cancel)
{
    // Snapshot active state under the lock so a mid-turn switch doesn't
    // partway change behavior.
    std::string modelId;
    EffortLevel effortLevel;
    {
        std::lock_guard<std::mutex> lk(stateMutex_);
        modelId     = active_model_id_;
        effortLevel = effort_;
    }
    // Find the ModelInfo for pricing + capability flags.
    ModelInfo modelInfo{};
    bool found = false;
    for (const auto& m : cfg_.models) {
        if (m.id == modelId) { modelInfo = m; found = true; break; }
    }
    if (!found) {
        if (onError) {
            onError(ProviderError{ "unknown_model",
                "Active model '" + modelId + "' not in " + cfg_.display_name + "'s catalog.", false });
        }
        return;
    }

    bridge::consolePrint(("[ModelerAI] " + cfg_.display_name
                          + "::stream_turn model=" + modelId + "\n"));

    std::string apiKey = readApiKey(cfg_);
    if (apiKey.empty()) {
        if (onError) {
            onError(ProviderError{
                "no_api_key",
                "No API key set for " + cfg_.display_name + ". Paste one in Settings → Providers, "
                "or set " + cfg_.env_var + " in your environment and restart FlexSim.",
                false });
        }
        return;
    }

    std::string url = joinUrl(cfg_.base_url, kCompletionsPath);
    std::vector<HttpHeader> headers = {
        {"Authorization", "Bearer " + apiKey},
    };

    // Only emit reasoning_effort for models that support thinking AND when
    // effort is non-Off. Other models would reject the field on some
    // providers (e.g. older GPT-4o).
    std::string reasoningEffort;
    if (modelInfo.supports_thinking && effortLevel != EffortLevel::Off) {
        reasoningEffort = effortToReasoning(effortLevel);
    }

    nlohmann::json req = openai_chat::buildRequest(
        modelId, defaultMaxTokens(modelInfo), conv, tools, reasoningEffort);
    std::string body = req.dump();

    openai_chat::UsageSnapshot usage;
    std::string messageId;
    bool seenDone = false;
    std::map<int, openai_chat::ToolCallAcc> toolCallsByIndex;

    SseParser parser([&](const SseEvent& ev) {
        openai_chat::processSseDataLine(
            ev.data, toolCallsByIndex, usage, messageId, seenDone,
            onChunk, onError);
        if (seenDone) {
            openai_chat::emitToolCalls(toolCallsByIndex, onToolCall);
            if (onDone) {
                openai_chat::Prices prices{
                    modelInfo.input_usd_per_mtok,
                    modelInfo.output_usd_per_mtok,
                    modelInfo.cache_read_usd_per_mtok
                };
                TurnResult r;
                r.message_id              = messageId;
                r.input_tokens            = usage.input_tokens;
                r.output_tokens           = usage.output_tokens;
                r.cache_read_input_tokens = usage.cache_read_input_tokens;
                r.cost_usd                = openai_chat::estimateCost(usage, prices);
                r.model_id                = modelId;
                onDone(r);
            }
            seenDone = false;
        }
    });

    bridge::consolePrint(("[ModelerAI] POST " + url + " (streaming)\n"));
    // Capture raw body alongside the SSE parser so 4xx/5xx error JSON
    // (which isn't SSE and gets silently dropped by the parser) is
    // visible in the error message + system console.
    std::string rawBody;
    rawBody.reserve(2048);
    int status = streamPostUrl(url, headers, body, [&](std::string_view chunk) {
        rawBody.append(chunk.data(), chunk.size());
        parser.feed(chunk);
    }, cancel);
    parser.flush();
    bridge::consolePrint(("[ModelerAI] HTTP status=" + std::to_string(status) + "\n"));

    if (status == -1) {
        if (onError) onError(ProviderError{ "cancelled",
            "Turn cancelled by user.", false });
        return;
    }
    if (status == 0) {
        if (onError) onError(ProviderError{ "network",
            "Network error reaching " + cfg_.display_name + ".", true });
        return;
    }
    if (status < 200 || status >= 300) {
        std::string detail = rawBody;
        if (detail.size() > 800) detail = detail.substr(0, 800) + "…";
        bridge::consolePrint("[" + cfg_.display_name + "] HTTP "
                             + std::to_string(status) + " body: " + rawBody + "\n");
        if (onError) {
            std::string msg;
            if (status == 401 || status == 403) {
                msg = cfg_.display_name + " rejected the API key (HTTP " + std::to_string(status) + ").";
            } else if (status == 429) {
                msg = cfg_.display_name + " is rate-limiting requests. Wait a minute and try again.";
            } else {
                msg = cfg_.display_name + " returned HTTP " + std::to_string(status)
                    + (detail.empty() ? "." : ". Response:\n" + detail);
            }
            onError(ProviderError{ "http_" + std::to_string(status), msg, status == 429 || status >= 500 });
        }
        return;
    }
}

} // namespace ModelerAi::provider
