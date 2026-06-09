// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// share/tunnel/tunnel.h — cloudflared subprocess management.
//
// Owns one cloudflared.exe child process and its lifecycle. Uses a
// Windows Job Object with JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE so the
// OS reaps the child even if our DLL host crashes — no zombie tunnels
// exposing localhost to the public internet.

#pragma once

#include <string>

namespace ModelerAi::tunnel {

enum class Mode { None, Quick, Named };

struct StartResult {
    bool        ok = false;
    std::string public_url;     // *.trycloudflare.com (Quick) or configured hostname (Named)
    std::string error_message;
};

// Spawn cloudflared pointed at http://127.0.0.1:<localPort>.
// For Mode::Named, requires CF_TUNNEL_TOKEN in environment OR
// share/.env (loaded at start()).
StartResult start(Mode mode, int localPort);

// Idempotent. Sends Ctrl-Break, waits 5s, then TerminateProcess.
void stop();

bool isRunning();

Mode currentMode();

// Empty string if not running or URL not yet captured (Quick mode
// takes a few seconds after start() for cloudflared to emit it).
std::string publicUrl();

} // namespace ModelerAi::tunnel
