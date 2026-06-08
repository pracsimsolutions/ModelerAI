// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/protocol.h — canonical message-type catalog for the JS ↔ C++ bridge.
//
// Every envelope crossing the boundary has this shape (compact JSON, no
// whitespace, UTF-8):
//
//     {"t":"user_message","id":"01HZ...","p":{...}}
//
// `t`  — Type from the BridgeMsgType enum below
// `id` — ULID-style correlation ID
// `p`  — Per-type payload (may be omitted)
//
// Schemas for each payload live as comments next to the enum value; the
// canonical source is this header, not the README.

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::bridge {

// ---------------------------------------------------------------------------
// Direction tags. Pure documentation — the C++ side doesn't enforce direction
// at runtime, but every handler should be on the correct side.
// ---------------------------------------------------------------------------
enum class Direction { JsToDll, DllToJs };

// ---------------------------------------------------------------------------
// Message types. Add new types here AND in protocol.cpp's name table when you
// extend the protocol.
// ---------------------------------------------------------------------------
enum class BridgeMsgType {
    // ---- JS → DLL ----
    UserMessage,        // p: { text: string }
    CancelTurn,         // p: { turn_id: string }
    ProviderSwitch,     // p: { provider: string, model: string }
    SaveApiKey,         // p: { provider: string, key: string }    // key never echoed back
    ForgetApiKey,       // p: { provider: string }
    LoadHistory,        // p: { model_path?: string }
    ClearHistory,       // p: { scope: "session" | "all" }
    GetProviderInfo,    // p: {}                                   // returns providers + models
    ViewerReady,        // p: {}                — viewer signals "I'm mounted, push initial state"
    ModeChange,         // p: { mode: "ask"|"plan"|"apply" }
    TestProvider,       // p: { provider: string }                 — validate the key
    EffortChange,       // p: { effort: "off"|"low"|"medium"|"high" }
    ListSessions,       // p: {}                — list every on-disk session for the picker
    DeleteSession,      // p: { key: string }   — delete one on-disk session file by key
    DeleteAllSessions,  // p: {}                — wipe every on-disk session ("clear session memory")
    AddCustomProvider,  // p: { display_name, base_url, api_key, model_id, ... }
    RemoveCustomProvider, // p: { id }
    ToolApprovalResponse,    // p: { interaction_id, decision: "approve"|"deny"|"always_allow" }
    AskUserQuestionResponse, // p: { interaction_id, answer: string, ... }
    SetSetting,              // p: { key, value, scope: "global"|"project" }
    RequestSettings,         // p: {} — re-emit current effective + meta
    CompleteOnboarding,      // p: {} — user finished the first-run wizard; writes onboarding_completed_version + onboarding_completed_at_ms to global settings
    ResetSettings,           // p: { scope: "global" } — clear user-pref keys back to compiled defaults; preserve onboarding completion + acks
    ExportConversation,      // p: {} — write current in-memory conversation to a Markdown file under %APPDATA%/PRACSIM/ModelerAI/exports/; DLL replies via state_update { key: "conversation_export", value: { path, ok, ... } }

    // ---- DLL → JS ----
    AssistantChunk,     // p: { turn_id: string, text: string }
    AssistantDone,      // p: { turn_id: string, message_id: string }
    ToolCallStart,      // p: { turn_id: string, call_id: string, name: string, args_summary: string }
    ToolCallDone,       // p: { turn_id: string, call_id: string, ok: bool, summary: string }
    StateUpdate,        // p: { key: string, value: any }
    Error,              // p: { code: string, message: string, retriable: bool }

    // ---- Sentinel ----
    Unknown
};

// String <-> enum conversion. Implementation lives in bridge.cpp (or its own
// .cpp file when the implementation arrives).
std::string_view typeName(BridgeMsgType t);
BridgeMsgType    parseType(std::string_view name);

} // namespace ModelerAi::bridge
