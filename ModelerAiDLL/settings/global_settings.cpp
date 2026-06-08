// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "settings/global_settings.h"
#include "bridge/log.h"
#include "paths.h"

#include <windows.h>
#include <fstream>
#include <sstream>
#include <string>

namespace ModelerAi::settings {

namespace {

std::string filePath()
{
    std::string dir = paths::appDataDir();
    if (dir.empty()) return "";
    return dir + "settings.json";
}

} // namespace

nlohmann::json loadGlobalSettings()
{
    std::string path = filePath();
    if (path.empty()) return nlohmann::json::object();
    std::ifstream f(path, std::ios::binary);
    if (!f) return nlohmann::json::object();
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string body = ss.str();
    if (body.empty()) return nlohmann::json::object();
    try {
        auto j = nlohmann::json::parse(body);
        if (j.is_object()) {
            // Strip the version wrapper if present so callers don't have
            // to special-case it.
            if (j.contains("settings") && j["settings"].is_object()) {
                return j["settings"];
            }
            return j;
        }
        return nlohmann::json::object();
    } catch (...) {
        bridge::consolePrint("[ModelerAI] WARN: settings.json parse failed\n");
        return nlohmann::json::object();
    }
}

bool saveGlobalSettings(const nlohmann::json& j)
{
    std::string path = filePath();
    if (path.empty()) return false;
    std::string tmp = path + ".tmp";

    nlohmann::json doc;
    doc["version"]  = 1;
    doc["settings"] = j;

    {
        std::ofstream f(tmp, std::ios::binary);
        if (!f) return false;
        f << doc.dump(2);
    }
    return MoveFileExA(tmp.c_str(), path.c_str(),
        MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH) != 0;
}

bool patchGlobalSettings(const nlohmann::json& patch)
{
    auto current = loadGlobalSettings();
    if (!patch.is_object()) return false;
    // Merge: for each key in patch, overwrite (or insert).
    for (auto it = patch.begin(); it != patch.end(); ++it) {
        current[it.key()] = it.value();
    }
    return saveGlobalSettings(current);
}

} // namespace ModelerAi::settings
