// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bridge/log.h — pretty-print envelope trace logger.
//
// When MODELER_AI_BRIDGE_LOG is set in the process environment, every
// envelope crossing the JS↔DLL boundary is appended (pretty-printed) to
// %APPDATA%/PRACSIM/ModelerAI/bridge.log. No-op when the env var is unset,
// so the cost in production is one std::atomic<bool> read per envelope.

#pragma once

#include <string_view>

namespace ModelerAi::bridge {

// `direction` is "js->dll" or "dll->js". `envelopeJson` is the raw
// envelope JSON string. Safe to call from any thread.
void logEnvelope(std::string_view direction, std::string_view envelopeJson);

// Append a line to FlexSim's Output Console via pt(). Always-on (not
// gated by MODELER_AI_BRIDGE_LOG) — this is for pre-ship runtime
// visibility. Per project memory, every DLL entry point and error
// path should call this. Trim verbosity once we ship.
void consolePrint(const char* msg);
void consolePrint(const std::string_view& msg);

} // namespace ModelerAi::bridge
