// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/diag.h — thread-safe diagnostic log for the remote subsystem.
//
// THE PROBLEM: bridge::consolePrint() uses FlexSim's pt(), which writes
// to the Output Console tree node. That tree node is FlexSim SDK state
// and is only safe to touch from the main thread. The remote server
// (cpp-httplib) hands requests to worker threads; calling pt() from
// there sometimes prints, sometimes silently drops, sometimes corrupts
// the console buffer. We can't rely on it for diagnostics.
//
// THE SOLUTION: diagInfo / diagTrace write through TWO sinks that are
// both thread-safe:
//   1. A mutex-protected log file at %APPDATA%/PRACSIM/ModelerAI/diag.log
//      (always written, never lost across DLL restarts).
//   2. Win32 OutputDebugStringA (visible live in SysInternals DebugView).
// Additionally, if the calling thread is the main thread, the message
// ALSO goes to pt() so it shows in the Output Console immediately.
//
// VERBOSITY:
//   - diagInfo() — always on.
//   - diagTrace() — gated. Off by default. Enable via:
//       (a) Environment variable MODELER_AI_REMOTE_TRACE=1 at startup,
//       (b) Runtime call to setTraceEnabled(true) (used by /remote trace on).
//
// FORMAT: each log line is "<HH:MM:SS.mmm> <tid> <level> <message>\n".

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::diag {

// Always-on. Use for lifecycle, route entry/exit, errors, state changes.
void info(std::string_view msg);

// Gated. Use for noisy per-request details (cookies, headers, queue depths,
// every drain tick). Cheap to call when disabled.
void trace(std::string_view msg);

// True iff trace() output will actually be emitted. Use to skip building
// expensive trace strings when they'd just be discarded.
bool traceEnabled();

// Toggle trace verbosity at runtime. Persists for the process lifetime.
// Returns the previous state. Threadsafe.
bool setTraceEnabled(bool on);

// Path to the diag.log file. Surfaced by /remote diag so the user can
// tail it.
std::string logFilePath();

} // namespace ModelerAi::diag
