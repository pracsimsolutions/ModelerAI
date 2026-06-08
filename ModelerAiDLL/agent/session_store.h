// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/session_store.h — disk persistence for Conversation objects.
//
// Default strategy (PerFile / "per model") keys each session by a GUID
// stored on the in-model ModelerAI object's projectSettings byteblock.
// That makes the chat anchor itself to the .fsm itself rather than to
// its filesystem path: rename, Save-As, or move-folders all preserve
// the session. PerFolder still keys by directory hash for the
// "everything-in-this-folder shares a chat" workflow. Files live at
// %APPDATA%/PRACSIM/ModelerAI/sessions/<key>.dat — DPAPI-encrypted
// JSON blobs tied to the current Windows user account. Autosave fires
// the moment an assistant turn completes (atomic rename). Loading
// happens when the viewer signals viewer_ready.
//
// v0.2 keeps this simple: one session per model, no multi-session
// browsing, no compaction. /history command + multi-session UI land later.

#pragma once

#include "agent/message.h"

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

namespace ModelerAi::agent {

// How the bridge derives the session key for the current FlexSim model.
// Picked by the user in Settings -> Sessions, persisted in global
// settings, and sent to the DLL with each viewer_ready envelope.
enum class SessionStrategy {
    PerFile,    // GUID on the in-model ModelerAI object; survives rename / Save-As
    PerFolder,  // hash(dir) — all .fsm files in same dir share one session
    Manual,     // user-supplied key, takes precedence over derivation
};

// Resolve the session key for the current FlexSim model under the
// chosen strategy. Must be called from the FlexSim main thread when
// `strategy != Manual` (touches the SDK). For Manual, `customKey` is
// returned verbatim and the function is thread-safe.
std::string currentSessionKey(SessionStrategy strategy = SessionStrategy::PerFile,
                              const std::string& customKey = "");

// Read-only peek at the in-model ModelerAI object's session_id. Used by
// the bridge poll loop to detect model changes WITHOUT side-effects (the
// regular currentSessionKey path will lazily generate-and-save a GUID if
// none exists, which we don't want on every poll tick). Returns "" when
// there's no in-model object or no GUID stored yet. Must be called from
// the FlexSim main thread.
std::string peekSessionIdFromProject();

// How Save-As (or a model open with a session pointer to a different
// existing .fsm dir) should treat the new file's session.
enum class SaveAsBehavior {
    Copy,    // duplicate the old session into a new GUID on the new model — DEFAULT
    Fresh,   // new model gets a fresh GUID + empty conversation
    Share    // legacy: both models keep writing to the same on-disk session file
};

SaveAsBehavior parseSaveAsBehavior(const std::string& s);
const char*    saveAsBehaviorName(SaveAsBehavior b);

// Result of applying a Save-As policy. effective_key is what the agent
// should use for the new model's session going forward; copied says
// whether we duplicated the on-disk conversation. share_kept says we
// did nothing (legacy mode).
struct SaveAsResolution {
    std::string    effective_key;
    bool           copied      = false;
    bool           freshly_set = false;
    bool           share_kept  = false;
};

// Apply the Save-As policy when a model open with the same session_id
// happens at a different modeldir. Mutates the in-model projectSettings
// (writes the new GUID) and writes the on-disk session file when
// behavior == Copy. Safe to call from FlexSim main thread.
SaveAsResolution applySaveAsBranch(const std::string& oldKey,
                                    SaveAsBehavior     behavior);

// Convert wire-format strings to/from the enum.
SessionStrategy parseStrategy(const std::string& s);
const char* strategyName(SessionStrategy s);

// File path the session for the given key would live at.
std::string sessionsDir();
std::string sessionFilePath(const std::string& key);

// Summary of one on-disk session file. Surfaced to the viewer via
// list_sessions so the user can pick a session for Manual mode.
struct SessionSummary {
    std::string key;             // matches the filename stem
    std::string model_path;      // value of "model_path" in the JSON; may be empty
    int         message_count = 0;
    int64_t     last_saved_ms = 0;
    int64_t     bytes = 0;
};

// Enumerate every .dat file under sessionsDir(). Each is DPAPI-decrypted
// + loaded just enough to read its metadata (skips the messages array).
// Files that fail to decrypt (different user / different machine) are
// still listed by key + bytes so they can be /cleared. Safe to call on
// any thread.
std::vector<SessionSummary> listSessions();

// Persist `conv` for the given session key. Atomic — writes to a .tmp
// then renames into place. Returns true on success.
bool saveSession(const std::string& key, const Conversation& conv);

// Load the conversation for the given key. Returns std::nullopt if no
// session file exists for that key, or if the file fails to parse.
std::optional<Conversation> loadSession(const std::string& key);

// Delete the session file (if any) for the given key. Returns true if
// the file existed and was removed.
bool deleteSession(const std::string& key);

} // namespace ModelerAi::agent
