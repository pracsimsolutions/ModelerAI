// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"          // pt() — must precede json.h-pulling headers
#include "bridge/diag.h"
#include "mainthread/mainthread.h"
#include "paths.h"

#include <windows.h>         // OutputDebugStringA, GetCurrentThreadId

#include <atomic>
#include <chrono>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>

namespace ModelerAi::diag {

namespace {

std::mutex          g_fileMutex;
std::atomic<bool>   g_traceEnabled{ false };
std::atomic<bool>   g_initialized{ false };

void initOnce()
{
    if (g_initialized.exchange(true)) return;
    const char* env = std::getenv("MODELER_AI_REMOTE_TRACE");
    if (env && env[0] && env[0] != '0') g_traceEnabled.store(true);
}

std::string timestampHHMMSSmmm()
{
    using namespace std::chrono;
    auto now = system_clock::now();
    auto t   = system_clock::to_time_t(now);
    auto ms  = duration_cast<milliseconds>(now.time_since_epoch()) % 1000;
    std::tm local;
    localtime_s(&local, &t);
    std::ostringstream os;
    os << std::put_time(&local, "%H:%M:%S")
       << '.' << std::setw(3) << std::setfill('0') << ms.count();
    return os.str();
}

// Append a single line to the diag log file. Mutex-protected; safe from
// any thread. Failures are silent — diagnostic logging must never throw
// or block, even if the disk is full.
void appendToFile(const std::string& line)
{
    std::lock_guard<std::mutex> lk(g_fileMutex);
    std::string path = paths::diagLogPath();
    if (path.empty()) return;
    try {
        std::ofstream f(path, std::ios::app | std::ios::binary);
        if (!f) return;
        f << line;
    } catch (...) { /* swallow */ }
}

// Core emit. `level` is "INFO" or "TRACE". Builds the formatted line
// once and routes it to all three sinks. The main-thread sink (pt) is
// best-effort: if we're not on the main thread, marshal a pt() call
// onto the mainthread queue so it eventually surfaces in the Output
// Console.
void emit(const char* level, std::string_view msg)
{
    initOnce();
    DWORD tid = GetCurrentThreadId();
    std::ostringstream os;
    os << '[' << timestampHHMMSSmmm() << "] "
       << "tid=" << tid << ' '
       << level << ' ';
    os.write(msg.data(), (std::streamsize)msg.size());
    if (msg.empty() || msg.back() != '\n') os << '\n';
    std::string line = os.str();

    appendToFile(line);
    OutputDebugStringA(line.c_str());

    if (mainthread::detail::isOnMainThread()) {
        pt(line.c_str());
    } else {
        // Best-effort: enqueue a console print so it shows up later when
        // the queue drains. Capture by value — the lambda runs after
        // this stack frame is gone.
        std::string copy = line;
        mainthread::detail::enqueue([copy = std::move(copy)]() {
            pt(copy.c_str());
        });
    }
}

} // namespace

void info(std::string_view msg)
{
    emit("INFO", msg);
}

void trace(std::string_view msg)
{
    initOnce();
    if (!g_traceEnabled.load(std::memory_order_relaxed)) return;
    emit("TRACE", msg);
}

bool traceEnabled()
{
    initOnce();
    return g_traceEnabled.load(std::memory_order_relaxed);
}

bool setTraceEnabled(bool on)
{
    initOnce();
    return g_traceEnabled.exchange(on);
}

std::string logFilePath()
{
    return paths::diagLogPath();
}

} // namespace ModelerAi::diag
