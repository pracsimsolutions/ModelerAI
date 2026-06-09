// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// remote/qr_payload.h — assembles the `remote_status` state_update
// payload shown in the chat bubble and the Settings → Remote section.
//
// All formatting (URLs, token-short, label strings) is pure logic so
// the same builder is reusable across both surfaces.

#pragma once

#include "remote/lan_iface.h"
#include "third_party/json.h"

#include <string>
#include <vector>

namespace ModelerAi::remote {

// NB: `ip` would collide with the FlexsimDefs.h macro — use `ipAddress`
// on the C++ side and serialize as "ip" on the JSON wire.
struct ConnectedClient {
    std::string id;            // subscriber id ("remote-...")
    std::string ipAddress;
    std::string user_agent;
    int         last_seen_seconds_ago = 0;
};

struct RemoteStatusInputs {
    bool                          enabled = false;
    int                           port = 0;
    std::string                   token;             // full token (used for URL)
    std::vector<LanIface>         interfaces;        // detectLanInterfaces() output
    std::vector<ConnectedClient>  clients;           // snapshot
    long long                     uptime_seconds = 0;
    std::string                   tunnel_public_url; // empty when not tunneled
    std::string                   tunnel_mode = "none"; // "none" | "quick" | "named"
};

// Build the JSON for the `remote_status` state_update.
nlohmann::json buildRemoteStatusJson(const RemoteStatusInputs& in);

// One-stop: snapshot server state + subscribers, build the full envelope,
// return as a JSON string ready for pushEvent / pushEventTo. Used by:
//   - pushInitialState (so fresh viewers know if the server is on),
//   - subscriber lifecycle pushes from /api/poll,
//   - broadcastRemoteStatus() throttled lifecycle broadcasts.
std::string buildRemoteStatusEnvelopeJson(const std::string& turnId);

} // namespace ModelerAi::remote
