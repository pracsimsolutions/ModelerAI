// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "agent/slash_remote.h"
#include "agent/agent.h"
#include "bridge/bridge.h"
#include "bridge/diag.h"
#include "bridge/subscriber.h"
#include "mainthread/mainthread.h"
#include "remote/remote_server.h"
#include "remote/qr_payload.h"
#include "share/tunnel/tunnel.h"
#include "third_party/json.h"

#include <chrono>
#include <climits>
#include <sstream>

namespace ModelerAi::agent {

namespace {

// Push a system assistant message — same path the existing slash
// commands use — with a tag that makes the viewer render the QR
// bubble inline.
void pushSystemBubble(const std::string& turnId,
                       const std::string& text,
                       bool isRemoteStatusBubble)
{
    nlohmann::json env;
    env["t"]  = "assistant_chunk";
    env["id"] = turnId;
    nlohmann::json p;
    p["text"]                    = text;
    p["role_hint"]               = "system";
    if (isRemoteStatusBubble) p["remote_status_bubble"] = true;
    env["p"] = std::move(p);
    bridge::pushEvent(env.dump());

    nlohmann::json done;
    done["t"]  = "assistant_done";
    done["id"] = turnId;
    done["p"]  = nlohmann::json::object();
    bridge::pushEvent(done.dump());
}

// Build + push the remote_status state_update.
void pushRemoteStatus(const std::string& turnId)
{
    auto in = remote::status();
    // Fill tunnel fields.
    in.tunnel_public_url = remote::tunnelPublicUrl();
    switch (remote::tunnelMode()) {
        case tunnel::Mode::None:  in.tunnel_mode = "none";  break;
        case tunnel::Mode::Quick: in.tunnel_mode = "quick"; break;
        case tunnel::Mode::Named: in.tunnel_mode = "named"; break;
    }
    // Fill clients from the subscriber snapshot.
    auto subs = bridge::snapshotSubscribers();
    auto now = std::chrono::steady_clock::now();
    for (const auto& s : subs) {
        if (s.id == bridge::kLocalFlexsimSubscriberId) continue;
        remote::ConnectedClient cc;
        cc.id                    = s.id;
        cc.ipAddress             = s.client_ip;
        cc.user_agent            = s.user_agent;
        cc.last_seen_seconds_ago = (int)std::chrono::duration_cast<std::chrono::seconds>(
                                       now - s.last_seen).count();
        in.clients.push_back(std::move(cc));
    }
    nlohmann::json v = remote::buildRemoteStatusJson(in);

    nlohmann::json env;
    env["t"]  = "state_update";
    env["id"] = turnId;
    nlohmann::json p;
    p["key"]   = "remote_status";
    p["value"] = std::move(v);
    env["p"]   = std::move(p);
    bridge::pushEvent(env.dump());
}

// Parse `port=N` and `bind=X.Y.Z.W` from the rest of args. Returns
// defaults if absent.
struct ParsedOpts {
    int         port = 12350;
    std::string bind;
};
ParsedOpts parseOnArgs(std::string_view rest)
{
    ParsedOpts out;
    std::string s(rest);
    size_t pos = 0;
    while (pos < s.size()) {
        size_t sp = s.find(' ', pos);
        std::string tok = s.substr(pos, sp - pos);
        if (auto eq = tok.find('='); eq != std::string::npos) {
            std::string k = tok.substr(0, eq);
            std::string v = tok.substr(eq + 1);
            if (k == "port") {
                try { out.port = std::stoi(v); } catch (...) {}
            } else if (k == "bind") {
                out.bind = v;
            }
        }
        if (sp == std::string::npos) break;
        pos = sp + 1;
    }
    return out;
}

void handleOn(const std::string& turnId, std::string_view rest)
{
    if (remote::isRunning()) {
        pushSystemBubble(turnId,
            "Remote server already running. Use `/remote off` first, "
            "or `/remote rotate` to refresh the token.", false);
        pushRemoteStatus(turnId);
        return;
    }
    auto opts  = parseOnArgs(rest);
    auto token = remote::generateToken();
    auto r     = remote::start(opts.port, token, opts.bind);
    if (!r.ok) {
        pushSystemBubble(turnId,
            "Failed to start remote server: " + r.error_message + ".", false);
        return;
    }
    pushSystemBubble(turnId,
        "Remote server enabled. Scan the QR or visit the URL on your phone.",
        true);
    pushRemoteStatus(turnId);
}

void handleOff(const std::string& turnId)
{
    if (!remote::isRunning()) {
        pushSystemBubble(turnId, "Remote server is not running.", false);
        pushRemoteStatus(turnId);
        return;
    }
    remote::stop();
    pushSystemBubble(turnId, "Remote server stopped.", false);
    pushRemoteStatus(turnId);
}

void handleStatus(const std::string& turnId)
{
    if (!remote::isRunning()) {
        pushSystemBubble(turnId,
            "Remote server is OFF. Use `/remote on` to start.", false);
    } else {
        pushSystemBubble(turnId, "Remote server status:", true);
    }
    pushRemoteStatus(turnId);
}

// Dump a snapshot of the remote-related runtime state into a chat bubble
// so the user can copy it back to me for diagnosis. Everything in here is
// cheap to compute — pure registry reads + thread-id lookups, no SDK calls.
void handleDiag(const std::string& turnId)
{
    std::ostringstream out;
    out << "**Remote diagnostics**\n\n";

    // --- Server state ---
    auto in = remote::status();
    out << "Server running: " << (in.enabled ? "YES" : "no") << "\n";
    if (in.enabled) {
        out << "Port: " << in.port << "  uptime: " << in.uptime_seconds << "s\n";
        out << "Token (short): " << (in.token.size() > 10
                                       ? in.token.substr(0,4) + "..." + in.token.substr(in.token.size()-4)
                                       : in.token) << "\n";
    }

    // --- Main-thread queue ---
    out << "\nMain-thread captured tid: " << mainthread::capturedMainThreadId() << "\n";
    out << "Main-thread pending tasks: " << mainthread::pendingTaskCount();
    if (mainthread::pendingTaskCount() > 0) {
        out << "  <-- BACKLOG (drainQueue not firing fast enough)";
    }
    out << "\n";
    long long ms = mainthread::msSinceLastDrain();
    out << "Last bridgePoll/drainQueue: ";
    if (ms == LLONG_MAX) out << "NEVER";
    else                  out << ms << "ms ago";
    if (ms != LLONG_MAX && ms > 2000) {
        out << "  <-- STALE (JS heartbeat appears dead)";
    }
    out << "\n";

    // --- Subscribers ---
    auto subs = bridge::snapshotSubscribers();
    auto now  = std::chrono::steady_clock::now();
    out << "\nSubscribers: " << subs.size() << "\n";
    for (const auto& s : subs) {
        int ago = (int)std::chrono::duration_cast<std::chrono::seconds>(
                     now - s.last_seen).count();
        int depth = bridge::subscriberQueueDepth(s.id);
        out << "  - id=" << s.id
            << "  last_seen=" << ago << "s ago"
            << "  outbound_queue=" << depth;
        if (!s.client_ip.empty())  out << "  ip=" << s.client_ip;
        if (!s.user_agent.empty()) out << "  ua='" << s.user_agent.substr(0, 40) << "'";
        out << "\n";

        // Ring buffers: most recent envelopes in each direction. Empty
        // sections are omitted so a brand-new subscriber doesn't pad the
        // bubble with two "(none)" lines.
        if (!s.recent_received.empty()) {
            out << "    recent recv (from this client):\n";
            for (const auto& line : s.recent_received) {
                out << "      " << line << "\n";
            }
        }
        if (!s.recent_sent.empty()) {
            out << "    recent sent (to this client):\n";
            for (const auto& line : s.recent_sent) {
                out << "      " << line << "\n";
            }
        }
    }

    // --- Diagnostic log info ---
    out << "\nDiag log: `" << diag::logFilePath() << "`\n";
    out << "Trace verbosity: " << (diag::traceEnabled() ? "ON" : "off")
        << "  (toggle: `/remote trace on` / `/remote trace off`)\n";
    out << "\nTo capture a run, in PowerShell:\n"
        << "  `Get-Content -Wait \"" << diag::logFilePath() << "\"`\n"
        << "Or open SysInternals DebugView for live `OutputDebugString` mirror.";

    pushSystemBubble(turnId, out.str(), false);
}

void handleRotate(const std::string& turnId)
{
    if (!remote::isRunning()) {
        pushSystemBubble(turnId,
            "Remote server isn't running — nothing to rotate.", false);
        return;
    }
    // C3 — Use rotateToken() which preserves the existing port and bind IP.
    // The old code passed port=12350 and bind="" (defaulting to 0.0.0.0),
    // which broke the loopback-only invariant when running in tunneled mode.
    auto newToken = remote::rotateToken();
    if (newToken.empty()) {
        pushSystemBubble(turnId,
            "Token rotation failed.", false);
        return;
    }
    pushSystemBubble(turnId,
        "Token rotated. Previous clients disconnected — rescan to reconnect.",
        true);
    pushRemoteStatus(turnId);
}

// Toggle TRACE-level diagnostic output at runtime. INFO is always on;
// TRACE is the noisy per-request stuff. Three call shapes:
//   /remote trace        — TOGGLE (no second-level arg needed, plays
//                          nicely with the slash-autocomplete UI which
//                          only knows about the first-level choice).
//   /remote trace on     — explicit on.
//   /remote trace off    — explicit off.
// Any other arg gets a usage hint.
void handleTrace(const std::string& turnId, std::string_view rest)
{
    std::string sub(rest);
    while (!sub.empty() && sub.front() == ' ') sub.erase(0, 1);
    auto sp = sub.find(' ');
    if (sp != std::string::npos) sub.resize(sp);

    if (sub == "on") {
        diag::setTraceEnabled(true);
    } else if (sub == "off") {
        diag::setTraceEnabled(false);
    } else if (sub.empty()) {
        // No arg from autocomplete — toggle.
        diag::setTraceEnabled(!diag::traceEnabled());
    } else {
        pushSystemBubble(turnId,
            "Usage: `/remote trace` (toggle), `/remote trace on`, or "
            "`/remote trace off`. Currently: trace "
            + std::string(diag::traceEnabled() ? "ON" : "OFF")
            + ". Log: `" + diag::logFilePath() + "`",
            false);
        return;
    }

    bool now = diag::traceEnabled();
    std::string msg = "Diagnostic trace is now **"
                      + std::string(now ? "ON" : "OFF")
                      + "**. Log file: `" + diag::logFilePath() + "`\n"
                      + "Tail it with PowerShell: `Get-Content -Wait \""
                      + diag::logFilePath() + "\"`";
    pushSystemBubble(turnId, msg, false);
}

} // namespace

bool dispatchRemote(Agent* /*agent*/,
                    const std::string& turnId,
                    std::string_view args)
{
    // args is everything after "/remote ". Pull the first token.
    std::string a(args);
    while (!a.empty() && a.front() == ' ') a.erase(0, 1);
    auto sp = a.find(' ');
    std::string sub = (sp == std::string::npos) ? a : a.substr(0, sp);
    std::string rest = (sp == std::string::npos) ? "" : a.substr(sp + 1);

    if      (sub == "on")     { handleOn(turnId, rest); return true; }
    else if (sub == "off")    { handleOff(turnId); return true; }
    else if (sub == "status") { handleStatus(turnId); return true; }
    else if (sub == "rotate") { handleRotate(turnId); return true; }
    else if (sub == "diag")   { handleDiag(turnId); return true; }
    else if (sub == "trace")  { handleTrace(turnId, rest); return true; }
    else if (sub == "tunnel") {
        // `/remote tunnel` is a toggle: off → start (Quick by default),
        // on → stop. Mode can be selected explicitly via
        // `/remote tunnel mode=named` — if a tunnel is already running
        // when a mode is specified, we stop+restart with the new mode.
        std::string rest2(rest);
        while (!rest2.empty() && rest2.front() == ' ') rest2.erase(0, 1);

        // Detect explicit mode= arg (overrides toggle semantics by
        // forcing a start in the named mode; restarts if already up).
        tunnel::Mode requestedMode = tunnel::Mode::Quick;
        bool modeExplicit = false;
        if (rest2.find("mode=named") != std::string::npos) {
            requestedMode = tunnel::Mode::Named;
            modeExplicit  = true;
        } else if (rest2.find("mode=quick") != std::string::npos) {
            requestedMode = tunnel::Mode::Quick;
            modeExplicit  = true;
        }

        // If tunnel is currently up:
        //   - No mode= arg → toggle off
        //   - mode= arg AND mode differs from current → stop then restart
        //   - mode= arg AND mode matches current → no-op (announce)
        bool currentlyTunneled = (remote::tunnelMode() != tunnel::Mode::None);
        if (currentlyTunneled && !modeExplicit) {
            remote::stopTunneled();
            pushSystemBubble(turnId, "Tunnel stopped.", false);
            pushRemoteStatus(turnId);
            return true;
        }
        if (currentlyTunneled && modeExplicit
            && remote::tunnelMode() == requestedMode) {
            pushSystemBubble(turnId,
                std::string("Tunnel already running (")
                + (requestedMode == tunnel::Mode::Named ? "named" : "quick")
                + "). Run `/remote tunnel` to stop.", false);
            pushRemoteStatus(turnId);
            return true;
        }
        if (currentlyTunneled && modeExplicit) {
            // Mode switch: stop the current tunnel before starting the new one.
            remote::stopTunneled();
        }

        // If LAN-only server is running (no tunnel), refuse — user has to
        // explicitly stop it first to avoid surprise port rebinding.
        if (remote::isRunning()) {
            pushSystemBubble(turnId,
                "LAN server is running. Use `/remote off` first, then "
                "`/remote tunnel` to start the tunnel.", false);
            pushRemoteStatus(turnId);
            return true;
        }

        auto r = remote::startTunneled(requestedMode);
        if (!r.ok) {
            pushSystemBubble(turnId,
                "Tunnel start failed: " + r.error_message + ".", false);
            return true;
        }
        pushSystemBubble(turnId,
            std::string("Cloudflare Tunnel active (")
            + (requestedMode == tunnel::Mode::Named ? "named, stable URL" : "quick, ephemeral URL")
            + "). Scan the QR or visit the URL on any device — works across networks. "
              "Run `/remote tunnel` again to stop.",
            true);
        pushRemoteStatus(turnId);
        return true;
    }
    return false;
}

} // namespace ModelerAi::agent
