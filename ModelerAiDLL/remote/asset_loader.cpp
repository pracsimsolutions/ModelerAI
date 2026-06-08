// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "remote/asset_loader.h"
#include "paths.h"

#include <filesystem>
#include <fstream>
#include <mutex>
#include <sstream>

namespace ModelerAi::remote {

namespace {
std::mutex   g_mu;
std::string  g_cached;
bool         g_loaded = false;
} // namespace

const std::string& loadEmbeddedViewer()
{
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_loaded) return g_cached;

    std::string installDir = paths::moduleInstallDir();
    if (!installDir.empty()) {
        std::filesystem::path p =
            std::filesystem::path(installDir) / "webview" / "index.html";
        std::ifstream f(p, std::ios::in | std::ios::binary);
        if (f.is_open()) {
            std::ostringstream buf;
            buf << f.rdbuf();
            g_cached = buf.str();
        }
    }
    g_loaded = true;
    return g_cached;
}

void invalidateViewerCache()
{
    std::lock_guard<std::mutex> lk(g_mu);
    g_cached.clear();
    g_loaded = false;
}

} // namespace ModelerAi::remote
