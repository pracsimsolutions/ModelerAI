// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/provider.h — abstract IProvider interface and supporting types.
//
// Every LLM adapter (Anthropic, OpenAI, Gemini) implements IProvider. The
// agent loop only ever holds an IProvider*; provider-specific shapes
// (SSE event formats, tool-call JSON layouts, etc.) are translated inside
// each adapter so the agent can stay generic.

#pragma once

#include <atomic>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace ModelerAi::provider {

// ---------------------------------------------------------------------------
// Effort level — maps to Anthropic's `thinking.budget_tokens` for models that
// support extended thinking. Off is the default (no thinking, lowest cost).
// ---------------------------------------------------------------------------
enum class EffortLevel { Off, Low, Medium, High };

// ---------------------------------------------------------------------------
// Static metadata exposed to the settings UI.
// ---------------------------------------------------------------------------
struct ModelInfo {
    std::string id;             // e.g. "claude-opus-4-8"
    std::string display_name;   // e.g. "Opus 4.8"
    int         context_tokens; // soft cap surfaced in the UI
    bool        supports_tools;
    bool        supports_vision;
    // True when the model accepts the `thinking` field for budget-controlled
    // extended thinking. Claude Opus 4.8 does NOT — it uses adaptive
    // thinking which is automatic. Sonnet 4.6 and Haiku 4.5 both do.
    bool        supports_thinking = false;
    // True when the model accepts a `temperature` parameter. Opus 4.8
    // rejects temperature outright (returns 400 "`temperature` is
    // deprecated for this model"). Default true keeps existing models
    // unaffected; flip this off per-model as Anthropic deprecates it.
    bool        accepts_temperature = true;

    // Per-Mtok USD pricing. Used for cost telemetry. Verify against current
    // vendor docs before relying on these for business decisions.
    double      input_usd_per_mtok        = 0.0;
    double      output_usd_per_mtok       = 0.0;
    double      cache_write_usd_per_mtok  = 0.0;
    double      cache_read_usd_per_mtok   = 0.0;
};

// ---------------------------------------------------------------------------
// Streaming response types — what the adapter emits as the model replies.
// ---------------------------------------------------------------------------
struct StreamChunk {
    std::string text;
    bool        is_final;
};

struct ProviderToolCall {
    std::string call_id;
    std::string name;
    std::string args_json;  // raw JSON; tool layer parses per its schema
};

struct ProviderError {
    std::string code;       // e.g. "rate_limited", "invalid_key", "network"
    std::string message;
    bool        retriable;
};

struct TurnResult {
    std::string message_id;                  // provider-assigned, used for history
    int         input_tokens = 0;            // tokens billed as input (excludes cache reads)
    int         output_tokens = 0;           // tokens billed as output (5x input rate)
    int         cache_read_input_tokens = 0; // tokens served from prompt cache (~10% input rate)
    int         cache_creation_input_tokens = 0; // tokens written to cache (slight surcharge)
    double      cost_usd = 0.0;              // adapter-computed cost for this single turn
    std::string model_id;                    // the model that actually answered (may differ from default if mid-turn override)
};

// Result of a one-shot key-validity check (see IProvider::test_key).
struct TestKeyResult {
    bool        ok = false;
    int         http_status = 0;   // 0 on network failure, else the HTTP code
    std::string error_message;     // user-visible reason on failure
};

} // namespace ModelerAi::provider

// ---------------------------------------------------------------------------
// Conversation + tool definitions live in the agent namespace. Forward-declare
// them here so the IProvider interface can reference them without pulling in
// the full message.h include.
// ---------------------------------------------------------------------------
namespace ModelerAi::agent {
    struct Conversation;
    struct ToolDef;
}

namespace ModelerAi::provider {

// ---------------------------------------------------------------------------
// Callbacks for the streaming turn.
// ---------------------------------------------------------------------------
using StreamCallback   = std::function<void(const StreamChunk&)>;
using ToolCallCallback = std::function<void(const ProviderToolCall&)>;
using DoneCallback     = std::function<void(const TurnResult&)>;
using ErrorCallback    = std::function<void(const ProviderError&)>;

// Cooperative cancellation token. Caller (the agent) owns the atomic and
// passes a pointer so the adapter — and the WinHTTP read loop deep inside
// it — can poll it between chunks. Setting to true makes the next read
// short-circuit; the provider reports back via ErrorCallback with code
// "cancelled". Nullable: providers must tolerate a null pointer.
using CancelToken = const std::atomic<bool>*;

// ---------------------------------------------------------------------------
// The interface every adapter implements.
// ---------------------------------------------------------------------------
class IProvider {
public:
    virtual ~IProvider() = default;

    /** Stable id used in bridge messages. e.g. "anthropic". */
    virtual std::string id() const = 0;

    /** Display label for the settings UI. e.g. "Anthropic (Claude)". */
    virtual std::string display_name() const = 0;

    /** Models the user can pick. Static list per provider, version-pinned. */
    virtual std::vector<ModelInfo> models() const = 0;

    /** Whether a key has been configured for this provider. The keystore
     *  layer answers this — the adapter just forwards the check. */
    virtual bool has_api_key() const = 0;

    /** Start a streaming turn. Blocks the calling (worker) thread until the
     *  stream completes, errors, or `cancel` flips to true.
     *  Callbacks fire on this thread; the agent layer marshals them back to
     *  the main thread before touching FlexSim state. */
    virtual void stream_turn(
        const agent::Conversation&         conv,
        const std::vector<agent::ToolDef>& tools,
        StreamCallback                     onChunk,
        ToolCallCallback                   onToolCall,
        DoneCallback                       onDone,
        ErrorCallback                      onError,
        CancelToken                        cancel = nullptr) = 0;

    /** Synchronous one-shot key check. Should be cheap (a list-models GET,
     *  or a 1-token completion). Blocks the calling thread; bridge dispatches
     *  on a worker. */
    virtual TestKeyResult test_key() = 0;

    /** The currently-active model's ID (the one stream_turn will use on the
     *  next call). Empty if none selected. */
    virtual std::string active_model_id() const = 0;

    /** Switch the active model. Returns true if id was found in models();
     *  false (and a no-op) otherwise. */
    virtual bool set_active_model(std::string_view id) = 0;

    /** Current effort level for extended thinking. Off by default. */
    virtual EffortLevel effort() const = 0;

    /** Set effort level. No-op if the active model doesn't support thinking. */
    virtual void set_effort(EffortLevel level) = 0;
};

using IProviderPtr = std::unique_ptr<IProvider>;

} // namespace ModelerAi::provider
