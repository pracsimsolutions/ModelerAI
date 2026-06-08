// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "remote/qr_payload.h"
#include "remote/remote_server.h"
#include "bridge/subscriber.h"

#include <chrono>

namespace ModelerAi::remote {

namespace {

std::string urlFor(const std::string& ip, int port, const std::string& token)
{
    return "http://" + ip + ":" + std::to_string(port) + "/#token=" + token;
}

// First 4 + last 4 chars, hyphenated in the middle. e.g. "AB7d…XYz9" for a
// 24-char token.
std::string tokenShort(const std::string& token)
{
    if (token.size() <= 10) return token;
    return token.substr(0, 4) + "…" + token.substr(token.size() - 4);
}

} // namespace

nlohmann::json buildRemoteStatusJson(const RemoteStatusInputs& in)
{
    nlohmann::json v;
    v["enabled"]        = in.enabled;
    v["port"]           = in.port;
    v["uptime_seconds"] = in.uptime_seconds;

    if (!in.enabled) {
        v["primary_url"]    = "";
        v["primary_label"]  = "";
        v["all_urls"]       = nlohmann::json::array();
        v["token_short"]    = "";
        v["connected_clients"] = nlohmann::json::array();
        return v;
    }

    nlohmann::json allUrls = nlohmann::json::array();
    for (const auto& i : in.interfaces) {
        nlohmann::json e;
        e["label"] = i.label;
        e["url"]   = urlFor(i.ipAddress, in.port, in.token);
        allUrls.push_back(std::move(e));
    }

    v["primary_url"]   = in.interfaces.empty()
                         ? std::string()
                         : urlFor(in.interfaces.front().ipAddress, in.port, in.token);
    v["primary_label"] = in.interfaces.empty()
                         ? std::string()
                         : in.interfaces.front().label;
    v["all_urls"]      = std::move(allUrls);
    v["token_short"]   = tokenShort(in.token);

    nlohmann::json clients = nlohmann::json::array();
    for (const auto& c : in.clients) {
        nlohmann::json e;
        e["id"]                    = c.id;
        e["ip"]                    = c.ipAddress;  // wire key stays "ip"
        e["user_agent"]            = c.user_agent;
        e["last_seen_seconds_ago"] = c.last_seen_seconds_ago;
        clients.push_back(std::move(e));
    }
    v["connected_clients"] = std::move(clients);

    return v;
}

std::string buildRemoteStatusEnvelopeJson(const std::string& turnId)
{
    // Same logic that slash_remote::pushRemoteStatus uses, factored out
    // so it can be called from non-slash code paths (initial state,
    // subscriber lifecycle hooks).
    auto in = status();
    auto subs = bridge::snapshotSubscribers();
    auto now  = std::chrono::steady_clock::now();
    for (const auto& s : subs) {
        if (s.id == bridge::kLocalFlexsimSubscriberId) continue;
        ConnectedClient cc;
        cc.id                    = s.id;
        cc.ipAddress             = s.client_ip;
        cc.user_agent            = s.user_agent;
        cc.last_seen_seconds_ago = (int)std::chrono::duration_cast<std::chrono::seconds>(
                                       now - s.last_seen).count();
        in.clients.push_back(std::move(cc));
    }
    nlohmann::json v = buildRemoteStatusJson(in);

    nlohmann::json env;
    env["t"]  = "state_update";
    env["id"] = turnId;
    nlohmann::json p;
    p["key"]   = "remote_status";
    p["value"] = std::move(v);
    env["p"]   = std::move(p);
    return env.dump();
}

} // namespace ModelerAi::remote
