// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/openai_chat_helpers.h — shared OpenAI Chat Completions wire-format
// helpers, used by both the generic OpenAiCompatible adapter (for user-added
// custom providers) and the built-in BuiltinChatCompletions adapter (for the
// curated OpenAI + xAI Grok entries that ship in-box).
//
// The wire format is identical across all "OpenAI-compatible" providers; only
// the host/port/auth/model-list metadata differs. Keeping the JSON build and
// SSE-stream parse here lets every adapter share one implementation.

#pragma once

#include "provider/provider.h"  // for callbacks, ProviderError, StreamChunk, ProviderToolCall
#include "third_party/json.h"

#include <map>
#include <string>
#include <vector>

namespace ModelerAi::agent {
    struct Conversation;
    struct Message;
    struct ToolDef;
}

namespace ModelerAi::provider::openai_chat {

// --------------------------------------------------------------------------
// Per-turn accumulators and snapshots.
// --------------------------------------------------------------------------

// Per-tool-call accumulating state for streamed OpenAI tool_calls. The wire
// format sends tool_call deltas by index; the first delta carries id + name,
// subsequent deltas append argument fragments.
struct ToolCallAcc {
    std::string id;
    std::string name;
    std::string arguments;  // accumulating
};

// Per-turn token totals harvested from the OpenAI SSE stream. Wire format
// varies a little across providers — OpenAI itself emits usage in a dedicated
// trailing chunk; Ollama / Groq sometimes emit it on every delta. We just
// take the latest seen values.
struct UsageSnapshot {
    int input_tokens = 0;
    int output_tokens = 0;
    int cache_read_input_tokens = 0;
};

// Per-Mtok USD pricing used by estimateCost(). Built-in adapters fill this
// from their active model's ModelInfo; the custom-provider adapter fills it
// from the user-supplied CustomProviderConfig.
struct Prices {
    double input_usd_per_mtok      = 0.0;
    double output_usd_per_mtok     = 0.0;
    double cache_read_usd_per_mtok = 0.0;
};

void   mergeUsage(UsageSnapshot& s, const nlohmann::json& usage);
double estimateCost(const UsageSnapshot& u, const Prices& p);

// --------------------------------------------------------------------------
// Request construction.
// --------------------------------------------------------------------------

// Append the OpenAI wire-format message(s) for one agent::Message to `out`.
// Tool-result messages get SPLIT into one wire message per result (OpenAI
// requires this; Anthropic doesn't). Assistant messages with tool_calls
// become a single message with the tool_calls array alongside (possibly
// empty) content text.
void appendMessage(nlohmann::json& out, const agent::Message& m);

// Build the full Chat Completions request body. Returns the JSON object
// ready to be `.dump()`ed.
//
// reasoning_effort: if non-empty, emitted as `reasoning_effort` (used for
//   OpenAI o-series models). Pass "" to omit (the default for non-thinking
//   models and providers that don't support it like xAI Grok).
nlohmann::json buildRequest(
    const std::string&                 model_id,
    int                                max_tokens,
    const agent::Conversation&         conv,
    const std::vector<agent::ToolDef>& tools,
    const std::string&                 reasoning_effort = "");

// --------------------------------------------------------------------------
// Stream parsing.
// --------------------------------------------------------------------------

// Process one SSE `data:` line payload (the caller has already stripped the
// "data: " prefix and joined multi-line continuations). [DONE] is handled
// here too — when seen, finalizeStream() should be called by the caller to
// emit any collected tool_calls and the TurnResult.
//
// Mutates the accumulator state; invokes onChunk for text deltas and onError
// for in-stream provider errors. Tool-call emission is deferred until
// finalizeStream() — see below.
void processSseDataLine(
    const std::string&            data,
    std::map<int, ToolCallAcc>&   tool_calls_by_index,
    UsageSnapshot&                usage,
    std::string&                  message_id_out,
    bool&                         seen_done,
    const StreamCallback&         onChunk,
    const ErrorCallback&          onError);

// Emit any collected tool calls in stable index order. Call this once the
// stream has terminated (either [DONE] or HTTP close).
void emitToolCalls(
    const std::map<int, ToolCallAcc>& tool_calls_by_index,
    const ToolCallCallback&           onToolCall);

} // namespace ModelerAi::provider::openai_chat
