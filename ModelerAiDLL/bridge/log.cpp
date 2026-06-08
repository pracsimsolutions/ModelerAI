// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // for pt() — must come BEFORE third_party/json.h
#include "bridge/log.h"
#include "paths.h"
#include "third_party/json.h"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

namespace ModelerAi::bridge {

namespace {

bool checkEnabled()
{
    const char* v = std::getenv("MODELER_AI_BRIDGE_LOG");
    if (!v) return false;
    return v[0] != '\0' && v[0] != '0';
}

bool g_enabled = checkEnabled();
std::mutex g_mutex;

std::string timestamp()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto t = system_clock::to_time_t(now);
    auto ms = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;

    std::tm utc;
    gmtime_s(&utc, &t);

    std::ostringstream os;
    os << std::put_time(&utc, "%Y-%m-%dT%H:%M:%S")
       << '.' << std::setw(3) << std::setfill('0') << ms.count() << "Z";
    return os.str();
}

} // namespace

void logEnvelope(std::string_view direction, std::string_view envelopeJson)
{
    if (!g_enabled) return;

    std::string pretty;
    try {
        auto j = nlohmann::json::parse(envelopeJson);
        pretty = j.dump(2);
    } catch (...) {
        // Couldn't parse — log as-is so the user can see the malformed
        // payload that caused the failure.
        pretty.assign(envelopeJson);
    }

    std::lock_guard<std::mutex> lock(g_mutex);
    std::ofstream f(paths::bridgeLogPath(), std::ios::app | std::ios::binary);
    if (!f) return;
    f << "[" << timestamp() << "] " << direction << "\n" << pretty << "\n\n";
}

void consolePrint(const char* msg)
{
    if (!msg) return;
    pt(msg);
}

void consolePrint(const std::string_view& msg)
{
    // pt() takes a null-terminated C string. string_view may not be NUL-
    // terminated, so copy.
    std::string s(msg);
    pt(s.c_str());
}

} // namespace ModelerAi::bridge
