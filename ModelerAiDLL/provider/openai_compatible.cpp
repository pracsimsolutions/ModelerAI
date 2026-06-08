// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/openai_compatible.h"
#include "provider/http_client.h"
#include "provider/openai_chat_helpers.h"
#include "provider/sse_parser.h"
#include "agent/message.h"
#include "bootstrap.h"
#include "bridge/log.h"
#include "keystore/keystore.h"
#include "third_party/json.h"

#include <map>
#include <string>
#include <vector>

namespace ModelerAi::provider {

namespace {

// OpenAI Chat Completions endpoint suffix. base_url is expected to include
// "/v1" so we just append the operation.
constexpr const char* kCompletionsPath = "/chat/completions";

// Resolve the API key for a custom provider. Keystore wins; legacy plaintext
// in the config is the fallback for in-flight migrations.
std::string resolveKey(const CustomProviderConfig& cfg)
{
    if (auto* ks = bootstrap::keystore()) {
        std::string s;
        if (ks->get(cfg.id, s) && !s.empty()) return s;
    }
    return cfg.api_key;
}

std::string joinUrl(const std::string& base, const std::string& suffix)
{
    if (base.empty()) return suffix;
    if (base.back() == '/' && !suffix.empty() && suffix.front() == '/')
        return base + suffix.substr(1);
    if (base.back() != '/' && !suffix.empty() && suffix.front() != '/')
        return base + "/" + suffix;
    return base + suffix;
}

} // namespace

OpenAiCompatible::OpenAiCompatible(CustomProviderConfig cfg)
    : cfg_(std::move(cfg))
{
}

std::string OpenAiCompatible::id() const          { return cfg_.id; }
std::string OpenAiCompatible::display_name() const { return cfg_.display_name; }

std::vector<ModelInfo> OpenAiCompatible::models() const
{
    ModelInfo m;
    m.id                       = cfg_.model_id;
    m.display_name             = cfg_.model_display.empty() ? cfg_.model_id : cfg_.model_display;
    m.context_tokens           = cfg_.context_tokens;
    m.supports_tools           = cfg_.supports_tools;
    m.supports_vision          = cfg_.supports_vision;
    m.supports_thinking        = false;  // OpenAI-compat doesn't expose this knob uniformly
    m.input_usd_per_mtok       = cfg_.input_usd_per_mtok;
    m.output_usd_per_mtok      = cfg_.output_usd_per_mtok;
    m.cache_write_usd_per_mtok = cfg_.cache_write_usd_per_mtok;
    m.cache_read_usd_per_mtok  = cfg_.cache_read_usd_per_mtok;
    return { m };
}

bool OpenAiCompatible::has_api_key() const
{
    // For local providers (Ollama, LM Studio) the api_key is often a
    // placeholder like "ollama" — the server ignores it but our adapter
    // still needs SOME non-empty value to flow through the Bearer header.
    return !resolveKey(cfg_).empty();
}

std::string OpenAiCompatible::active_model_id() const
{
    return cfg_.model_id;
}

bool OpenAiCompatible::set_active_model(std::string_view id)
{
    // One model per OpenAiCompatible instance — add a separate custom
    // provider entry to use a different model.
    return std::string(id) == cfg_.model_id;
}

EffortLevel OpenAiCompatible::effort() const { return EffortLevel::Off; }
void OpenAiCompatible::set_effort(EffortLevel) { /* no-op — not supported */ }

TestKeyResult OpenAiCompatible::test_key()
{
    TestKeyResult r;
    std::string url = joinUrl(cfg_.base_url, kCompletionsPath);

    std::string apiKey = resolveKey(cfg_);
    std::vector<HttpHeader> headers;
    if (!apiKey.empty()) {
        headers.push_back({"Authorization", "Bearer " + apiKey});
    }

    nlohmann::json req;
    req["model"]      = cfg_.model_id;
    req["max_tokens"] = 1;
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
    else if (status == 404)             r.error_message = "Endpoint not found — verify base URL.";
    else if (status == 429)             r.error_message = "Rate limited or cap reached.";
    else                                r.error_message = "HTTP " + std::to_string(status);
    return r;
}

void OpenAiCompatible::stream_turn(
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    StreamCallback                     onChunk,
    ToolCallCallback                   onToolCall,
    DoneCallback                       onDone,
    ErrorCallback                      onError,
    CancelToken                        cancel)
{
    CustomProviderConfig localCfg;
    {
        std::lock_guard<std::mutex> lk(stateMutex_);
        localCfg = cfg_;
    }
    bridge::consolePrint(("[ModelerAI] OpenAiCompatible::stream_turn id=" + localCfg.id
                          + " model=" + localCfg.model_id + "\n"));

    if (localCfg.base_url.empty()) {
        if (onError) onError(ProviderError{ "no_base_url",
            "Custom provider has no base URL configured.", false });
        return;
    }

    std::string url = joinUrl(localCfg.base_url, kCompletionsPath);

    std::string apiKey = resolveKey(localCfg);
    std::vector<HttpHeader> headers;
    if (!apiKey.empty()) {
        headers.push_back({"Authorization", "Bearer " + apiKey});
    }

    nlohmann::json req = openai_chat::buildRequest(
        localCfg.model_id, 2048, conv, tools, /*reasoning_effort=*/"");
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
                    localCfg.input_usd_per_mtok,
                    localCfg.output_usd_per_mtok,
                    localCfg.cache_read_usd_per_mtok
                };
                TurnResult r;
                r.message_id              = messageId;
                r.input_tokens            = usage.input_tokens;
                r.output_tokens           = usage.output_tokens;
                r.cache_read_input_tokens = usage.cache_read_input_tokens;
                r.cost_usd                = openai_chat::estimateCost(usage, prices);
                r.model_id                = localCfg.model_id;
                onDone(r);
            }
            // Reset so a stray subsequent [DONE] doesn't double-fire.
            seenDone = false;
        }
    });

    bridge::consolePrint(("[ModelerAI] POST " + url + " (streaming)\n"));
    int status = streamPostUrl(url, headers, body, [&](std::string_view chunk) {
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
            "Network error reaching " + localCfg.base_url + ".", true });
        return;
    }
    if (status < 200 || status >= 300) {
        if (onError) {
            std::string msg = localCfg.display_name + " returned HTTP " + std::to_string(status);
            onError(ProviderError{ "http_" + std::to_string(status), msg, status >= 500 });
        }
        return;
    }
}

} // namespace ModelerAi::provider
