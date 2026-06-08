// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// remote/lan_iface.h — Windows network adapter discovery for the
// remote viewer's QR-code IP picker.
//
// detectLanInterfaces() walks GetAdaptersAddresses, skips virtual /
// down / loopback adapters, returns the candidate list sorted with the
// most likely "real LAN" interface first. The remote_status JSON
// builder uses [0] as the primary URL and the rest under "all_urls".

#pragma once

#include <string>
#include <vector>

namespace ModelerAi::remote {

// NB: `ip` would collide with FlexsimDefs.h `#define ip ((size_t)edsdata(...))`
// macro — call the field `ipAddress` here and only serialize as "ip" on
// the JSON wire.
struct LanIface {
    std::string label;              // "WiFi (192.168.1.42)" / "Ethernet (10.0.0.5)"
    std::string ipAddress;          // "192.168.1.42"
    std::string adapter_friendly;   // "Wi-Fi" / "Ethernet" / "Hyper-V Virtual Ethernet Adapter"
    bool        is_private = false; // RFC1918
};

// Returns the candidate LAN interfaces, sorted primary-first.
std::vector<LanIface> detectLanInterfaces();

} // namespace ModelerAi::remote
