// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "paths.h"

#include <windows.h>
#include <shlobj.h>
#include <knownfolders.h>

#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "ole32.lib")

namespace ModelerAi::paths {

namespace {

std::string wideToUtf8(const wchar_t* w)
{
    if (!w || !*w) return "";
    int len = WideCharToMultiByte(CP_UTF8, 0, w, -1, nullptr, 0, nullptr, nullptr);
    if (len <= 1) return "";
    std::string s(len - 1, 0);
    WideCharToMultiByte(CP_UTF8, 0, w, -1, &s[0], len, nullptr, nullptr);
    return s;
}

std::string resolveAppDataDir()
{
    PWSTR roaming = nullptr;
    if (FAILED(SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, nullptr, &roaming))) {
        if (roaming) CoTaskMemFree(roaming);
        return "";
    }
    std::string base = wideToUtf8(roaming);
    CoTaskMemFree(roaming);
    if (base.empty()) return "";
    if (base.back() != '\\') base += "\\";

    std::string pracsim = base + "PRACSIM\\";
    CreateDirectoryA(pracsim.c_str(), nullptr);  // idempotent
    std::string modelerAi = pracsim + "ModelerAI\\";
    CreateDirectoryA(modelerAi.c_str(), nullptr);
    return modelerAi;
}

std::string resolveModuleInstallDir()
{
    HMODULE h = GetModuleHandleA("ModelerAI.dll");
    if (!h) return "";
    char buf[MAX_PATH];
    DWORD n = GetModuleFileNameA(h, buf, MAX_PATH);
    if (n == 0 || n >= MAX_PATH) return "";
    std::string p(buf, n);
    auto slash = p.find_last_of("\\/");
    if (slash == std::string::npos) return "";
    return p.substr(0, slash + 1);
}

} // namespace

std::string appDataDir()
{
    static const std::string s = resolveAppDataDir();
    return s;
}

std::string bridgeLogPath()
{
    return appDataDir() + "bridge.log";
}

std::string diagLogPath()
{
    return appDataDir() + "diag.log";
}

std::string moduleInstallDir()
{
    static const std::string s = resolveModuleInstallDir();
    return s;
}

std::string customProvidersPath()
{
    std::string d = appDataDir();
    if (d.empty()) return "";
    return d + "custom_providers.json";
}

} // namespace ModelerAi::paths
