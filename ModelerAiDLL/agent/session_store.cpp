// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"          // FlexSim SDK — must come before json.h wrapper
#include "agent/session_store.h"
#include "bridge/log.h"
#include "paths.h"
#include "settings/project_settings.h"
#include "third_party/json.h"

#include <windows.h>
#include <dpapi.h>            // CryptProtectData / CryptUnprotectData
#include <wincrypt.h>
#include <objbase.h>          // CoCreateGuid
#include <algorithm>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#pragma comment(lib, "crypt32.lib")

namespace ModelerAi::agent {

namespace {

// ----- DPAPI helpers (same pattern as keystore/keystore.cpp) -----
// Conversation contents include the full user prompts + assistant
// replies + tool results, which can contain proprietary model data.
// We encrypt at rest so a casual filesystem-level snoop on a shared
// or RDP machine can't read it. Tied to current Windows user account,
// so roaming the file to another user/machine yields garbage.
bool dpapiEncrypt(std::string_view plaintext, std::vector<unsigned char>& out)
{
    DATA_BLOB in{};
    in.pbData = (BYTE*)plaintext.data();
    in.cbData = (DWORD)plaintext.size();
    DATA_BLOB enc{};
    if (!CryptProtectData(&in, L"ModelerAI-Session",
                          nullptr, nullptr, nullptr, 0, &enc))
    {
        return false;
    }
    out.assign(enc.pbData, enc.pbData + enc.cbData);
    LocalFree(enc.pbData);
    return true;
}

bool dpapiDecrypt(const std::vector<unsigned char>& blob, std::string& outPlain)
{
    if (blob.empty()) { outPlain.clear(); return true; }
    DATA_BLOB in{};
    in.pbData = (BYTE*)blob.data();
    in.cbData = (DWORD)blob.size();
    DATA_BLOB plain{};
    LPWSTR descOut = nullptr;
    if (!CryptUnprotectData(&in, &descOut,
                            nullptr, nullptr, nullptr, 0, &plain))
    {
        return false;
    }
    outPlain.assign((const char*)plain.pbData, plain.cbData);
    if (descOut) LocalFree(descOut);
    LocalFree(plain.pbData);
    return true;
}

// FNV-1a 64-bit hash. Not cryptographic — just need a short, deterministic,
// filesystem-safe identifier per folder for PerFolder strategy. Output
// as 16 hex digits.
std::string fnv1aHex(const std::string& s)
{
    uint64_t h = 0xcbf29ce484222325ULL;
    for (unsigned char c : s) {
        h ^= (uint64_t)c;
        h *= 0x100000001b3ULL;
    }
    char buf[17];
    std::snprintf(buf, sizeof(buf), "%016llx", (unsigned long long)h);
    return buf;
}

const char* roleToStr(Role r)
{
    switch (r) {
        case Role::User:      return "user";
        case Role::Assistant: return "assistant";
        case Role::Tool:      return "tool";
        case Role::System:    return "system";
    }
    return "user";
}

Role roleFromStr(const std::string& s)
{
    if (s == "assistant") return Role::Assistant;
    if (s == "tool")      return Role::Tool;
    if (s == "system")    return Role::System;
    return Role::User;
}

int64_t timePointToEpochMs(std::chrono::system_clock::time_point tp)
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(tp.time_since_epoch()).count();
}

std::chrono::system_clock::time_point epochMsToTimePoint(int64_t ms)
{
    using namespace std::chrono;
    return system_clock::time_point{ milliseconds(ms) };
}

bool ensureDir(const std::string& dir)
{
    if (dir.empty()) return false;
    // CreateDirectoryA is idempotent — ERROR_ALREADY_EXISTS is fine.
    if (CreateDirectoryA(dir.c_str(), nullptr)) return true;
    return GetLastError() == ERROR_ALREADY_EXISTS;
}

} // namespace

SessionStrategy parseStrategy(const std::string& s)
{
    if (s == "per-folder" || s == "folder") return SessionStrategy::PerFolder;
    if (s == "manual")                      return SessionStrategy::Manual;
    return SessionStrategy::PerFile;
}

const char* strategyName(SessionStrategy s)
{
    switch (s) {
        case SessionStrategy::PerFile:   return "per-file";
        case SessionStrategy::PerFolder: return "per-folder";
        case SessionStrategy::Manual:    return "manual";
    }
    return "per-file";
}

namespace {

// 32-hex-char GUID (no braces, no dashes). The session file's stem is
// this string. Collisions are astronomically unlikely.
std::string generateSessionGuid()
{
    GUID g;
    if (FAILED(CoCreateGuid(&g))) {
        // Astronomically unlikely; fall back to time-seeded random hex
        // so we still return SOMETHING usable.
        uint64_t t = (uint64_t)std::chrono::system_clock::now().time_since_epoch().count();
        return fnv1aHex(std::to_string(t)) + fnv1aHex(std::to_string(t ^ 0x9e3779b97f4a7c15ULL));
    }
    char buf[33];
    std::snprintf(buf, sizeof(buf),
        "%08lx%04x%04x%02x%02x%02x%02x%02x%02x%02x%02x",
        (unsigned long)g.Data1, g.Data2, g.Data3,
        g.Data4[0], g.Data4[1], g.Data4[2], g.Data4[3],
        g.Data4[4], g.Data4[5], g.Data4[6], g.Data4[7]);
    return buf;
}

// Read session_id from the in-model projectSettings, generate + persist
// one if missing. Returns empty if there's no in-model object yet (i.e.
// addmodelerai hasn't run). Caller must be on the FlexSim main thread.
std::string sessionIdFromProject()
{
    if (!settings::projectStorageAvailable()) return "";
    auto j = settings::loadProjectSettings();
    if (j.contains("session_id") && j["session_id"].is_string()) {
        std::string s = j["session_id"].get<std::string>();
        if (!s.empty()) return s;
    }
    std::string fresh = generateSessionGuid();
    settings::patchProjectSettings({{"session_id", fresh}});
    return fresh;
}

} // namespace

// Read-only peek — never generates a GUID. Used by the bridge poll loop
// for change detection so we don't silently write GUIDs into every model
// the user opens just by polling.
std::string peekSessionIdFromProject()
{
    if (!settings::projectStorageAvailable()) return "";
    auto j = settings::loadProjectSettings();
    if (j.contains("session_id") && j["session_id"].is_string()) {
        return j["session_id"].get<std::string>();
    }
    return "";
}

SaveAsBehavior parseSaveAsBehavior(const std::string& s)
{
    if (s == "fresh") return SaveAsBehavior::Fresh;
    if (s == "share") return SaveAsBehavior::Share;
    return SaveAsBehavior::Copy;  // default
}

const char* saveAsBehaviorName(SaveAsBehavior b)
{
    switch (b) {
        case SaveAsBehavior::Copy:  return "copy";
        case SaveAsBehavior::Fresh: return "fresh";
        case SaveAsBehavior::Share: return "share";
    }
    return "copy";
}

SaveAsResolution applySaveAsBranch(const std::string& oldKey, SaveAsBehavior behavior)
{
    SaveAsResolution r;
    r.effective_key = oldKey;  // default if nothing applies

    if (behavior == SaveAsBehavior::Share) {
        // Legacy: both models share the same session file. We don't
        // touch the in-model projectSettings — it already has oldKey.
        r.share_kept = true;
        return r;
    }

    // Both Copy and Fresh generate a new GUID for the new model.
    std::string newKey = generateSessionGuid();

    if (behavior == SaveAsBehavior::Copy) {
        // Duplicate the on-disk session under the new GUID so the new
        // model starts with the same history as the old model and the
        // two diverge from here. Loaded conversation's model_path will
        // be re-written by the next autosave.
        auto loaded = loadSession(oldKey);
        if (loaded.has_value()) {
            saveSession(newKey, *loaded);
            r.copied = true;
        }
    } else {
        r.freshly_set = true;
    }

    // Write the new GUID into the new model's projectSettings so any
    // future open of THIS model resolves to newKey.
    settings::patchProjectSettings({{"session_id", newKey}});
    r.effective_key = newKey;
    return r;
}

std::string currentSessionKey(SessionStrategy strategy, const std::string& customKey)
{
    if (strategy == SessionStrategy::Manual) {
        // User-controlled — pass the picked key through unchanged.
        // Blank falls through to PerFile so the UI degrades gracefully.
        if (!customKey.empty()) return customKey;
    }

    // PerFile (default): the session id lives ON THE MODEL itself in
    // the projectSettings byteblock. This means:
    //   * Rename a .fsm to anything — same session, because the GUID
    //     travels with the model, not the path.
    //   * Brand-new model — gets a fresh GUID immediately on first
    //     access, so it can never inherit another model's history.
    //   * Reopen FlexSim later — the GUID is still in the byteblock,
    //     so loadSession picks the conversation back up automatically.
    //   * No in-model object yet (panel opened before addmodelerai
    //     has been run on the current model): returns empty string.
    //     Callers must treat that as "no anchor" and skip load + save
    //     entirely — the chat stays in memory only. This used to fall
    //     back to a shared "__unsaved__" sentinel file, which caused
    //     every fresh FlexSim launch with no anchor to reload the prior
    //     no-anchor conversation across launches. Killed.
    if (strategy == SessionStrategy::PerFile || strategy == SessionStrategy::Manual) {
        return sessionIdFromProject();  // "" when no anchor
    }

    // PerFolder: every .fsm in the directory shares one chat. The
    // folder path itself is the identity, so this deliberately ignores
    // the in-model session_id.
    std::string dir;
    try {
        const char* d = modeldir_cstr();
        if (d) dir = d;
    } catch (...) {}

    if (dir.empty()) {
        // Unsaved model under PerFolder: no folder, so fall back to
        // the in-model GUID — returns "" if no anchor, same contract.
        return sessionIdFromProject();
    }
    return fnv1aHex(dir);
}

std::string sessionsDir()
{
    std::string app = paths::appDataDir();  // already ends with backslash
    if (app.empty()) return "";
    return app + "sessions\\";
}

std::string sessionFilePath(const std::string& key)
{
    std::string dir = sessionsDir();
    if (dir.empty() || key.empty()) return "";
    // .dat extension signals binary (DPAPI ciphertext); old .json files
    // from before the encryption work are orphaned — acceptable pre-ship.
    return dir + key + ".dat";
}

bool saveSession(const std::string& key, const Conversation& conv)
{
    std::string dir = sessionsDir();
    if (dir.empty()) return false;
    if (!ensureDir(dir)) return false;

    std::string finalPath = sessionFilePath(key);
    std::string tmpPath   = finalPath + ".tmp";

    nlohmann::json j;
    j["version"]     = 1;
    j["saved_at_ms"] = timePointToEpochMs(std::chrono::system_clock::now());
    j["key"]         = key;
    j["model_path"]  = conv.model_path;

    nlohmann::json msgs = nlohmann::json::array();
    for (const auto& m : conv.messages) {
        nlohmann::json e;
        e["role"]   = roleToStr(m.role);
        e["text"]   = m.text;
        e["ts_ms"]  = timePointToEpochMs(m.ts);
        // Tool fields reserved for the future; serialize empty so reload
        // round-trips cleanly.
        msgs.push_back(std::move(e));
    }
    j["messages"] = std::move(msgs);

    // Encrypt the JSON dump via DPAPI before writing. Tied to current
    // Windows user — protects against shared-machine / RDP snooping.
    std::vector<unsigned char> blob;
    if (!dpapiEncrypt(j.dump(), blob)) {
        bridge::consolePrint(("[ModelerAI] WARN: dpapiEncrypt failed for session key=" + key + "\n"));
        return false;
    }

    {
        std::ofstream out(tmpPath, std::ios::binary);
        if (!out) return false;
        out.write(reinterpret_cast<const char*>(blob.data()), (std::streamsize)blob.size());
        out.close();
        if (!out.good()) return false;
    }

    // Atomic rename. MoveFileExA with REPLACE_EXISTING gives us the
    // single-file-swap behavior we want even when finalPath already
    // exists.
    if (!MoveFileExA(tmpPath.c_str(), finalPath.c_str(),
                     MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH))
    {
        DeleteFileA(tmpPath.c_str());
        return false;
    }
    return true;
}

std::optional<Conversation> loadSession(const std::string& key)
{
    std::string path = sessionFilePath(key);
    if (path.empty()) return std::nullopt;

    std::ifstream f(path, std::ios::binary);
    if (!f) return std::nullopt;
    std::ostringstream os;
    os << f.rdbuf();
    std::string raw = os.str();
    if (raw.empty()) return std::nullopt;

    // Decrypt — DPAPI ciphertext written by saveSession on this user
    // account on this machine. Mismatched user/machine returns nullopt.
    std::vector<unsigned char> blob(raw.begin(), raw.end());
    std::string body;
    if (!dpapiDecrypt(blob, body)) {
        bridge::consolePrint(("[ModelerAI] session decrypt failed key=" + key
                              + " (different user/machine, or pre-DPAPI file)\n"));
        return std::nullopt;
    }

    Conversation conv;
    try {
        auto j = nlohmann::json::parse(body);

        if (j.contains("model_path") && j["model_path"].is_string()) {
            conv.model_path = j["model_path"].get<std::string>();
        }
        if (j.contains("messages") && j["messages"].is_array()) {
            for (const auto& e : j["messages"]) {
                Message m;
                if (e.contains("role") && e["role"].is_string()) {
                    m.role = roleFromStr(e["role"].get<std::string>());
                }
                if (e.contains("text") && e["text"].is_string()) {
                    m.text = e["text"].get<std::string>();
                }
                if (e.contains("ts_ms") && e["ts_ms"].is_number_integer()) {
                    m.ts = epochMsToTimePoint(e["ts_ms"].get<int64_t>());
                } else {
                    m.ts = std::chrono::system_clock::now();
                }
                conv.messages.push_back(std::move(m));
            }
        }
    } catch (...) {
        bridge::consolePrint(("[ModelerAI] session parse failed for key=" + key + "\n"));
        return std::nullopt;
    }
    return conv;
}

bool deleteSession(const std::string& key)
{
    std::string path = sessionFilePath(key);
    if (path.empty()) return false;
    return DeleteFileA(path.c_str()) != 0;
}

std::vector<SessionSummary> listSessions()
{
    std::vector<SessionSummary> out;
    std::string dir = sessionsDir();
    if (dir.empty()) return out;

    std::string pattern = dir + "*.dat";
    WIN32_FIND_DATAA fd;
    HANDLE h = FindFirstFileA(pattern.c_str(), &fd);
    if (h == INVALID_HANDLE_VALUE) return out;

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) continue;
        std::string fname = fd.cFileName;
        // Strip ".dat"
        if (fname.size() < 5) continue;
        std::string key = fname.substr(0, fname.size() - 4);

        std::ifstream f(dir + fname, std::ios::binary);
        if (!f) continue;
        std::ostringstream ss;
        ss << f.rdbuf();
        std::string raw = ss.str();
        if (raw.empty()) continue;

        SessionSummary s;
        s.key   = key;
        s.bytes = (int64_t)raw.size();

        // Decrypt to read metadata. If decryption fails (different user,
        // different machine, corrupted), surface the entry with bytes
        // populated so the user can /clear it but skip the JSON details.
        std::vector<unsigned char> blob(raw.begin(), raw.end());
        std::string body;
        if (!dpapiDecrypt(blob, body)) {
            out.push_back(std::move(s));
            continue;
        }

        try {
            auto j = nlohmann::json::parse(body);
            if (j.contains("saved_at_ms") && j["saved_at_ms"].is_number_integer()) {
                s.last_saved_ms = j["saved_at_ms"].get<int64_t>();
            }
            if (j.contains("model_path") && j["model_path"].is_string()) {
                s.model_path = j["model_path"].get<std::string>();
            }
            if (j.contains("messages") && j["messages"].is_array()) {
                s.message_count = (int)j["messages"].size();
            }
        } catch (...) {
            // Malformed file — surface it anyway so the user can /clear it.
        }

        out.push_back(std::move(s));
    } while (FindNextFileA(h, &fd));
    FindClose(h);

    // Sort newest-first by save timestamp.
    std::sort(out.begin(), out.end(),
              [](const SessionSummary& a, const SessionSummary& b) {
                  return a.last_saved_ms > b.last_saved_ms;
              });
    return out;
}

} // namespace ModelerAi::agent
