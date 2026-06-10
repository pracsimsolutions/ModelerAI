// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "share/tunnel/tunnel.h"
#include "bridge/diag.h"
#include "paths.h"

#include <windows.h>

#include <atomic>
#include <fstream>
#include <memory>
#include <mutex>
#include <regex>
#include <sstream>
#include <thread>
#include <vector>

namespace ModelerAi::tunnel {

namespace {

std::mutex                              g_mu;
HANDLE                                  g_job        = nullptr;
PROCESS_INFORMATION                     g_pi         = {};
HANDLE                                  g_stderrRead = nullptr;
std::unique_ptr<std::thread>            g_readerThread;
std::atomic<bool>                       g_running{false};
Mode                                    g_mode       = Mode::None;
std::string                             g_publicUrl;

// SECURITY-CRITICAL: Create a Job Object with KILL_ON_JOB_CLOSE so
// the OS terminates the child when this DLL's host process dies.
// Without this, a FlexSim crash leaves cloudflared running and the
// public *.trycloudflare.com URL alive until the user notices.
HANDLE createJob()
{
    HANDLE job = CreateJobObjectW(nullptr, nullptr);
    if (!job) return nullptr;
    JOBOBJECT_EXTENDED_LIMIT_INFORMATION info = {};
    info.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if (!SetInformationJobObject(job, JobObjectExtendedLimitInformation,
                                  &info, sizeof(info))) {
        CloseHandle(job);
        return nullptr;
    }
    return job;
}

// Best-effort YAML scan for `hostname:` line in
// <moduleDir>/share/cloudflared/config.yml. Returns empty on failure.
std::string readHostnameFromConfigYml()
{
    auto installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";
    std::ifstream f(installDir + "\\share\\cloudflared\\config.yml");
    if (!f) return "";
    std::string line;
    while (std::getline(f, line)) {
        const std::string key = "hostname:";
        auto pos = line.find(key);
        if (pos == std::string::npos) continue;
        auto v = line.substr(pos + key.size());
        while (!v.empty() && (v.front() == ' ' || v.front() == '\t')) v.erase(0, 1);
        while (!v.empty() && (v.back() == '\r' || v.back() == ' ')) v.pop_back();
        if (v.size() >= 2 && v.front() == '"' && v.back() == '"') v = v.substr(1, v.size() - 2);
        return v;
    }
    return "";
}

// Best-effort .env loader. Looks at <moduleDir>/share/.env. Returns
// the value of CF_TUNNEL_TOKEN if found, empty otherwise. Used for
// Mode::Named.
std::string readCfTunnelTokenFromEnvFile()
{
    auto installDir = paths::moduleInstallDir();
    if (installDir.empty()) return "";
    std::ifstream f(installDir + "\\share\\.env");
    if (!f) return "";
    std::string line;
    while (std::getline(f, line)) {
        const std::string key = "CF_TUNNEL_TOKEN=";
        auto pos = line.find(key);
        if (pos == 0) {
            auto v = line.substr(key.size());
            // Trim CR/LF and surrounding quotes
            while (!v.empty() && (v.back() == '\r' || v.back() == '\n')) v.pop_back();
            if (v.size() >= 2 && v.front() == '"' && v.back() == '"')
                v = v.substr(1, v.size() - 2);
            return v;
        }
    }
    return "";
}

void readerLoop()
{
    static const std::regex urlRe(R"(https://[a-z0-9-]+\.trycloudflare\.com)");
    char buf[4096];
    DWORD n = 0;
    std::string acc;
    while (g_running.load()) {
        if (!ReadFile(g_stderrRead, buf, sizeof(buf) - 1, &n, nullptr)
            || n == 0) {
            break;
        }
        buf[n] = '\0';
        acc.append(buf, n);

        // Newline-based scan so we don't re-match the same URL twice
        // and so partial lines aren't logged twice.
        size_t pos = 0;
        while (true) {
            auto nl = acc.find('\n', pos);
            if (nl == std::string::npos) break;
            std::string line = acc.substr(pos, nl - pos);
            pos = nl + 1;

            diag::info(std::string("[cloudflared] ") + line);

            std::smatch m;
            if (std::regex_search(line, m, urlRe)) {
                std::lock_guard<std::mutex> lk(g_mu);
                if (g_publicUrl.empty()) {
                    g_publicUrl = m.str();
                    diag::info("[tunnel] public URL: " + g_publicUrl);
                }
            }
        }
        acc.erase(0, pos);
    }
}

} // namespace

StartResult start(Mode mode, int localPort)
{
    StartResult r;
    std::lock_guard<std::mutex> lk(g_mu);
    if (g_running.load()) {
        r.error_message = "already_running";
        return r;
    }

    // I3 — Resolve cloudflared.exe to an absolute path before spawning.
    // Using nullptr as lpApplicationName with a bare "cloudflared" in the
    // cmdline lets Windows search PATH, which includes user-writable dirs
    // (current dir, user-local bin). Resolving once via SearchPathW gives
    // the user an auditable log line and prevents PATH-shadow attacks.
    auto resolveCloudflared = []() -> std::wstring {
        wchar_t buf[MAX_PATH] = {0};
        DWORD n = SearchPathW(nullptr, L"cloudflared", L".exe",
                               MAX_PATH, buf, nullptr);
        if (n == 0 || n >= MAX_PATH) return L"";
        return std::wstring(buf, n);
    };
    std::wstring cfExe = resolveCloudflared();
    if (cfExe.empty()) {
        r.error_message = "cloudflared_not_found: install via "
                          "`winget install Cloudflare.cloudflared` then restart FlexSim";
        return r;
    }
    // Log the resolved path so the user can audit it.
    {
        int n = WideCharToMultiByte(CP_UTF8, 0, cfExe.c_str(), -1,
                                     nullptr, 0, nullptr, nullptr);
        std::string narrow(n - 1, '\0');
        WideCharToMultiByte(CP_UTF8, 0, cfExe.c_str(), -1,
                            narrow.data(), n, nullptr, nullptr);
        diag::info("[tunnel] spawning cloudflared from: " + narrow);
    }

    // I2 — Token validation. Reject tokens containing characters outside
    // [A-Za-z0-9+/=._-] to prevent CLI-flag injection in Named mode.
    auto isValidToken = [](const std::string& t) {
        if (t.empty()) return false;
        for (char c : t) {
            bool ok = (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z')
                   || (c >= '0' && c <= '9') || c == '+' || c == '/'
                   || c == '=' || c == '.' || c == '_' || c == '-';
            if (!ok) return false;
        }
        return true;
    };

    // Build command line per mode.
    // I2 — Named mode: token is NOT placed on the cmdline; it is passed via
    // TUNNEL_TOKEN env var in the child's environment block instead.
    std::wstring cmdline;
    std::string tok;  // Named mode token (narrow); empty for Quick
    if (mode == Mode::Quick) {
        // I3 — Use quoted absolute path as the executable prefix.
        cmdline = L"\"" + cfExe + L"\" tunnel --url http://127.0.0.1:"
                 + std::to_wstring(localPort)
                 + L" --no-autoupdate";
    } else if (mode == Mode::Named) {
        tok = readCfTunnelTokenFromEnvFile();
        if (tok.empty()) {
            // Also check environment (allows users to set externally).
            char buf[2048];
            DWORD n = GetEnvironmentVariableA("CF_TUNNEL_TOKEN", buf, sizeof(buf));
            if (n > 0 && n < sizeof(buf)) tok.assign(buf, n);
        }
        if (tok.empty()) {
            r.error_message = "missing_cf_tunnel_token";
            return r;
        }
        if (!isValidToken(tok)) {
            r.error_message = "invalid_cf_tunnel_token: contains unexpected characters";
            return r;
        }
        // I2 — Token on cmdline removed; passed via TUNNEL_TOKEN env var below.
        // I3 — Use quoted absolute path as the executable prefix.
        cmdline = L"\"" + cfExe + L"\" tunnel run --no-autoupdate";
    } else {
        r.error_message = "mode_none";
        return r;
    }

    // I2 — Build a child environment block: parent env + TUNNEL_TOKEN for
    // Named mode. This avoids placing the token in the cmdline where it
    // would be visible in process listings and could be CLI-injected.
    std::wstring envBlock;
    {
        LPWCH parentEnv = GetEnvironmentStringsW();
        if (parentEnv) {
            LPWCH p = parentEnv;
            while (*p) {
                std::wstring var(p);
                envBlock += var;
                envBlock.push_back(L'\0');
                p += var.size() + 1;
            }
            FreeEnvironmentStringsW(parentEnv);
        }
        if (mode == Mode::Named && !tok.empty()) {
            std::wstring wtok(tok.begin(), tok.end());
            envBlock += L"TUNNEL_TOKEN=" + wtok;
            envBlock.push_back(L'\0');
        }
        envBlock.push_back(L'\0');  // double-null terminator
    }

    // Build the Job Object FIRST so the child enters it atomically.
    g_job = createJob();
    if (!g_job) {
        r.error_message = "job_create_failed";
        return r;
    }

    // stderr pipe so we can scrape the URL line (Quick) and surface errors.
    SECURITY_ATTRIBUTES sa = { sizeof(sa), nullptr, TRUE };
    HANDLE stderrWrite = nullptr;
    if (!CreatePipe(&g_stderrRead, &stderrWrite, &sa, 0)) {
        CloseHandle(g_job); g_job = nullptr;
        r.error_message = "pipe_failed";
        return r;
    }
    SetHandleInformation(g_stderrRead, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si = { sizeof(si) };
    si.dwFlags    = STARTF_USESTDHANDLES;
    si.hStdError  = stderrWrite;
    si.hStdOutput = stderrWrite;   // route stdout to same pipe; cloudflared logs to stderr anyway.
    si.hStdInput  = INVALID_HANDLE_VALUE;  // no stdin — cloudflared takes no input

    std::vector<wchar_t> cmd(cmdline.begin(), cmdline.end());
    cmd.push_back(L'\0');

    // I3 — Pass cfExe.c_str() as lpApplicationName (absolute path) to
    // eliminate the current-directory and PATH-shadow attack vectors.
    // I2 — Pass CREATE_UNICODE_ENVIRONMENT and the custom env block so
    // TUNNEL_TOKEN reaches the child without appearing on the cmdline.
    // CREATE_NEW_PROCESS_GROUP so we can later send Ctrl-Break to the
    // tree without affecting our own process.
    BOOL ok = CreateProcessW(
        cfExe.c_str(),       // lpApplicationName — absolute resolved path (I3)
        cmd.data(),          // lpCommandLine — still needed for args
        nullptr, nullptr,
        TRUE,
        CREATE_NEW_PROCESS_GROUP | CREATE_NO_WINDOW | CREATE_UNICODE_ENVIRONMENT,
        envBlock.data(),     // lpEnvironment — parent env + TUNNEL_TOKEN (I2)
        nullptr,
        &si, &g_pi);

    CloseHandle(stderrWrite);   // we keep the read end

    if (!ok) {
        DWORD err = GetLastError();
        CloseHandle(g_stderrRead); g_stderrRead = nullptr;
        CloseHandle(g_job);        g_job = nullptr;
        r.error_message = "CreateProcess failed: " + std::to_string(err)
                        + " (is cloudflared on PATH?)";
        return r;
    }

    // Assign the child to the Job Object IMMEDIATELY after creation.
    // KILL_ON_JOB_CLOSE means we don't have to explicitly terminate
    // cloudflared in normal stop() paths either — closing g_job is
    // sufficient.
    if (!AssignProcessToJobObject(g_job, g_pi.hProcess)) {
        TerminateProcess(g_pi.hProcess, 1);
        CloseHandle(g_pi.hThread);
        CloseHandle(g_pi.hProcess);
        CloseHandle(g_stderrRead); g_stderrRead = nullptr;
        CloseHandle(g_job);        g_job = nullptr;
        r.error_message = "job_assign_failed";
        return r;
    }

    g_mode = mode;
    g_running.store(true);
    g_readerThread = std::make_unique<std::thread>(readerLoop);

    // For Named mode: read the user-configured hostname from
    // share/cloudflared/config.yml and populate g_publicUrl NOW, before
    // returning, so the caller's publicUrl() poll finds it on the first
    // check.  g_mu is already held here — no re-lock needed.
    if (mode == Mode::Named) {
        auto host = readHostnameFromConfigYml();
        if (!host.empty()) {
            g_publicUrl = (host.find("://") == std::string::npos)
                          ? "https://" + host
                          : host;
            diag::info("[tunnel] named-mode public URL: " + g_publicUrl);
        }
        // If empty, the caller's poll loop will time out with a clear
        // error pointing at share/cloudflared/config.yml.
    }

    r.ok = true;
    r.public_url = "";   // filled by reader (Quick) or above (Named); caller polls publicUrl()
    return r;
}

void stop()
{
    HANDLE job = nullptr;
    HANDLE proc = nullptr;
    HANDLE thr = nullptr;
    HANDLE stderrRead = nullptr;
    std::unique_ptr<std::thread> reader;
    {
        std::lock_guard<std::mutex> lk(g_mu);
        if (!g_running.load()) return;
        g_running.store(false);
        job = g_job;        g_job = nullptr;
        proc = g_pi.hProcess; g_pi.hProcess = nullptr;
        thr  = g_pi.hThread;  g_pi.hThread  = nullptr;
        stderrRead = g_stderrRead; g_stderrRead = nullptr;
        reader = std::move(g_readerThread);
        g_publicUrl.clear();
        g_mode = Mode::None;
    }

    // Try graceful shutdown first.
    if (proc) {
        GenerateConsoleCtrlEvent(CTRL_BREAK_EVENT, GetProcessId(proc));
        if (WaitForSingleObject(proc, 5000) != WAIT_OBJECT_0) {
            TerminateProcess(proc, 1);
        }
        CloseHandle(proc);
    }
    if (thr) CloseHandle(thr);
    if (stderrRead) CloseHandle(stderrRead);   // unblocks the reader
    if (reader && reader->joinable()) reader->join();
    if (job) CloseHandle(job);   // also kills any straggler child via KILL_ON_JOB_CLOSE

    diag::info("[tunnel] stopped");
}

bool isRunning() { return g_running.load(); }
Mode currentMode() { return g_mode; }
std::string publicUrl()
{
    std::lock_guard<std::mutex> lk(g_mu);
    return g_publicUrl;
}

} // namespace ModelerAi::tunnel
