// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/subscriber.cpp — implementation of the multi-subscriber registry.

#include "bridge/subscriber.h"
#include "third_party/json.h"

#include <chrono>
#include <ctime>
#include <iomanip>
#include <mutex>
#include <random>
#include <sstream>
#include <unordered_map>

namespace ModelerAi::bridge {

namespace {

std::mutex                                  g_mu;
std::unordered_map<std::string, Subscriber> g_subs;

// 8-hex-char random suffix used to form "remote-<suffix>" subscriber ids.
std::string makeRandomSuffix()
{
    static thread_local std::mt19937_64 rng{std::random_device{}()};
    std::uniform_int_distribution<uint64_t> dist;
    std::ostringstream oss;
    oss << std::hex << (dist(rng) & 0xFFFFFFFFu);
    return oss.str();
}

std::string hhmmssMs()
{
    auto now = std::chrono::system_clock::now();
    auto t   = std::chrono::system_clock::to_time_t(now);
    auto ms  = std::chrono::duration_cast<std::chrono::milliseconds>(
                  now.time_since_epoch()) % 1000;
    std::tm local;
    localtime_s(&local, &t);
    std::ostringstream os;
    os << std::put_time(&local, "%H:%M:%S")
       << '.' << std::setw(3) << std::setfill('0') << ms.count();
    return os.str();
}

// Trim an envelope to fit on a diag bubble line — preserve type + id +
// short payload preview. Keeps the output readable when the chat is on
// a phone with no horizontal scroll headroom.
std::string makePreview(std::string_view envelopeJson)
{
    constexpr size_t kMaxLen = 120;
    try {
        auto j = nlohmann::json::parse(envelopeJson);
        std::string t  = j.value("t", "?");
        std::string id = j.value("id", "");
        std::string head = t + " id=" + id;
        if (j.contains("p")) {
            std::string ps = j["p"].dump();
            if (ps.size() > 60) ps = ps.substr(0, 57) + "...";
            head += " p=" + ps;
        }
        if (head.size() > kMaxLen) head = head.substr(0, kMaxLen - 3) + "...";
        return head;
    } catch (...) {
        std::string s(envelopeJson);
        if (s.size() > kMaxLen) s = s.substr(0, kMaxLen - 3) + "...";
        return s;
    }
}

// Caller MUST hold g_mu — does not lock. Append + trim to kRecentRingSize.
void appendToRing(std::deque<std::string>& ring, std::string_view envelopeJson)
{
    ring.push_back("[" + hhmmssMs() + "] " + makePreview(envelopeJson));
    while (ring.size() > kRecentRingSize) ring.pop_front();
}

} // namespace

void initSubscriberRegistry()
{
    std::lock_guard<std::mutex> lk(g_mu);
    Subscriber s;
    s.id        = kLocalFlexsimSubscriberId;
    s.last_seen = std::chrono::steady_clock::now();
    g_subs[s.id] = std::move(s);
}

std::string registerRemoteSubscriber(const std::string& client_ip,
                                      const std::string& user_agent)
{
    std::lock_guard<std::mutex> lk(g_mu);
    Subscriber s;
    s.id         = "remote-" + makeRandomSuffix();
    s.last_seen  = std::chrono::steady_clock::now();
    s.client_ip  = client_ip;
    s.user_agent = user_agent;
    g_subs[s.id] = std::move(s);
    return g_subs[s.id].id;
}

void adoptRemoteSubscriber(const std::string& id,
                            const std::string& client_ip,
                            const std::string& user_agent)
{
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_subs.count(id)) return;     // already known — no-op
    Subscriber s;
    s.id         = id;
    s.last_seen  = std::chrono::steady_clock::now();
    s.client_ip  = client_ip;
    s.user_agent = user_agent;
    g_subs[id]   = std::move(s);
}

void touchSubscriber(const std::string& id)
{
    std::lock_guard<std::mutex> lk(g_mu);
    auto it = g_subs.find(id);
    if (it != g_subs.end()) it->second.last_seen = std::chrono::steady_clock::now();
}

void removeSubscriber(const std::string& id)
{
    std::lock_guard<std::mutex> lk(g_mu);
    if (id == kLocalFlexsimSubscriberId) return; // never drop local
    g_subs.erase(id);
}

std::string drainSubscriber(const std::string& id)
{
    std::deque<std::string> drained;
    {
        std::lock_guard<std::mutex> lk(g_mu);
        auto it = g_subs.find(id);
        if (it == g_subs.end()) return "[]";
        drained.swap(it->second.queue);
        it->second.last_seen = std::chrono::steady_clock::now();
    }
    nlohmann::json arr = nlohmann::json::array();
    for (auto& s : drained) {
        try { arr.push_back(nlohmann::json::parse(s)); } catch (...) {}
    }
    return arr.dump();
}

bool hasSubscriber(const std::string& id)
{
    std::lock_guard<std::mutex> lk(g_mu);
    return g_subs.find(id) != g_subs.end();
}

std::vector<Subscriber> snapshotSubscribers()
{
    std::lock_guard<std::mutex> lk(g_mu);
    std::vector<Subscriber> out;
    out.reserve(g_subs.size());
    for (const auto& kv : g_subs) out.push_back(kv.second);
    return out;
}

int subscriberQueueDepth(const std::string& id)
{
    std::lock_guard<std::mutex> lk(g_mu);
    auto it = g_subs.find(id);
    if (it == g_subs.end()) return -1;
    return (int)it->second.queue.size();
}

void garbageCollectSubscribers()
{
    auto cutoff = std::chrono::steady_clock::now()
                  - std::chrono::seconds(kIdleSecondsBeforeGC);
    std::lock_guard<std::mutex> lk(g_mu);
    for (auto it = g_subs.begin(); it != g_subs.end();) {
        if (it->first != kLocalFlexsimSubscriberId && it->second.last_seen < cutoff) {
            it = g_subs.erase(it);
        } else {
            ++it;
        }
    }
}

// Internal-only — used by bridge.cpp's pushEvent fan-out. Declared in
// bridge.cpp via extern; not part of the public header.
void enqueueToAll(const std::string& envelopeJson);

void enqueueToAll(const std::string& envelopeJson)
{
    std::lock_guard<std::mutex> lk(g_mu);
    for (auto& kv : g_subs) {
        kv.second.queue.push_back(envelopeJson);
        appendToRing(kv.second.recent_sent, envelopeJson);
    }
}

void enqueueTo(std::string_view id, const std::string& envelopeJson)
{
    std::lock_guard<std::mutex> lk(g_mu);
    auto it = g_subs.find(std::string(id));
    if (it == g_subs.end()) return;   // unknown subscriber — silently drop
    it->second.queue.push_back(envelopeJson);
    appendToRing(it->second.recent_sent, envelopeJson);
}

void recordSubscriberRecv(std::string_view id, std::string_view envelopeJson)
{
    std::lock_guard<std::mutex> lk(g_mu);
    auto it = g_subs.find(std::string(id));
    if (it == g_subs.end()) return;
    appendToRing(it->second.recent_received, envelopeJson);
}

void recordSubscriberSent(std::string_view id, std::string_view envelopeJson)
{
    std::lock_guard<std::mutex> lk(g_mu);
    auto it = g_subs.find(std::string(id));
    if (it == g_subs.end()) return;
    appendToRing(it->second.recent_sent, envelopeJson);
}

} // namespace ModelerAi::bridge
