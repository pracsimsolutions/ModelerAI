// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/user_interaction.h — pause-and-wait primitive.
//
// Some tool flows need user input mid-turn: approving a mutating tool
// call, picking from an ask_user_question, etc. The InteractionBroker
// lets the worker thread block on a future the bridge fulfills when
// the viewer sends back a response envelope.
//
// One broker per Agent instance. Created during Agent construction,
// torn down at teardown. The bridge exposes it via a global accessor
// (bootstrap::interactionBroker) so envelope handlers can resolve
// pending requests without holding an Agent reference.

#pragma once

#include "third_party/json.h"

#include <chrono>
#include <condition_variable>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <unordered_map>

namespace ModelerAi::agent {

class InteractionBroker {
public:
    // Bridge side, called from the worker thread BEFORE pushing the
    // request envelope: register the id so resolve() can find it.
    void registerPending(const std::string& interaction_id);

    // Worker side: block until the matching response arrives or the
    // timeout fires. Returns the response JSON on success, nullopt on
    // timeout or cancellation. Auto-cleans the pending entry.
    std::optional<nlohmann::json> waitFor(
        const std::string& interaction_id,
        std::chrono::milliseconds timeout);

    // Bridge side: viewer sent a response. Fulfills the matching
    // pending entry. Silent no-op if the id isn't pending (idempotent
    // double-resolves, late timeouts, etc. won't crash).
    void resolve(const std::string& interaction_id, nlohmann::json response);

    // Cancel any pending interactions. Used on /clear or teardown so
    // worker threads waiting for input unblock with nullopt instead
    // of hanging forever.
    void cancelAll();

private:
    struct Entry {
        std::mutex                m;
        std::condition_variable   cv;
        bool                      resolved = false;
        bool                      cancelled = false;
        nlohmann::json            response;
    };

    std::mutex                                              tableMutex_;
    std::unordered_map<std::string, std::shared_ptr<Entry>> pending_;
};

} // namespace ModelerAi::agent
