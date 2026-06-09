// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// remote/remote_server.h — controlling API for the cpp-httplib server
// that exposes the viewer + bridge over LAN HTTP.
//
// Lifecycle is owned by bootstrap.cpp:
//   - start(port, token) — spins up the server thread
//   - stop()             — gracefully tears it down (joins the thread)
//   - status()           — returns the structured snapshot used by
//                          the /remote status slash command and the
//                          Settings → Remote section
//
// Routes:
//   GET  /              -> serves loadEmbeddedViewer()
//   GET  /api/health    -> { ok, version, server_started_at }
//   GET  /api/poll      -> auth-gated drainSubscriber for the requester
//   POST /api/send      -> auth-gated forward to bridge::handleEnvelope
//   GET  /api/qr_lib.js -> serves the QR JS lib (cached)

#pragma once

#include "remote/qr_payload.h"
#include "share/tunnel/tunnel.h"

#include <string>

namespace ModelerAi::remote {

struct StartResult {
    bool        ok = false;
    int         actual_port = 0;
    std::string error_message;
};

// Idempotent start. Returns ok=true if a server is now running on the
// requested port with the requested token. If a server is ALREADY
// running, returns ok=false with error_message "already_running"
// (caller should call stop() first).
StartResult start(int port, const std::string& token, const std::string& bind_ip /* empty = all */);

// Graceful stop. No-op if not running.
void stop();

bool isRunning();

// Start tunneled mode: spawn cloudflared, wait briefly for its URL,
// then start the local server bound to 127.0.0.1 ONLY. The LAN cannot
// reach the server when tunnel mode is active — cloudflared connects
// loopback; everyone else goes through the *.trycloudflare.com URL.
StartResult startTunneled(tunnel::Mode mode);

// Stop tunneled mode: stop the server, kill cloudflared, rotate the
// token. Idempotent. After this call, both isRunning() and
// tunnel::isRunning() return false.
void stopTunneled();

// Current tunnel mode (None when not tunneled).
tunnel::Mode tunnelMode();

// Public tunnel URL (empty if no tunnel active OR Quick mode hasn't
// scraped the URL yet — caller may want to retry after a short delay).
std::string tunnelPublicUrl();

// Snapshot for /remote status + Settings panel. enabled=false when
// stopped.
RemoteStatusInputs status();

// Kick a specific subscriber — used by the Settings panel Disconnect
// button. Removes from registry; their next poll returns 401 (because
// we re-check that the subscriber still exists at poll time).
void kickClient(const std::string& subscriber_id);

// Rotate token + kick all current clients. Returns the new token.
std::string rotateToken();

// Generate a fresh 24-char base64 token. Public so the slash command
// can use it on initial start.
std::string generateToken();

// Broadcast the current `remote_status` state_update to every
// subscriber. Used by subscriber-lifecycle hooks (new phone polled,
// phone kicked) so the Settings panel updates LIVE instead of only
// after the user manually runs `/remote status`. Throttled internally
// to at most once per 500ms — multiple registrations in fast succession
// coalesce into one push. Safe to call from any thread (including
// cpp-httplib worker threads).
void broadcastRemoteStatus();

} // namespace ModelerAi::remote
