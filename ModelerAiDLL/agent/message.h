// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/message.h — canonical message + conversation types.
//
// The agent reasons about messages in this provider-neutral shape. Each
// provider adapter (anthropic.cpp, openai.cpp, gemini.cpp) translates to and
// from its own wire format internally.

#pragma once

#include <chrono>
#include <optional>
#include <string>
#include <vector>

namespace ModelerAi::agent {

enum class Role { System, User, Assistant, Tool };

// One tool invocation, captured across the lifecycle:
//   - In an assistant Message (Role::Assistant): the call as emitted by
//     the LLM. result_json is empty / ok is meaningless at this stage.
//   - In a tool Message (Role::Tool): the same record after the tool
//     ran. result_json populated; ok + error_message reflect success.
// Adapters that round-trip tool calls in history serialize both forms
// per their wire format.
struct ToolCallRecord {
    std::string call_id;
    std::string name;
    std::string args_json;     // raw JSON
    std::string result_json;   // raw JSON; empty until the tool runs
    bool        ok = false;
    std::string error_message; // populated when ok == false
};

// User-supplied attachment carried on a User message. v1 supports images
// only (Anthropic accepts image/jpeg, image/png, image/gif, image/webp).
// `data_base64` is the raw base64 payload — NO "data:..." prefix.
struct Attachment {
    std::string media_type;   // "image/png", "image/jpeg", "image/gif", "image/webp"
    std::string data_base64;  // base64 encoded bytes
    std::string name;         // display-only (filename or "clipboard-image-N.png")
};

struct Message {
    Role                                       role;
    std::string                                text;          // primary content
    std::vector<Attachment>                    attachments;   // user images (User role only)
    std::vector<ToolCallRecord>                tool_calls;    // assistant-emitted, may be empty
    std::optional<std::string>                 tool_call_id;  // set when role == Tool
    std::chrono::system_clock::time_point      ts;
};

struct Conversation {
    std::string            id;                       // ULID; persists with the model
    std::string            model_path;               // FlexSim .fsm path this conversation is scoped to
    std::string            system_prompt_stable;     // persona + rules + user rules + KB — cacheable
    std::string            system_prompt_variable;   // condensed model state — changes per turn
    std::vector<Message>   messages;                 // ordered, oldest first
};

// ---------------------------------------------------------------------------
// Tool definitions surfaced to the provider. The agent assembles these from
// the tool registry; per-provider adapters translate to the wire format.
// ---------------------------------------------------------------------------
struct ToolDef {
    std::string name;
    std::string description;
    std::string params_schema_json;  // JSON Schema string
};

} // namespace ModelerAi::agent
