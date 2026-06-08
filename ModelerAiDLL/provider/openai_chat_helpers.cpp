// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "provider/openai_chat_helpers.h"
#include "agent/message.h"
#include "bridge/log.h"

namespace ModelerAi::provider::openai_chat {

namespace {

// Plain-text shape — used when there are no tool_calls to encode.
nlohmann::json plainMessageToJson(const agent::Message& m)
{
    nlohmann::json j;
    switch (m.role) {
        case agent::Role::User:      j["role"] = "user"; break;
        case agent::Role::Assistant: j["role"] = "assistant"; break;
        case agent::Role::Tool:      j["role"] = "tool"; break;
        case agent::Role::System:    j["role"] = "system"; break;
    }
    j["content"] = m.text;
    return j;
}

} // namespace

void appendMessage(nlohmann::json& out, const agent::Message& m)
{
    if (m.role == agent::Role::Assistant && !m.tool_calls.empty()) {
        nlohmann::json a;
        a["role"]    = "assistant";
        a["content"] = m.text;
        nlohmann::json calls = nlohmann::json::array();
        for (const auto& c : m.tool_calls) {
            nlohmann::json fn;
            fn["name"]      = c.name;
            fn["arguments"] = c.args_json.empty() ? std::string("{}") : c.args_json;
            calls.push_back({
                {"id",       c.call_id},
                {"type",     "function"},
                {"function", std::move(fn)}
            });
        }
        a["tool_calls"] = std::move(calls);
        out.push_back(std::move(a));
        return;
    }
    if (m.role == agent::Role::Tool) {
        // OpenAI: one role="tool" message per call (in the order their ids
        // appeared in the prior assistant message).
        for (const auto& c : m.tool_calls) {
            nlohmann::json t;
            t["role"]         = "tool";
            t["tool_call_id"] = c.call_id;
            t["content"]      = c.result_json.empty() ? "{}" : c.result_json;
            out.push_back(std::move(t));
        }
        return;
    }
    out.push_back(plainMessageToJson(m));
}

void mergeUsage(UsageSnapshot& s, const nlohmann::json& u)
{
    if (!u.is_object()) return;
    if (u.contains("prompt_tokens") && u["prompt_tokens"].is_number_integer()) {
        s.input_tokens = u["prompt_tokens"].get<int>();
    }
    if (u.contains("completion_tokens") && u["completion_tokens"].is_number_integer()) {
        s.output_tokens = u["completion_tokens"].get<int>();
    }
    // OpenAI: usage.prompt_tokens_details.cached_tokens
    if (u.contains("prompt_tokens_details") && u["prompt_tokens_details"].is_object()) {
        const auto& d = u["prompt_tokens_details"];
        if (d.contains("cached_tokens") && d["cached_tokens"].is_number_integer()) {
            s.cache_read_input_tokens = d["cached_tokens"].get<int>();
        }
    }
}

double estimateCost(const UsageSnapshot& u, const Prices& p)
{
    constexpr double kMtok = 1'000'000.0;
    return (u.input_tokens            * p.input_usd_per_mtok      / kMtok)
         + (u.output_tokens           * p.output_usd_per_mtok     / kMtok)
         + (u.cache_read_input_tokens * p.cache_read_usd_per_mtok / kMtok);
}

nlohmann::json buildRequest(
    const std::string&                 model_id,
    int                                max_tokens,
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    const std::string&                 reasoning_effort)
{
    nlohmann::json req;
    req["model"]      = model_id;
    req["max_tokens"] = max_tokens;
    req["stream"]     = true;
    // Newer OpenAI supports "stream_options": {"include_usage": true} for a
    // final usage chunk. Harmless on providers that ignore it.
    req["stream_options"] = { {"include_usage", true} };

    if (!reasoning_effort.empty()) {
        req["reasoning_effort"] = reasoning_effort;
    }

    // OpenAI puts the system prompt inline as the first message rather than
    // in a dedicated `system` field. Concatenate stable + variable prefix
    // into one system message.
    auto& msgs = req["messages"] = nlohmann::json::array();
    std::string systemText;
    if (!conv.system_prompt_stable.empty())   systemText += conv.system_prompt_stable;
    if (!conv.system_prompt_variable.empty()) {
        if (!systemText.empty()) systemText += "\n\n";
        systemText += conv.system_prompt_variable;
    }
    if (!systemText.empty()) {
        msgs.push_back({{"role", "system"}, {"content", systemText}});
    }
    for (const auto& m : conv.messages) {
        if (m.role == agent::Role::System) continue;
        appendMessage(msgs, m);
    }

    // tools[] — OpenAI shape is [{type: "function", function: {name,
    // description, parameters}}, ...]. Empty vector means "no tools" and the
    // field is omitted (some providers reject empty arrays).
    if (!tools.empty()) {
        nlohmann::json toolsArr = nlohmann::json::array();
        for (const auto& td : tools) {
            nlohmann::json fn;
            fn["name"]        = td.name;
            fn["description"] = td.description;
            try {
                fn["parameters"] = nlohmann::json::parse(
                    td.params_schema_json.empty()
                        ? std::string(R"({"type":"object","properties":{}})")
                        : td.params_schema_json);
            } catch (...) {
                bridge::consolePrint(("[ModelerAI] WARN: tool '" + td.name
                                      + "' has invalid params_schema_json; using empty\n"));
                fn["parameters"] = {{"type","object"},{"properties", nlohmann::json::object()}};
            }
            toolsArr.push_back({
                {"type",     "function"},
                {"function", std::move(fn)}
            });
        }
        req["tools"] = std::move(toolsArr);
    }

    return req;
}

void processSseDataLine(
    const std::string&            data,
    std::map<int, ToolCallAcc>&   tool_calls_by_index,
    UsageSnapshot&                usage,
    std::string&                  message_id_out,
    bool&                         seen_done,
    const StreamCallback&         onChunk,
    const ErrorCallback&          onError)
{
    if (data == "[DONE]") { seen_done = true; return; }
    try {
        auto j = nlohmann::json::parse(data);
        if (j.contains("id") && j["id"].is_string() && message_id_out.empty()) {
            message_id_out = j["id"].get<std::string>();
        }
        if (j.contains("choices") && j["choices"].is_array() && !j["choices"].empty()) {
            const auto& choice = j["choices"][0];
            if (choice.contains("delta") && choice["delta"].is_object()) {
                const auto& d = choice["delta"];
                if (d.contains("content") && d["content"].is_string()) {
                    if (onChunk) {
                        onChunk(StreamChunk{ d["content"].get<std::string>(), false });
                    }
                }
                // Streaming tool_calls — accumulate by index.
                if (d.contains("tool_calls") && d["tool_calls"].is_array()) {
                    for (const auto& tc : d["tool_calls"]) {
                        int idx = tc.value("index", 0);
                        auto& acc = tool_calls_by_index[idx];
                        if (tc.contains("id") && tc["id"].is_string() && acc.id.empty()) {
                            acc.id = tc["id"].get<std::string>();
                        }
                        if (tc.contains("function") && tc["function"].is_object()) {
                            const auto& fn = tc["function"];
                            if (fn.contains("name") && fn["name"].is_string() && acc.name.empty()) {
                                acc.name = fn["name"].get<std::string>();
                            }
                            if (fn.contains("arguments") && fn["arguments"].is_string()) {
                                acc.arguments.append(fn["arguments"].get<std::string>());
                            }
                        }
                    }
                }
            }
        }
        if (j.contains("usage")) {
            mergeUsage(usage, j["usage"]);
        }
        if (j.contains("error") && j["error"].is_object()) {
            const auto& e = j["error"];
            std::string msg = "Provider error";
            if (e.contains("message") && e["message"].is_string()) {
                msg = e["message"].get<std::string>();
            }
            if (onError) onError(ProviderError{ "stream_error", msg, true });
        }
    } catch (...) {}
}

void emitToolCalls(
    const std::map<int, ToolCallAcc>& tool_calls_by_index,
    const ToolCallCallback&           onToolCall)
{
    if (!onToolCall) return;
    for (auto& kv : tool_calls_by_index) {
        ProviderToolCall pc;
        pc.call_id  = kv.second.id;
        pc.name     = kv.second.name;
        pc.args_json = kv.second.arguments.empty()
                       ? std::string("{}")
                       : kv.second.arguments;
        onToolCall(pc);
    }
}

} // namespace ModelerAi::provider::openai_chat
