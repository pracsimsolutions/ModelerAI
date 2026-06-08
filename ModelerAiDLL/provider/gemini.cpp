// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/gemini.h"
#include "provider/http_client.h"
#include "provider/sse_parser.h"
#include "agent/message.h"
#include "bootstrap.h"
#include "bridge/log.h"
#include "keystore/keystore.h"
#include "third_party/json.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace ModelerAi::provider {

namespace {

constexpr const char* kDefaultModel = "gemini-2.5-flash";
constexpr const char* kBaseUrl      = "https://generativelanguage.googleapis.com/v1beta";

// Gemini-side thinking budget. 0 = thinking off. Matches the Anthropic
// mapping in spirit: Low/Medium/High get the same token budgets so the
// user's effort slider behaves the same way across providers.
int thinkingBudgetTokens(EffortLevel e)
{
    switch (e) {
        case EffortLevel::Off:    return 0;
        case EffortLevel::Low:    return 4096;
        case EffortLevel::Medium: return 16384;
        case EffortLevel::High:   return 32768;
    }
    return 0;
}

int defaultMaxOutputTokens(const std::string& modelId)
{
    if (modelId.find("flash-lite") != std::string::npos) return 1024;
    if (modelId.find("flash")      != std::string::npos) return 2048;
    return 4096;  // pro
}

// Key resolution order:
//   1. DPAPI keystore entry for "gemini".
//   2. GEMINI_API_KEY environment variable (Google AI Studio's docs).
//   3. GOOGLE_API_KEY environment variable (some Google libraries use this).
std::string readApiKey()
{
    if (auto* ks = bootstrap::keystore()) {
        std::string s;
        if (ks->get("gemini", s) && !s.empty()) return s;
    }
    if (const char* k = std::getenv("GEMINI_API_KEY")) {
        if (k[0] != '\0') return k;
    }
    if (const char* k = std::getenv("GOOGLE_API_KEY")) {
        if (k[0] != '\0') return k;
    }
    return "";
}

// Convert one ToolCallRecord's result_json into the {"response": {...}} JSON
// object Gemini expects. The Gemini API requires `response` to be an object;
// if our stored result is itself an object we use it directly, otherwise we
// wrap it as { "result": <value> } so the call still succeeds.
nlohmann::json makeFunctionResponseBody(const agent::ToolCallRecord& c)
{
    nlohmann::json body;
    if (!c.result_json.empty()) {
        try {
            auto parsed = nlohmann::json::parse(c.result_json);
            if (parsed.is_object()) return parsed;
            body["result"] = std::move(parsed);
            return body;
        } catch (...) {
            body["result"] = c.result_json;  // store the raw string
            return body;
        }
    }
    body["result"] = nlohmann::json::object();
    return body;
}

// Translate one agent::Message into the Gemini `contents` entry shape.
// Returns nullopt for messages that don't belong in the contents array
// (System — handled separately via systemInstruction).
nlohmann::json messageToContent(const agent::Message& m)
{
    nlohmann::json c;
    auto parts = nlohmann::json::array();

    if (m.role == agent::Role::Tool) {
        // Tool results replay as a user-role message with one
        // functionResponse part per tool that ran.
        c["role"] = "user";
        for (const auto& tc : m.tool_calls) {
            nlohmann::json fr;
            fr["name"]     = tc.name;
            fr["response"] = makeFunctionResponseBody(tc);
            parts.push_back({{"functionResponse", std::move(fr)}});
        }
        if (parts.empty()) parts.push_back({{"text", ""}});
        c["parts"] = std::move(parts);
        return c;
    }

    if (m.role == agent::Role::Assistant) {
        c["role"] = "model";
        if (!m.text.empty()) {
            parts.push_back({{"text", m.text}});
        }
        for (const auto& tc : m.tool_calls) {
            nlohmann::json fc;
            fc["name"] = tc.name;
            try {
                fc["args"] = tc.args_json.empty()
                    ? nlohmann::json::object()
                    : nlohmann::json::parse(tc.args_json);
            } catch (...) {
                fc["args"] = nlohmann::json::object();
            }
            parts.push_back({{"functionCall", std::move(fc)}});
        }
        if (parts.empty()) parts.push_back({{"text", ""}});
        c["parts"] = std::move(parts);
        return c;
    }

    // User message — text only (attachments deferred per design spec).
    c["role"] = "user";
    parts.push_back({{"text", m.text}});
    c["parts"] = std::move(parts);
    return c;
}

struct UsageSnapshot {
    int prompt_tokens     = 0;
    int candidates_tokens = 0;
    int cached_tokens     = 0;
    int thoughts_tokens   = 0;
};

void mergeUsage(UsageSnapshot& s, const nlohmann::json& u)
{
    if (!u.is_object()) return;
    if (u.contains("promptTokenCount") && u["promptTokenCount"].is_number_integer()) {
        s.prompt_tokens = u["promptTokenCount"].get<int>();
    }
    if (u.contains("candidatesTokenCount") && u["candidatesTokenCount"].is_number_integer()) {
        s.candidates_tokens = u["candidatesTokenCount"].get<int>();
    }
    if (u.contains("cachedContentTokenCount") && u["cachedContentTokenCount"].is_number_integer()) {
        s.cached_tokens = u["cachedContentTokenCount"].get<int>();
    }
    if (u.contains("thoughtsTokenCount") && u["thoughtsTokenCount"].is_number_integer()) {
        s.thoughts_tokens = u["thoughtsTokenCount"].get<int>();
    }
}

double estimateCost(const UsageSnapshot& u, const ModelInfo& m)
{
    constexpr double kMtok = 1'000'000.0;
    // Thoughts tokens are billed at the output rate on Gemini 2.5.
    int output = u.candidates_tokens + u.thoughts_tokens;
    return (u.prompt_tokens * m.input_usd_per_mtok       / kMtok)
         + (output          * m.output_usd_per_mtok      / kMtok)
         + (u.cached_tokens  * m.cache_read_usd_per_mtok / kMtok);
}

} // namespace

Gemini::Gemini() : active_model_id_(kDefaultModel) {}

std::string Gemini::id() const           { return "gemini"; }
std::string Gemini::display_name() const { return "Google Gemini"; }

std::vector<ModelInfo> Gemini::models() const
{
    // Pricing snapshot as of 2026-05-27. Re-verify against
    // ai.google.dev/pricing before relying on cost numbers for billing.
    return {
        ModelInfo{
            /*id*/                       "gemini-2.0-flash",
            /*display_name*/             "Gemini 2.0 Flash",
            /*context_tokens*/           1000000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            /*supports_thinking*/        false,
            /*accepts_temperature*/      true,
            /*input_usd_per_mtok*/       0.10,
            /*output_usd_per_mtok*/      0.40,
            /*cache_write_usd_per_mtok*/ 0.00,
            /*cache_read_usd_per_mtok*/  0.025,
        },
        ModelInfo{
            /*id*/                       "gemini-2.5-flash",
            /*display_name*/             "Gemini 2.5 Flash",
            /*context_tokens*/           1000000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            /*supports_thinking*/        true,
            /*accepts_temperature*/      true,
            /*input_usd_per_mtok*/       0.30,
            /*output_usd_per_mtok*/      2.50,
            /*cache_write_usd_per_mtok*/ 0.00,
            /*cache_read_usd_per_mtok*/  0.075,
        },
        ModelInfo{
            /*id*/                       "gemini-2.5-pro",
            /*display_name*/             "Gemini 2.5 Pro",
            /*context_tokens*/           2000000,
            /*supports_tools*/           true,
            /*supports_vision*/          true,
            /*supports_thinking*/        true,
            /*accepts_temperature*/      true,
            /*input_usd_per_mtok*/       1.25,
            /*output_usd_per_mtok*/      10.00,
            /*cache_write_usd_per_mtok*/ 0.00,
            /*cache_read_usd_per_mtok*/  0.31,
        },
    };
}

bool Gemini::has_api_key() const
{
    return !readApiKey().empty();
}

std::string Gemini::active_model_id() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return active_model_id_;
}

bool Gemini::set_active_model(std::string_view id)
{
    std::string target(id);
    for (const auto& m : models()) {
        if (m.id == target) {
            std::lock_guard<std::mutex> lk(stateMutex_);
            active_model_id_ = target;
            if (!m.supports_thinking && effort_ != EffortLevel::Off) {
                effort_ = EffortLevel::Off;
            }
            return true;
        }
    }
    return false;
}

EffortLevel Gemini::effort() const
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    return effort_;
}

void Gemini::set_effort(EffortLevel level)
{
    std::lock_guard<std::mutex> lk(stateMutex_);
    for (const auto& m : models()) {
        if (m.id == active_model_id_) {
            if (!m.supports_thinking && level != EffortLevel::Off) return;
            break;
        }
    }
    effort_ = level;
}

TestKeyResult Gemini::test_key()
{
    TestKeyResult r;
    std::string apiKey = readApiKey();
    if (apiKey.empty()) {
        r.error_message = "GEMINI_API_KEY is not set and no key in keystore.";
        return r;
    }

    // 1-token call against the cheapest model.
    std::string url = std::string(kBaseUrl) + "/models/gemini-2.0-flash:generateContent";
    std::vector<HttpHeader> headers = {
        {"x-goog-api-key", apiKey},
    };

    nlohmann::json req;
    req["contents"] = nlohmann::json::array({
        {{"role", "user"}, {"parts", nlohmann::json::array({{{"text", "."}}})}}
    });
    req["generationConfig"] = {{"maxOutputTokens", 1}};
    std::string body = req.dump();

    int status = streamPostUrl(url, headers, body, [](std::string_view) {});
    r.http_status = status;
    if (status == 0) {
        r.error_message = "Network error contacting Gemini.";
        return r;
    }
    if (status >= 200 && status < 300) {
        r.ok = true;
        return r;
    }
    if (status == 401 || status == 403) r.error_message = "Invalid API key.";
    else if (status == 404)             r.error_message = "Endpoint not found — base URL may be stale.";
    else if (status == 429)             r.error_message = "Rate limited.";
    else                                r.error_message = "HTTP " + std::to_string(status);
    return r;
}

void Gemini::stream_turn(
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    StreamCallback                     onChunk,
    ToolCallCallback                   onToolCall,
    DoneCallback                       onDone,
    ErrorCallback                      onError,
    CancelToken                        cancel)
{
    std::string modelId;
    EffortLevel effortLevel;
    {
        std::lock_guard<std::mutex> lk(stateMutex_);
        modelId     = active_model_id_;
        effortLevel = effort_;
    }
    ModelInfo modelInfo{};
    bool found = false;
    for (const auto& m : models()) {
        if (m.id == modelId) { modelInfo = m; found = true; break; }
    }
    if (!found) {
        if (onError) {
            onError(ProviderError{ "unknown_model",
                "Active model '" + modelId + "' not in Gemini catalog.", false });
        }
        return;
    }

    bridge::consolePrint(("[ModelerAI] Gemini::stream_turn model=" + modelId + "\n"));

    std::string apiKey = readApiKey();
    if (apiKey.empty()) {
        if (onError) {
            onError(ProviderError{
                "no_api_key",
                "No API key set for Google Gemini. Paste one in Settings → Providers, "
                "or set GEMINI_API_KEY in your environment and restart FlexSim.",
                false });
        }
        return;
    }

    std::string url = std::string(kBaseUrl) + "/models/" + modelId
                    + ":streamGenerateContent?alt=sse";
    std::vector<HttpHeader> headers = {
        {"x-goog-api-key", apiKey},
        {"accept",         "text/event-stream"},
    };

    // ----- Build request body -----
    nlohmann::json req;

    // systemInstruction (top-level, NOT in messages).
    std::string systemText;
    if (!conv.system_prompt_stable.empty())   systemText  = conv.system_prompt_stable;
    if (!conv.system_prompt_variable.empty()) {
        if (!systemText.empty()) systemText += "\n\n";
        systemText += conv.system_prompt_variable;
    }
    if (!systemText.empty()) {
        req["systemInstruction"] = {
            {"parts", nlohmann::json::array({{{"text", systemText}}})}
        };
    }

    // contents
    auto& contents = req["contents"] = nlohmann::json::array();
    for (const auto& m : conv.messages) {
        if (m.role == agent::Role::System) continue;
        contents.push_back(messageToContent(m));
    }

    // tools
    if (!tools.empty()) {
        nlohmann::json decls = nlohmann::json::array();
        for (const auto& td : tools) {
            nlohmann::json decl;
            decl["name"]        = td.name;
            decl["description"] = td.description;
            try {
                decl["parameters"] = nlohmann::json::parse(
                    td.params_schema_json.empty()
                        ? std::string(R"({"type":"object","properties":{}})")
                        : td.params_schema_json);
            } catch (...) {
                bridge::consolePrint(("[ModelerAI] WARN: tool '" + td.name
                                      + "' has invalid params_schema_json; using empty\n"));
                decl["parameters"] = {{"type","object"},{"properties", nlohmann::json::object()}};
            }
            decls.push_back(std::move(decl));
        }
        req["tools"] = nlohmann::json::array({{{"functionDeclarations", std::move(decls)}}});
    }

    // generationConfig
    nlohmann::json genConfig;
    genConfig["maxOutputTokens"] = defaultMaxOutputTokens(modelId);
    int budget = thinkingBudgetTokens(effortLevel);
    if (budget > 0 && modelInfo.supports_thinking) {
        genConfig["thinkingConfig"] = {{"thinkingBudget", budget}};
        bridge::consolePrint(("[ModelerAI] Gemini thinking budget=" + std::to_string(budget) + "\n"));
    } else if (modelInfo.supports_thinking) {
        // Explicitly disable thinking on 2.5 models when the user hasn't
        // opted in — saves output tokens vs. Gemini's dynamic default.
        genConfig["thinkingConfig"] = {{"thinkingBudget", 0}};
    }
    req["generationConfig"] = std::move(genConfig);

    std::string body = req.dump();

    // ----- Stream parsing -----
    UsageSnapshot usage;
    std::string messageId;  // Gemini doesn't give us a stable id mid-stream; leave empty
    bool sawFinish = false;
    std::vector<ProviderToolCall> pendingToolCalls;

    SseParser parser([&](const SseEvent& ev) {
        if (ev.data.empty()) return;
        try {
            auto j = nlohmann::json::parse(ev.data);

            if (j.contains("usageMetadata")) {
                mergeUsage(usage, j["usageMetadata"]);
            }

            if (j.contains("candidates") && j["candidates"].is_array() && !j["candidates"].empty()) {
                const auto& cand = j["candidates"][0];
                if (cand.contains("content") && cand["content"].is_object()) {
                    const auto& content = cand["content"];
                    if (content.contains("parts") && content["parts"].is_array()) {
                        for (const auto& part : content["parts"]) {
                            if (part.contains("text") && part["text"].is_string()) {
                                if (onChunk) {
                                    onChunk(StreamChunk{ part["text"].get<std::string>(), false });
                                }
                            }
                            else if (part.contains("functionCall") && part["functionCall"].is_object()) {
                                const auto& fc = part["functionCall"];
                                ProviderToolCall pc;
                                if (fc.contains("name") && fc["name"].is_string()) {
                                    pc.name = fc["name"].get<std::string>();
                                }
                                // Gemini may attach an "id" for parallel-call
                                // disambiguation; honor it when present.
                                if (fc.contains("id") && fc["id"].is_string()) {
                                    pc.call_id = fc["id"].get<std::string>();
                                } else {
                                    // Synthesize a stable id from the name + count
                                    // so the agent's tool-result message can find it.
                                    pc.call_id = "gemini_call_" + pc.name + "_"
                                               + std::to_string(pendingToolCalls.size());
                                }
                                if (fc.contains("args") && fc["args"].is_object()) {
                                    pc.args_json = fc["args"].dump();
                                } else {
                                    pc.args_json = "{}";
                                }
                                pendingToolCalls.push_back(std::move(pc));
                            }
                        }
                    }
                }
                if (cand.contains("finishReason") && cand["finishReason"].is_string()) {
                    sawFinish = true;
                }
            }
        } catch (...) {
            // Malformed SSE chunk — log once, keep streaming.
            bridge::consolePrint("[ModelerAI] WARN: Gemini SSE parse failure (chunk skipped)\n");
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
            "Network error reaching Google Gemini.", true });
        return;
    }
    if (status < 200 || status >= 300) {
        if (onError) {
            std::string msg;
            if (status == 401 || status == 403) {
                msg = "Gemini rejected the API key (HTTP " + std::to_string(status) + ").";
            } else if (status == 429) {
                msg = "Gemini is rate-limiting requests. Wait a minute and try again.";
            } else if (status == 400) {
                msg = "Gemini returned HTTP 400 — usually means a malformed request "
                      "(check tool schema or model id).";
            } else {
                msg = "Gemini returned HTTP " + std::to_string(status);
            }
            onError(ProviderError{ "http_" + std::to_string(status), msg, status == 429 || status >= 500 });
        }
        return;
    }

    // Successful stream — emit any tool calls collected, then onDone.
    if (onToolCall) {
        for (const auto& pc : pendingToolCalls) onToolCall(pc);
    }
    if (onDone) {
        TurnResult r;
        r.message_id              = messageId;
        r.input_tokens            = usage.prompt_tokens;
        r.output_tokens           = usage.candidates_tokens + usage.thoughts_tokens;
        r.cache_read_input_tokens = usage.cached_tokens;
        r.cost_usd                = estimateCost(usage, modelInfo);
        r.model_id                = modelId;
        onDone(r);
    }
    (void)sawFinish;
}

} // namespace ModelerAi::provider
