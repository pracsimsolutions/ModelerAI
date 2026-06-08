// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/subscriber.h — multi-subscriber outbound queue registry.
//
// The bridge fans out every pushEvent() to every registered subscriber's
// queue. The local CEF viewer is always registered (id = "local-flexsim"
// at startup). Each connected phone/browser registers a fresh subscriber
// on its first poll and self-identifies on every subsequent request via
// the `mraisid` cookie.
//
// Subscribers GC themselves after kIdleSecondsBeforeGC seconds of no
// poll activity. The local-flexsim subscriber is exempt from GC.

#pragma once

#include <chrono>
#include <deque>
#include <string>
#include <vector>

namespace ModelerAi::bridge {

constexpr int kIdleSecondsBeforeGC = 30;

constexpr const char* kLocalFlexsimSubscriberId = "local-flexsim";

struct Subscriber {
    std::string                                    id;
    std::deque<std::string>                        queue;
    std::chrono::steady_clock::time_point          last_seen;
    std::string                                    client_ip;     // empty for local
    std::string                                    user_agent;    // empty for local

    // Diagnostic ring buffers — last N envelopes received from this
    // subscriber (their /api/send or bridgeSend calls) and last N
    // envelopes we delivered TO this subscriber (their drain). Surfaced
    // by `/remote diag` so we can see what each side actually said
    // without having to run wireshark or rebuild the DLL with extra
    // logging. Capped at kRecentRingSize entries (oldest dropped).
    std::deque<std::string>                        recent_received;
    std::deque<std::string>                        recent_sent;
};

// Cap on the diagnostic ring buffers. Five is enough to spot the recent
// pattern without blowing up the /remote diag bubble.
constexpr size_t kRecentRingSize = 5;

// One-time init — registers the local-flexsim subscriber. Called from
// bootstrap::initialize.
void initSubscriberRegistry();

// Register a new remote subscriber. Called from remote_server on first
// authed poll from a previously-unseen client. Returns the subscriber id
// (which the caller serializes into the mraisid cookie).
std::string registerRemoteSubscriber(const std::string& client_ip,
                                      const std::string& user_agent);

// Adopt a client-supplied subscriber id (from the JS-side localStorage).
// No-op if `id` is already registered. Used by the X-Mraisid header path
// so the same id survives server restarts without the JS having to renegotiate.
// Token-bearer auth gates who can call this — collisions are not a security
// concern, they would just merge two clients' queues.
void adoptRemoteSubscriber(const std::string& id,
                            const std::string& client_ip,
                            const std::string& user_agent);

// Touch the subscriber's last_seen — every poll calls this.
// No-op if id is unknown (caller should handle re-register).
void touchSubscriber(const std::string& id);

// Drop a subscriber from the registry. Used by remote_server when an
// admin clicks Disconnect, and by the GC sweep.
void removeSubscriber(const std::string& id);

// Drain the named subscriber's queue. Returns a JSON array string. Empty
// array if unknown id (the caller should treat that as a re-register signal).
std::string drainSubscriber(const std::string& id);

// Targeted enqueue — push to ONE subscriber's queue. No-op if `id`
// isn't registered. Used by bridge::pushEventTo for response routing.
void enqueueTo(std::string_view id, const std::string& envelopeJson);

// Returns true iff the id is registered. Used by remote_server to detect
// stale cookies.
bool hasSubscriber(const std::string& id);

// Snapshot of all currently-registered subscribers, used by the
// remote_status state_update builder for the Settings panel.
std::vector<Subscriber> snapshotSubscribers();

// Run one GC pass — drops any non-local subscriber whose last_seen is
// older than kIdleSecondsBeforeGC. Called from the remote server's
// idle ticker.
void garbageCollectSubscribers();

// Diagnostic-only: queue depth for the named subscriber. -1 if unknown.
// Used by `/remote diag` to surface whether outbound events are piling
// up because a subscriber stopped polling.
int subscriberQueueDepth(const std::string& id);

// Diagnostic ring writes — append a single line to the per-subscriber
// recv/sent ring. Caller passes the envelope JSON; this records a
// trimmed preview with a timestamp prefix so /remote diag can display
// recent traffic. No-op for unknown ids. Thread-safe.
void recordSubscriberRecv(std::string_view id, std::string_view envelopeJson);
void recordSubscriberSent(std::string_view id, std::string_view envelopeJson);

} // namespace ModelerAi::bridge
