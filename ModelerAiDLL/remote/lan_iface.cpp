// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "remote/lan_iface.h"

#include <winsock2.h>
#include <iphlpapi.h>
#include <ws2tcpip.h>
#include <vector>
#include <string>
#include <algorithm>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

namespace ModelerAi::remote {

namespace {

// RFC1918 ranges (IPv4):
//   10.0.0.0/8
//   172.16.0.0/12
//   192.168.0.0/16
bool isPrivateIPv4(uint32_t ip_be)
{
    uint32_t ip = ntohl(ip_be);
    if ((ip & 0xFF000000u) == 0x0A000000u) return true; // 10/8
    if ((ip & 0xFFF00000u) == 0xAC100000u) return true; // 172.16/12
    if ((ip & 0xFFFF0000u) == 0xC0A80000u) return true; // 192.168/16
    return false;
}

bool isLikelyVirtual(const std::wstring& description)
{
    static const wchar_t* tokens[] = {
        L"Hyper-V", L"Virtual", L"VMware", L"VirtualBox", L"WSL",
        L"Docker", L"Loopback", L"TAP", L"Tunnel", L"vEthernet"
    };
    for (auto* t : tokens) {
        if (description.find(t) != std::wstring::npos) return true;
    }
    return false;
}

std::string wideToUtf8(const std::wstring& w)
{
    if (w.empty()) return "";
    int n = WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), nullptr, 0, nullptr, nullptr);
    std::string out(n, '\0');
    WideCharToMultiByte(CP_UTF8, 0, w.data(), (int)w.size(), out.data(), n, nullptr, nullptr);
    return out;
}

std::string ipToString(uint32_t ip_be)
{
    in_addr a; a.s_addr = ip_be;
    char buf[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &a, buf, sizeof(buf));
    return std::string(buf);
}

} // namespace

std::vector<LanIface> detectLanInterfaces()
{
    std::vector<LanIface> out;

    ULONG bufSize = 0;
    if (GetAdaptersAddresses(AF_INET,
                             GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
                             nullptr, nullptr, &bufSize) != ERROR_BUFFER_OVERFLOW) {
        return out;
    }
    std::vector<uint8_t> buf(bufSize);
    auto* adapters = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(buf.data());
    if (GetAdaptersAddresses(AF_INET,
                             GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
                             nullptr, adapters, &bufSize) != NO_ERROR) {
        return out;
    }

    for (auto* a = adapters; a; a = a->Next) {
        if (a->OperStatus != IfOperStatusUp) continue;
        if (a->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;
        std::wstring desc = a->Description ? a->Description : L"";
        std::wstring friendlyW = a->FriendlyName ? a->FriendlyName : L"";
        if (isLikelyVirtual(desc) || isLikelyVirtual(friendlyW)) continue;

        for (auto* ua = a->FirstUnicastAddress; ua; ua = ua->Next) {
            if (!ua->Address.lpSockaddr) continue;
            if (ua->Address.lpSockaddr->sa_family != AF_INET) continue;
            auto* sin = reinterpret_cast<sockaddr_in*>(ua->Address.lpSockaddr);
            uint32_t ip_be = sin->sin_addr.S_un.S_addr;
            // Skip 169.254.0.0/16 link-local
            if ((ntohl(ip_be) & 0xFFFF0000u) == 0xA9FE0000u) continue;

            LanIface li;
            li.ipAddress        = ipToString(ip_be);
            li.adapter_friendly = wideToUtf8(friendlyW);
            li.label            = li.adapter_friendly + " (" + li.ipAddress + ")";
            li.is_private       = isPrivateIPv4(ip_be);
            out.push_back(std::move(li));
        }
    }

    // Private IPs first, then preserve discovery order.
    std::stable_sort(out.begin(), out.end(),
                     [](const LanIface& a, const LanIface& b) {
                         return a.is_private && !b.is_private;
                     });
    return out;
}

} // namespace ModelerAi::remote
