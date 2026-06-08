// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "agent/user_interaction.h"

namespace ModelerAi::agent {

void InteractionBroker::registerPending(const std::string& interaction_id)
{
    std::lock_guard<std::mutex> lk(tableMutex_);
    pending_[interaction_id] = std::make_shared<Entry>();
}

std::optional<nlohmann::json> InteractionBroker::waitFor(
    const std::string& interaction_id,
    std::chrono::milliseconds timeout)
{
    std::shared_ptr<Entry> entry;
    {
        std::lock_guard<std::mutex> lk(tableMutex_);
        auto it = pending_.find(interaction_id);
        if (it == pending_.end()) return std::nullopt;
        entry = it->second;
    }

    std::unique_lock<std::mutex> lk(entry->m);
    bool ok = entry->cv.wait_for(lk, timeout, [&]() {
        return entry->resolved || entry->cancelled;
    });

    // Pull the response under the lock, then drop the entry from the
    // table. We're done with it either way.
    std::optional<nlohmann::json> out;
    if (ok && entry->resolved && !entry->cancelled) {
        out = entry->response;
    }
    lk.unlock();

    {
        std::lock_guard<std::mutex> tlk(tableMutex_);
        pending_.erase(interaction_id);
    }
    return out;
}

void InteractionBroker::resolve(const std::string& interaction_id,
                                nlohmann::json response)
{
    std::shared_ptr<Entry> entry;
    {
        std::lock_guard<std::mutex> lk(tableMutex_);
        auto it = pending_.find(interaction_id);
        if (it == pending_.end()) return;  // silent no-op (idempotent)
        entry = it->second;
    }
    {
        std::lock_guard<std::mutex> lk(entry->m);
        entry->resolved = true;
        entry->response = std::move(response);
    }
    entry->cv.notify_all();
}

void InteractionBroker::cancelAll()
{
    std::lock_guard<std::mutex> lk(tableMutex_);
    for (auto& kv : pending_) {
        auto& entry = kv.second;
        {
            std::lock_guard<std::mutex> elk(entry->m);
            entry->cancelled = true;
        }
        entry->cv.notify_all();
    }
    // Entries get removed by their own waitFor() returning. If a thread
    // somehow already exited without removing, the dangling entries are
    // harmless — they just hold a tiny bit of memory until process exit.
}

} // namespace ModelerAi::agent
