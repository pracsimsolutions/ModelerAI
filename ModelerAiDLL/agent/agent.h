// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/agent.h — turn driver.
//
// One Agent per DLL. Owns the conversation history. Each user message
// kicks off a worker thread that streams chunks back through the
// outbound-push callback the bridge supplies.

#pragma once

#include "agent/message.h"
#include "agent/user_interaction.h"

#include <atomic>
#include <functional>
#include <mutex>
#include <string>
#include <string_view>
#include <unordered_map>

namespace ModelerAi::provider {
    class IProvider;
}

namespace ModelerAi::agent {

// Cumulative usage / cost counters for the current session. Cleared by
// `/clear` and `/new`; mutated by the agent on every assistant-done.
struct SessionStats {
    long long input_tokens = 0;
    long long output_tokens = 0;
    long long cache_read_input_tokens = 0;
    long long cache_creation_input_tokens = 0;
    int       turns = 0;
    double    cost_usd = 0.0;  // Sum of per-turn adapter-computed costs.
};

// Chat mode selected via the header segmented control. Drives tool
// filtering + approval flow. See project-mode-and-approval-design memory.
enum class AgentMode { Ask, Plan, Apply };

// Per-tool runtime policy. Defaults to Ask for any tool the user
// hasn't decided on; AutoAllow set by clicking "Always allow this tool"
// in the approval modal; AlwaysDeny reserved for future "block a tool
// permanently" UX.
enum class ToolPolicy { Ask, AutoAllow, AlwaysDeny };

// Wire-format helpers — keep enum strings in one place.
const char* modeName(AgentMode m);
AgentMode   parseMode(std::string_view s);
const char* policyName(ToolPolicy p);
ToolPolicy  parsePolicy(std::string_view s);

class Agent {
public:
    using OutboundPush = std::function<void(const std::string& envelopeJson)>;

    Agent(provider::IProvider* provider, OutboundPush push);

    // Kick off a turn. condensedModel is captured on the FlexSim main
    // thread before this call so the worker thread never touches the
    // FlexSim tree. attachments are user-supplied images (empty for
    // text-only turns); they ride along on the User message and are
    // forwarded as image content blocks by provider adapters that
    // support multi-block messages (Anthropic does; OpenAI-compatible
    // adapters drop them for now). Returns immediately; the reply
    // streams via push_.
    void startTurn(std::string turnId,
                   std::string userText,
                   std::string condensedModel,
                   std::vector<Attachment> attachments = {});

    // Wipe in-memory conversation. Caller (slash::dispatch) decides whether
    // to also push a confirmation envelope. Returns the number of messages
    // that were cleared (for the confirmation text).
    int clearConversation();

    // Snapshot the current session stats (thread-safe copy).
    SessionStats stats() const;

    // Reset stats counters to zero (doesn't touch conversation history).
    void resetStats();

    // (licenseStatus() / resetFreeQuotaCounter() removed — freeware.)

    // Cooperative cancel of the in-flight turn (if any). Sets an atomic
    // the WinHTTP read loop polls between chunks; the next read short-
    // circuits and the worker thread surfaces a "cancelled" error. Idempotent;
    // safe to call from any thread, including when no turn is in flight.
    // Returns true if a turn was actually in flight.
    bool cancelCurrentTurn();

    // Cancel variant for the model-switch flow: same cancel behavior, but
    // sets a flag that tells the worker to commit any partial assistant
    // text to history (with an "[Interrupted — model switched]" marker
    // appended) before exiting. The OLD model's session then includes
    // the abandoned reply when the user navigates back to it. Pattern
    // mirrors Claude Code / VS Code "stop in flight" behavior.
    bool cancelForModelSwitch();

    // True iff a worker thread is currently running a turn. Used by
    // bootstrap::teardown to wait for the worker to wind down before
    // freeing the agent (avoids a use-after-free in the detached thread).
    bool turnInFlight() const { return turnInFlight_.load(); }

    // True if a cancel was requested on the in-flight turn. Read-only
    // surface for long-running tool implementations (e.g. run_to_time)
    // so they can poll instead of blocking forever — the C++ thread that
    // runs the tool body needs a way to abort when the user clicks Stop.
    bool cancelRequested() const { return cancelRequested_.load(); }

    // Read-only access to the conversation (for /export, future persistence).
    // Caller must hold no Agent locks. Returns a copy.
    Conversation conversationCopy() const;

    // Provider info for /model and the header pill.
    provider::IProvider* provider() const { return provider_; }

    // Switch the active provider mid-session. Future startTurn calls use the
    // new pointer; any in-flight worker thread holds its own captured copy
    // and finishes on the old one cleanly.
    void setProvider(provider::IProvider* p) { provider_ = p; }

    // Current chat mode (Ask / Plan / Apply). Default Ask. The user toggles
    // via the header segmented control; the bridge calls setMode in response.
    AgentMode mode() const;
    void setMode(AgentMode m);

    // Per-tool policy. Defaults to Ask. setPolicy() persists into the
    // in-memory map (volatile — resets on FlexSim restart for v0.3).
    ToolPolicy toolPolicy(std::string_view name) const;
    void       setToolPolicy(std::string_view name, ToolPolicy p);

    // Broker exposed so the bridge can resolve pending interactions on
    // tool_approval_response / ask_user_question_response envelopes.
    InteractionBroker& interactionBroker() { return broker_; }

    // Replace in-memory history with the contents of `loaded`. Called by
    // the bridge after a successful session_store::loadSession. Stats are
    // NOT replayed — historical turns don't roll back into the cost
    // counters (the persisted file is conversation-only, not metering).
    void replaceConversation(Conversation loaded);

    // The session key associated with the current conversation. Empty until
    // the first turn (or load) has fired.
    std::string sessionKey() const;

    // Update the session key (typically when viewer_ready resolves the
    // current FlexSim model). Future saves use this key.
    void setSessionKey(std::string key);

    // Remember the viewer's session-strategy choice so non-viewer-ready
    // re-renders of initial state (after set_setting, reset_settings, etc.)
    // don't silently drop the user back to PerFile. Free-form strings
    // matching SessionStrategy serialization ("per-file"/"per-folder"/"manual").
    void        setSessionStrategy(std::string strategy, std::string customKey);
    std::string sessionStrategy() const;
    std::string sessionCustomKey() const;

private:
    provider::IProvider* provider_;
    OutboundPush         push_;

    mutable std::mutex   historyMutex_;
    Conversation         history_;
    std::string          session_key_;
    std::string          session_strategy_  = "per-file";
    std::string          session_custom_key_;

    mutable std::mutex   statsMutex_;
    SessionStats         stats_;

    // (promptsAttempted_ / promptsCompleted_ removed with the freeware
    // pivot — no quota to track.)

    // Cancel flag for the in-flight turn. The worker thread holds a
    // pointer to this and passes it down to the provider; the WinHTTP
    // read loop polls it between chunks. Reset at the top of each turn.
    std::atomic<bool>    cancelRequested_{false};
    std::atomic<bool>    turnInFlight_{false};
    // True while a cancel was triggered by a model-switch event (vs. an
    // explicit user Stop). Tells the turn worker to commit partial
    // assistant text + [Interrupted] marker into history before exiting.
    std::atomic<bool>    modelSwitchCancel_{false};

    mutable std::mutex   modeMutex_;
    AgentMode            mode_ = AgentMode::Ask;

    mutable std::mutex   policyMutex_;
    std::unordered_map<std::string, ToolPolicy> toolPolicies_;

    InteractionBroker    broker_;
};

} // namespace ModelerAi::agent
