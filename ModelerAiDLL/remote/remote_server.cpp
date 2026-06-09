// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

// IMPORTANT: cpp-httplib uses `#ifdef CPPHTTPLIB_OPENSSL_SUPPORT` (not
// `#if CPPHTTPLIB_OPENSSL_SUPPORT == 1`) to gate OpenSSL inclusion. So
// `#define CPPHTTPLIB_OPENSSL_SUPPORT 0` would actually OPT IN to OpenSSL.
// The correct way to opt OUT is to NOT define the macro at all — that's
// our v1 posture (plain HTTP, LAN-trust).
#include "remote/remote_server.h"
#include "remote/asset_loader.h"
#include "remote/lan_iface.h"
#include "remote/qr_payload.h"
#include "share/tunnel/tunnel.h"
#include "third_party/cpp-httplib/httplib.h"
#include "bridge/log.h"
#include "bridge/diag.h"
#include "bridge/subscriber.h"
#include "bridge/bridge.h"
#include "mainthread/mainthread.h"
#include "paths.h"

#include <windows.h>   // GetCurrentThreadId for /api/send trace
#include <bcrypt.h>

#include <atomic>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <mutex>
#include <random>
#include <sstream>
#include <thread>
#include <unordered_map>

namespace ModelerAi::remote {

namespace {

std::mutex                              g_mu;
std::unique_ptr<httplib::Server>        g_server;
std::unique_ptr<std::thread>            g_serverThread;
std::atomic<bool>                       g_running{false};
std::string                             g_token;
int                                     g_port = 0;
std::string                             g_bindIp;
std::chrono::steady_clock::time_point   g_startTime;

// Returns the bearer token from the Authorization header, OR the
// `?token=...` query string, OR empty string if neither.
std::string extractToken(const httplib::Request& req)
{
    auto auth = req.get_header_value("Authorization");
    const std::string prefix = "Bearer ";
    if (auth.size() > prefix.size()
        && auth.compare(0, prefix.size(), prefix) == 0) {
        return auth.substr(prefix.size());
    }
    if (req.has_param("token")) return req.get_param_value("token");
    return "";
}

// Resolve the real client IP. cloudflared puts itself between us and the
// world, so req.remote_addr is always 127.0.0.1 in tunnel mode. The
// real client IP rides in CF-Connecting-IP. Falls back to remote_addr
// for LAN/direct mode.
std::string clientIp(const httplib::Request& req)
{
    auto cf = req.get_header_value("CF-Connecting-IP");
    if (!cf.empty()) return cf;
    return req.remote_addr;
}

bool authOk(const httplib::Request& req)
{
    auto t = extractToken(req);
    std::lock_guard<std::mutex> lk(g_mu);
    return !g_token.empty() && t == g_token;
}

void writeAuthError(httplib::Response& res)
{
    res.status = 401;
    res.set_content("{\"error\":\"token_mismatch\","
                    "\"message\":\"Token missing or invalid.\"}",
                    "application/json");
}

// Reads the mraisid cookie. Empty if absent.
std::string readCookie(const httplib::Request& req, const std::string& name)
{
    auto cookie = req.get_header_value("Cookie");
    // Cookie header is "k1=v1; k2=v2" — naive parse is sufficient.
    size_t pos = 0;
    while (pos < cookie.size()) {
        size_t eq    = cookie.find('=', pos);
        size_t semi  = cookie.find(';', pos);
        if (eq == std::string::npos) break;
        std::string k = cookie.substr(pos, eq - pos);
        // Trim leading spaces from k
        size_t firstNonSpace = k.find_first_not_of(' ');
        if (firstNonSpace != std::string::npos) k = k.substr(firstNonSpace);
        std::string v = (semi == std::string::npos)
                        ? cookie.substr(eq + 1)
                        : cookie.substr(eq + 1, semi - eq - 1);
        if (k == name) return v;
        if (semi == std::string::npos) break;
        pos = semi + 1;
    }
    return "";
}

// Resolve the client's subscriber id. Priority:
//   1. X-Mraisid header — set by JS from localStorage. Bypasses the
//      browser cookie jar entirely, which is unreliable on mobile
//      Safari over plain-HTTP + IP-address origins.
//   2. mraisid cookie — kept as a fallback for clients that don't run
//      the JS bootstrap (curl, health probes, future server-to-server).
//
// Empty string means "no identity provided"; the route handler decides
// whether to issue a fresh one.
std::string resolveSid(const httplib::Request& req)
{
    auto hdr = req.get_header_value("X-Mraisid");
    if (!hdr.empty()) return hdr;
    return readCookie(req, "mraisid");
}

struct RateState {
    std::chrono::steady_clock::time_point window_start;
    int                                   count = 0;
};
std::mutex                                       g_rateMu;
std::unordered_map<std::string, RateState>       g_rate;

bool checkSendRate(const std::string& sid)
{
    constexpr int kMaxPerWindow = 10;
    constexpr auto kWindow      = std::chrono::seconds(1);
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lk(g_rateMu);
    auto& st = g_rate[sid];
    if (now - st.window_start > kWindow) {
        st.window_start = now;
        st.count        = 0;
    }
    if (st.count >= kMaxPerWindow) return false;
    ++st.count;
    return true;
}

// Per-IP auth-failure tracking. Without this, an internet attacker can
// hammer /api/poll?token=X at any rate trying tokens. We cap auth failures
// per source IP at 10/min, then 5-min ban (429). Keyed by clientIp() so
// per-guest accounting survives the cloudflared loopback termination.
struct AuthFailState {
    std::chrono::steady_clock::time_point window_start;
    int   count = 0;
    std::chrono::steady_clock::time_point banned_until;  // zero = not banned
};
std::mutex                                       g_authFailMu;
std::unordered_map<std::string, AuthFailState>   g_authFail;

// Returns true if the IP is currently banned (caller should 429 immediately).
bool isAuthBanned(const std::string& ip)
{
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lk(g_authFailMu);
    auto it = g_authFail.find(ip);
    if (it == g_authFail.end()) return false;
    return now < it->second.banned_until;
}

// Record one auth failure; promotes to a 5-minute ban after 10 failures
// in 60 seconds. Bookkeeping is per-IP; tunneled clients all share the
// cloudflared loopback IP from req.remote_addr but we use CF-Connecting-IP
// via clientIp() so per-guest accounting works in tunnel mode.
void recordAuthFail(const std::string& ip)
{
    constexpr int  kMaxFailsPerWindow = 10;
    constexpr auto kWindow            = std::chrono::seconds(60);
    constexpr auto kBan               = std::chrono::minutes(5);
    auto now = std::chrono::steady_clock::now();
    std::lock_guard<std::mutex> lk(g_authFailMu);
    auto& st = g_authFail[ip];
    if (now - st.window_start > kWindow) {
        st.window_start = now;
        st.count        = 0;
    }
    ++st.count;
    if (st.count >= kMaxFailsPerWindow) {
        st.banned_until = now + kBan;
        diag::info("auth-fail ban: ip=" + ip
                   + " for " + std::to_string(std::chrono::duration_cast<std::chrono::seconds>(kBan).count())
                   + "s");
    }
}

} // namespace

std::string generateToken()
{
    // CSPRNG so tunnel-mode tokens (publicly-visible URL) aren't predictable.
    uint8_t bytes[18];
    NTSTATUS s = BCryptGenRandom(nullptr, bytes, sizeof(bytes),
                                  BCRYPT_USE_SYSTEM_PREFERRED_RNG);
    if (s != 0) {
        // Defense in depth: if CSPRNG fails (shouldn't), refuse to start.
        // The caller (slash::on, settings panel) will surface the error.
        return "";
    }
    static const char kAlphabet[] =
        "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string out;
    out.reserve(24);
    for (int i = 0; i < 18; i += 3) {
        uint32_t v = (bytes[i] << 16) | (bytes[i+1] << 8) | bytes[i+2];
        out += kAlphabet[(v >> 18) & 0x3F];
        out += kAlphabet[(v >> 12) & 0x3F];
        out += kAlphabet[(v >> 6)  & 0x3F];
        out += kAlphabet[ v        & 0x3F];
    }
    return out;
}

StartResult start(int port, const std::string& token, const std::string& bind_ip)
{
    StartResult r;
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_running.load()) {
        r.error_message = "already_running";
        return r;
    }
    if (token.empty()) {
        r.error_message = "rng_failed";
        return r;
    }
    g_token     = token;
    g_port      = port;
    g_bindIp    = bind_ip.empty() ? std::string("0.0.0.0") : bind_ip;
    g_startTime = std::chrono::steady_clock::now();
    g_server    = std::make_unique<httplib::Server>();
    g_server->set_payload_max_length(64 * 1024);

    // Health route — no auth.
    g_server->Get("/api/health", [](const httplib::Request&, httplib::Response& res) {
        res.set_content("{\"ok\":true,\"version\":\"v1\"}", "application/json");
    });

    g_server->Get("/api/poll", [](const httplib::Request& req, httplib::Response& res) {
        auto ip = clientIp(req);
        if (isAuthBanned(ip)) {
            res.status = 429;
            res.set_content("{\"error\":\"rate_limited\",\"message\":\"Too many auth failures; try again later.\"}",
                            "application/json");
            return;
        }
        if (!authOk(req)) {
            recordAuthFail(ip);
            return writeAuthError(res);
        }
        std::string sid = resolveSid(req);
        bool        registered_now = false;
        if (sid.empty()) {
            // Client offered nothing — issue a new one. Will be returned
            // both as a cookie (for any client that honors them) AND in
            // X-Mraisid response header (so JS can pin it into localStorage
            // and bypass cookies entirely thereafter).
            auto remoteIp = clientIp(req);
            auto ua       = req.get_header_value("User-Agent");
            sid = bridge::registerRemoteSubscriber(remoteIp, ua);
            registered_now = true;
        } else if (!bridge::hasSubscriber(sid)) {
            // Client offered a sid we don't know — adopt it. Lets the JS
            // generate its own (UUID-ish) and present it on first request,
            // so the same id survives server restarts AND browser refreshes
            // without us having to issue + persist anything.
            auto remoteIp = clientIp(req);
            auto ua       = req.get_header_value("User-Agent");
            bridge::adoptRemoteSubscriber(sid, remoteIp, ua);
            registered_now = true;
        } else {
            bridge::touchSubscriber(sid);
        }
        if (registered_now) {
            res.set_header("Set-Cookie",
                "mraisid=" + sid + "; Path=/; HttpOnly; SameSite=Strict");
            diag::info("/api/poll: registered sid=" + sid + " ip=" + req.remote_addr);
            // Tell every connected viewer (settings panels especially)
            // that the subscriber set just changed. Throttled internally.
            broadcastRemoteStatus();
        }
        // Always echo the sid in a response header so the JS client can
        // pin it on first contact even if cookies are silently dropped.
        res.set_header("X-Mraisid", sid);
        res.set_content(bridge::drainSubscriber(sid), "application/json");
    });

    g_server->Post("/api/send", [](const httplib::Request& req, httplib::Response& res) {
        DWORD workerTid = GetCurrentThreadId();
        // Promoted from trace to info: "did the phone's send even reach
        // the server?" is the single most common diagnostic question and
        // not worth gating. One log line per phone send is acceptable.
        diag::info("/api/send: entry worker_tid=" + std::to_string(workerTid)
                   + " body_len=" + std::to_string(req.body.size())
                   + " remote_ip=" + req.remote_addr);

        auto ip = clientIp(req);
        if (isAuthBanned(ip)) {
            res.status = 429;
            res.set_content("{\"error\":\"rate_limited\",\"message\":\"Too many auth failures; try again later.\"}",
                            "application/json");
            return;
        }
        if (!authOk(req)) {
            recordAuthFail(ip);
            diag::info("/api/send: AUTH_FAIL ip=" + req.remote_addr);
            return writeAuthError(res);
        }
        std::string sid = resolveSid(req);
        if (sid.empty()) {
            diag::info("/api/send: NO_SID (neither X-Mraisid header nor cookie present) ip=" + req.remote_addr);
            res.status = 401;
            res.set_content("{\"error\":\"no_subscriber\","
                            "\"message\":\"Poll first to register.\"}",
                            "application/json");
            return;
        }
        if (!bridge::hasSubscriber(sid)) {
            // Adopt — client knows its sid (from localStorage), server lost
            // it (restart). Same accept-the-id-the-client-claims policy as
            // /api/poll. Token-bearer auth gates who can claim ids at all.
            auto ua = req.get_header_value("User-Agent");
            bridge::adoptRemoteSubscriber(sid, clientIp(req), ua);
            diag::info("/api/send: adopted unknown sid=" + sid);
        }
        bridge::touchSubscriber(sid);
        if (!checkSendRate(sid)) {
            diag::info("/api/send: RATE_LIMITED sid=" + sid);
            res.status = 429;
            res.set_content("{\"error\":\"rate_limited\","
                            "\"message\":\"More than 10 sends/sec.\"}",
                            "application/json");
            return;
        }
        // handleEnvelope assumes it's running on the FlexSim main thread
        // (it calls tree::condenseActiveModel(), settings reads, agent
        // start — all of which touch FlexSim SDK state that's NOT safe
        // off-thread). cpp-httplib hands us a worker thread, so marshal
        // the call through the existing mainthread queue. Blocks until
        // the main thread drains the task and returns the ack string.
        diag::trace("/api/send: enqueuing runAndWait sid=" + sid);
        auto t0 = std::chrono::steady_clock::now();
        std::string body = req.body;
        std::string sidCopy = sid;
        std::string reply = ModelerAi::mainthread::runAndWait(
            [body = std::move(body), sidCopy = std::move(sidCopy), workerTid]() {
                diag::trace("/api/send: task running on main_tid="
                            + std::to_string(GetCurrentThreadId())
                            + " (worker was tid=" + std::to_string(workerTid) + ")");
                auto r = std::string(bridge::handleEnvelope(body, sidCopy));
                diag::trace("/api/send: task done reply='" + r + "'");
                return r;
            });
        auto elapsedMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                            std::chrono::steady_clock::now() - t0).count();
        // Promoted: "how long did it take + did the lambda actually return"
        // is critical for diagnosing main-thread queue drain issues.
        diag::info("/api/send: runAndWait returned after " + std::to_string(elapsedMs)
                   + "ms reply_len=" + std::to_string(reply.size()));
        res.set_content(reply, "text/plain");
    });

    g_server->Get("/", [](const httplib::Request& /*req*/, httplib::Response& res) {
        const std::string& html = loadEmbeddedViewer();
        if (html.empty()) {
            res.status = 500;
            res.set_content("Viewer asset not found. Check install dir.", "text/plain");
            return;
        }
        res.set_header("Cache-Control",          "no-store, no-cache, must-revalidate, private");
        res.set_header("Pragma",                 "no-cache");
        res.set_header("X-Content-Type-Options", "nosniff");
        res.set_header("Referrer-Policy",        "no-referrer");
        res.set_content(html, "text/html; charset=utf-8");
    });

    // Vendored qrcode-generator library. The local CEF viewer gets the
    // library inlined into index.html (the embed pipeline only handles
    // index.html), but remote browsers fetching the served HTML need the
    // file as a separate asset. Cached on first read like the viewer HTML.
    g_server->Get("/vendor/qrcode-generator.js", [](const httplib::Request&, httplib::Response& res) {
        static std::string cached;
        static bool        loaded = false;
        if (!loaded) {
            loaded = true;
            std::string installDir = paths::moduleInstallDir();
            if (!installDir.empty()) {
                std::filesystem::path p = std::filesystem::path(installDir)
                                           / "webview" / "vendor" / "qrcode-generator.js";
                std::ifstream f(p, std::ios::in | std::ios::binary);
                if (f.is_open()) {
                    std::ostringstream buf; buf << f.rdbuf();
                    cached = buf.str();
                }
            }
        }
        res.set_content(cached, "application/javascript");
    });

    // 404 handler (so unmatched routes don't hang).
    g_server->set_error_handler([](const httplib::Request&, httplib::Response& res) {
        if (res.status == 404) {
            res.set_content("{\"error\":\"not_found\"}", "application/json");
        }
    });

    // Bind first so we can fail synchronously if the port is in use.
    if (!g_server->bind_to_port(g_bindIp.c_str(), port)) {
        g_server.reset();
        r.error_message = "bind_failed";
        return r;
    }
    r.actual_port = port;

    g_serverThread = std::make_unique<std::thread>([]() {
        // listen_after_bind() blocks until stop().
        if (g_server) g_server->listen_after_bind();
    });
    g_running.store(true);
    r.ok = true;
    diag::info("remote server listening on " + g_bindIp + ":" + std::to_string(port)
               + " (diag.log=" + diag::logFilePath() + ", trace="
               + (diag::traceEnabled() ? "on" : "off") + ")");
    return r;
}

void stop()
{
    std::unique_ptr<httplib::Server>     server;
    std::unique_ptr<std::thread>         thr;
    {
        std::lock_guard<std::mutex> lk(g_mu);
        if (!g_running.load()) return;
        g_running.store(false);
        server = std::move(g_server);
        thr    = std::move(g_serverThread);
    }
    if (server) server->stop();
    if (thr && thr->joinable()) thr->join();
    diag::info("remote server stopped");
}

bool isRunning() { return g_running.load(); }

RemoteStatusInputs status()
{
    RemoteStatusInputs in;
    std::lock_guard<std::mutex> lk(g_mu);
    in.enabled = g_running.load();
    in.port    = g_port;
    in.token   = g_token;
    if (in.enabled) {
        in.interfaces      = detectLanInterfaces();
        in.uptime_seconds  = (long long)std::chrono::duration_cast<std::chrono::seconds>(
                                std::chrono::steady_clock::now() - g_startTime).count();
        // clients filled by caller (needs bridge::snapshotSubscribers())
    }
    return in;
}

void kickClient(const std::string& subscriber_id)
{
    bridge::removeSubscriber(subscriber_id);
    diag::info("kickClient: removed sid=" + subscriber_id);
    broadcastRemoteStatus();
}

std::string rotateToken()
{
    if (!isRunning()) return "";
    int   savedPort = 0;
    std::string savedBind;
    {
        std::lock_guard<std::mutex> lk(g_mu);
        savedPort = g_port;
        savedBind = g_bindIp;
    }
    stop();
    auto t = generateToken();
    auto r = start(savedPort, t, savedBind);
    return r.ok ? t : "";
}

// Throttle state for broadcastRemoteStatus. Worker threads call this
// when a phone connects, and many phones rejoining at once could
// otherwise spam the queue. Atomic so we don't need a lock.
namespace {
    std::atomic<long long> g_lastBroadcastMs{ 0 };
    constexpr long long    kBroadcastThrottleMs = 500;
}

void broadcastRemoteStatus()
{
    long long nowMs = std::chrono::duration_cast<std::chrono::milliseconds>(
                          std::chrono::steady_clock::now().time_since_epoch()).count();
    long long last = g_lastBroadcastMs.load();
    // CAS so two callers don't both pass the gate. Loser just exits.
    if (nowMs - last < kBroadcastThrottleMs) return;
    if (!g_lastBroadcastMs.compare_exchange_strong(last, nowMs)) return;
    bridge::pushEvent(buildRemoteStatusEnvelopeJson("remote-status-lifecycle"));
}

StartResult startTunneled(tunnel::Mode mode)
{
    StartResult r;

    // 1. Pick a port — use the same default as LAN mode for now (12350).
    //    The settings UI in a future task will let the user override.
    constexpr int kDefaultPort = 12350;
    int port = kDefaultPort;

    // 2. Start cloudflared FIRST so we have a URL to publish before
    //    accepting traffic. Tunnels can take 1–10s to register with
    //    the edge.
    auto t = tunnel::start(mode, port);
    if (!t.ok) {
        r.error_message = "tunnel_start_failed: " + t.error_message;
        return r;
    }

    // 3. For Quick mode, poll publicUrl() with a 15s timeout. For
    //    Named mode, the URL is the user-configured hostname — we
    //    leave that hookup to Task 2.5 (read from share/cloudflared/config.yml).
    if (mode == tunnel::Mode::Quick) {
        auto deadline = std::chrono::steady_clock::now()
                       + std::chrono::seconds(15);
        while (tunnel::publicUrl().empty()
            && std::chrono::steady_clock::now() < deadline) {
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
        }
        if (tunnel::publicUrl().empty()) {
            tunnel::stop();
            r.error_message = "tunnel_url_timeout: cloudflared did not "
                              "publish a URL within 15s";
            return r;
        }
    }

    // 4. Start the local server bound to 127.0.0.1 ONLY. Critical
    //    security boundary: cloudflared connects loopback; LAN cannot
    //    reach the server during tunnel mode.
    auto token = generateToken();
    if (token.empty()) {
        tunnel::stop();
        r.error_message = "rng_failed";
        return r;
    }
    auto sr = start(port, token, "127.0.0.1");
    if (!sr.ok) {
        tunnel::stop();
        return sr;
    }

    r.ok = true;
    r.actual_port = port;
    return r;
}

void stopTunneled()
{
    stop();
    tunnel::stop();
}

tunnel::Mode tunnelMode() { return tunnel::currentMode(); }
std::string  tunnelPublicUrl() { return tunnel::publicUrl(); }

} // namespace ModelerAi::remote
