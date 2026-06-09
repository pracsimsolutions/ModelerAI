// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // FlexSim SDK — applicationcommand(); must precede json.h
#include "bridge/bridge.h"
#include "bridge/log.h"
#include "bridge/protocol.h"
#include "bridge/subscriber.h"
#include "bootstrap.h"
#include "agent/agent.h"
#include "agent/session_store.h"
#include "agent/slash.h"
#include "agent/slash_remote.h"
#include "agent/user_rules.h"
#include "keystore/keystore.h"
#include "paths.h"
#include "provider/provider.h"
#include "provider/openai_compatible.h"
#include "remote/remote_server.h"
#include "remote/qr_payload.h"
#include "settings/effective_settings.h"
#include "settings/global_settings.h"
#include "settings/project_settings.h"
#include "tree/condense.h"
#include "third_party/json.h"

#include <ctime>
#include <filesystem>
#include <fstream>
#include <thread>

#include <chrono>
#include <deque>
#include <mutex>
#include <sstream>
#include <string>

namespace ModelerAi::bridge {

namespace {

std::string buildErrorEnvelope(const std::string& turnId,
                               const char* code,
                               const std::string& message)
{
    nlohmann::json env;
    env["t"]  = "error";
    env["id"] = turnId;
    nlohmann::json p;
    p["code"]      = code;
    p["message"]   = message;
    p["retriable"] = false;
    env["p"] = std::move(p);
    return env.dump();
}

std::string buildStateUpdateEnvelope(const std::string& turnId,
                                     const std::string& key,
                                     nlohmann::json value)
{
    nlohmann::json env;
    env["t"]  = "state_update";
    env["id"] = turnId;
    nlohmann::json p;
    p["key"]   = key;
    p["value"] = std::move(value);
    env["p"]   = std::move(p);
    return env.dump();
}

// Serialize a single ModelInfo to the JSON shape the viewer consumes.
nlohmann::json modelInfoJson(const provider::ModelInfo& m)
{
    nlohmann::json j;
    j["model_id"]                   = m.id;
    j["model_name"]                 = m.display_name;
    j["context_tokens"]             = m.context_tokens;
    j["supports_tools"]             = m.supports_tools;
    j["supports_vision"]            = m.supports_vision;
    j["supports_thinking"]          = m.supports_thinking;
    j["input_usd_per_mtok"]         = m.input_usd_per_mtok;
    j["output_usd_per_mtok"]        = m.output_usd_per_mtok;
    j["cache_write_usd_per_mtok"]   = m.cache_write_usd_per_mtok;
    j["cache_read_usd_per_mtok"]    = m.cache_read_usd_per_mtok;
    return j;
}

const char* effortToStr(provider::EffortLevel e)
{
    switch (e) {
        case provider::EffortLevel::Off:    return "off";
        case provider::EffortLevel::Low:    return "low";
        case provider::EffortLevel::Medium: return "medium";
        case provider::EffortLevel::High:   return "high";
    }
    return "off";
}

provider::EffortLevel effortFromStr(const std::string& s)
{
    if (s == "low")    return provider::EffortLevel::Low;
    if (s == "medium") return provider::EffortLevel::Medium;
    if (s == "high")   return provider::EffortLevel::High;
    return provider::EffortLevel::Off;
}

// Author label for the shared-chat user_message_broadcast envelope.
// Local CEF (kLocalFlexsimSubscriberId) is "Host". Remote subscribers
// get a short readable label derived from their subscriber id so
// participants can tell who said what without a real display-name
// system (deferred to v2).
std::string deriveAuthor(std::string_view sid)
{
    if (sid == kLocalFlexsimSubscriberId) return "Host";
    // Take the first 6 chars of the sid. Subscriber ids are random 24-char
    // base64 strings, so 6 chars is enough entropy for human disambiguation
    // in a typical 1-10 guest demo while staying readable.
    std::string s(sid);
    if (s.size() > 6) s.resize(6);
    return "Guest-" + s;
}

// Push the user's persisted settings into runtime state (Agent + active
// provider). Called on viewer_ready and after any set_setting so the
// running session always matches the saved EffectiveSettings.
void applyEffectiveToRuntime(const settings::EffectiveSettings& eff)
{
    auto* a = bootstrap::agent();
    if (!a) return;

    // Mode.
    a->setMode(agent::parseMode(eff.mode));

    // Per-tool policies — clear any session-only entries the user set
    // via Always-Allow that aren't reflected in persisted settings,
    // then layer persisted policies back in. Simpler: just apply the
    // persisted ones; session-set ones remain (no harm).
    for (const auto& kv : eff.tool_policies) {
        a->setToolPolicy(kv.first, agent::parsePolicy(kv.second));
    }

    // Active provider + model.
    if (!eff.active_model_id.empty()) {
        if (auto* owner = bootstrap::findProviderForModel(eff.active_model_id)) {
            bootstrap::setActiveProvider(owner);
            owner->set_active_model(eff.active_model_id);
            // Effort applies to whatever provider just became active.
            owner->set_effort(provider::EffortLevel{[&]{
                if (eff.effort == "low")    return provider::EffortLevel::Low;
                if (eff.effort == "medium") return provider::EffortLevel::Medium;
                if (eff.effort == "high")   return provider::EffortLevel::High;
                return provider::EffortLevel::Off;
            }()});
        }
    }
}

// Emit a fresh `settings` state_update so the viewer's panel reflects
// any write that just landed. Called after set_setting succeeds.
void pushSettingsUpdate(const std::string& turnId)
{
    auto eff     = settings::resolveEffectiveSettings();
    auto global  = settings::loadGlobalSettings();
    auto project = settings::loadProjectSettings();
    nlohmann::json v;
    v["effective"]            = settings::toJson(eff);
    v["global"]               = global;
    v["project"]              = project;
    v["project_avail"]        = settings::projectStorageAvailable();
    v["onboarding_current_version"]  = std::string(settings::kCurrentOnboardingVersion);
    pushEvent(buildStateUpdateEnvelope(turnId, "settings", std::move(v)));
}

// Build the initial-state snapshot the viewer wants on mount: active
// model + provider + capability flags, model catalog, effort, session
// stats, user-rules preview, slash command catalog.
// Forward decl — pushInitialState is defined further down but called by
// pushInitialStateStored() right below.
void pushInitialState(const std::string& turnId,
                      agent::SessionStrategy strategy,
                      const std::string& customSessionKey,
                      std::string_view target_sid = "");

// Convenience wrapper: re-push initial state under whatever
// session-strategy the viewer last reported via viewer_ready. Use this
// from set_setting / reset_settings / provider_switch handlers so the
// strategy doesn't silently revert to PerFile.
// Always broadcasts (no target_sid) because those handlers represent
// settings changes that legitimately affect every connected viewer.
void pushInitialStateStored(const std::string& turnId)
{
    auto* a = bootstrap::agent();
    if (!a) return;
    pushInitialState(turnId, agent::parseStrategy(a->sessionStrategy()), a->sessionCustomKey());
}

// --------------------------------------------------------------------------
// Model-change auto-detection state.
//
// We track the last-seen {modeldir, in-model session_id} pair across
// FlexSim main-thread poll ticks. When either component changes we run
// the re-anchor flow: cancel any in-flight turn, apply the Save-As policy
// if it looks like a Save-As branch, then push a fresh `session` envelope
// so the viewer rehydrates.
//
// Limitation: there's no public SDK function for the current .fsm
// filename, only the directory. Save-As that only changes the filename
// (same dir) is therefore not detected. Save-As to a different folder
// is the common case and DOES get detected via modeldir change.
// --------------------------------------------------------------------------
struct LastSeenModel {
    std::string sessionId;
    std::string modelDir;
    bool        initialized = false;
};
LastSeenModel              g_lastSeenModel;
std::mutex                 g_lastSeenMutex;

// ---------------------------------------------------------------------------
// Re-anchor flow used by onPollTick when a model change is detected.
//
// Sequence:
//   1. Cancel any in-flight turn via cancelForModelSwitch (preserves
//      partial assistant text in the OLD session under an [Interrupted]
//      marker so navigating back to that model shows what was happening).
//   2. Briefly wait for the worker thread to wind down + autosave the
//      partial reply to the OLD session_key.
//   3. Decide whether this is a Save-As (same session_id, new modeldir)
//      and apply the user's save_as_behavior setting if so.
//   4. Clear in-memory conversation, re-resolve the new session key,
//      load it from disk if it exists.
//   5. Push the standard `session` envelope so the viewer rehydrates.
//
// Runs on the FlexSim main thread (called from bridgePoll). The brief
// wait at step 2 blocks the main thread but bounded to ~500ms.
// ---------------------------------------------------------------------------
void reanchorAfterModelChange(const std::string& prevSessionId,
                               const std::string& prevModelDir,
                               const std::string& currentSessionId,
                               const std::string& currentModelDir)
{
    auto* a = bootstrap::agent();
    if (!a) return;

    consolePrint(("[ModelerAI] model context change detected: "
                  "prev{id=" + prevSessionId + ", dir=" + prevModelDir + "} "
                  "→ curr{id=" + currentSessionId + ", dir=" + currentModelDir + "}\n"));

    // Step 1: cancel in-flight turn (preserving partial reply for the
    // OLD session). If no turn is running this is a no-op.
    if (a->cancelForModelSwitch()) {
        // Step 2: wait briefly for the worker to wind down + autosave.
        // Cancel typically completes in <100ms (WinHTTP read loop polls
        // the cancel flag between chunks). Cap at 1s so we don't hang
        // FlexSim's UI thread on a stuck network read.
        for (int i = 0; i < 100; ++i) {
            if (!a->turnInFlight()) break;
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        if (a->turnInFlight()) {
            consolePrint("[ModelerAI] WARN: model-switch cancel did not wind down in 1s; proceeding anyway\n");
        }
    }

    // Step 3: Save-As detection. If the new model has the SAME session_id
    // we previously saw but a DIFFERENT modeldir, the user just did a
    // Save-As (or otherwise produced a second file pointing at the same
    // session GUID). Apply the user's save_as_behavior setting.
    bool isSaveAs = !currentSessionId.empty()
                  && currentSessionId == prevSessionId
                  && !currentModelDir.empty()
                  && currentModelDir != prevModelDir;
    if (isSaveAs) {
        // Read the user's preference. Default is "copy" (matches the
        // brainstorming decision: branching workflow where the original
        // and the Save-As are both viable scenarios).
        auto globalSettings = settings::loadGlobalSettings();
        std::string mode = "copy";
        if (globalSettings.contains("save_as_behavior")
            && globalSettings["save_as_behavior"].is_string())
        {
            mode = globalSettings["save_as_behavior"].get<std::string>();
        }
        agent::SaveAsBehavior behavior = agent::parseSaveAsBehavior(mode);
        auto resolution = agent::applySaveAsBranch(currentSessionId, behavior);
        consolePrint(("[ModelerAI] Save-As detected — behavior=" + std::string(agent::saveAsBehaviorName(behavior))
                       + " effective_key=" + resolution.effective_key
                       + (resolution.copied ? " (conversation copied)" : "")
                       + (resolution.freshly_set ? " (fresh start)" : "")
                       + (resolution.share_kept ? " (legacy share kept)" : "") + "\n"));
        // applySaveAsBranch already wrote the new GUID into projectSettings
        // when behavior != Share, so the next pushInitialStateStored will
        // re-read it as the new anchor.
    }

    // Step 4 + 5: re-push initial state. pushInitialStateStored calls
    // pushInitialState which re-resolves currentSessionKey, loads the
    // session if any, updates the agent's in-memory conversation, and
    // emits the `session` envelope. The viewer's `session` handler then
    // replaces the chat surface with the new model's history (or shows
    // the empty-chat placeholder + ⚓ banner if there's no anchor).
    pushInitialStateStored(/*turnId=*/"model_change");
}

// `target_sid`:
//   - empty (default): broadcast every state_update via pushEvent. Used
//     by pushInitialStateStored for fan-out after settings changes.
//   - non-empty: route every state_update via pushEventTo(target_sid).
//     Used by viewer_ready so a phone connecting only updates ITS state,
//     not the desktop's. The audit at 2026-06-05-remote-viewer-audit.md
//     identifies this as the fix for the "chat wipes on phone connect"
//     symptom.
void pushInitialState(const std::string& turnId,
                      agent::SessionStrategy strategy,
                      const std::string& customSessionKey,
                      std::string_view target_sid)
{
    // Pick the dispatch shape ONCE up top. Avoids `if (target_sid.empty())`
    // sprinkled throughout the body, and means a future contributor
    // adding a state_update inherits the right routing automatically.
    auto emit = [&](const std::string& env) {
        if (target_sid.empty()) pushEvent(env);
        else                    pushEventTo(target_sid, env);
    };
    auto* a = bootstrap::agent();
    if (!a) return;

    // Restore runtime state from persisted settings BEFORE we emit any
    // state_update envelopes. That way the active_model / mode pushes
    // below reflect the user's saved choices, not the volatile RAM
    // defaults from when the Agent was constructed.
    applyEffectiveToRuntime(settings::resolveEffectiveSettings());

    if (auto* prov = a->provider()) {
        auto models = prov->models();
        std::string activeId = prov->active_model_id();

        // active_model — currently selected model details (used by the header pill).
        {
            nlohmann::json v;
            v["provider_id"]   = prov->id();
            v["provider_name"] = prov->display_name();
            v["has_api_key"]   = prov->has_api_key();
            v["effort"]        = effortToStr(prov->effort());
            for (const auto& m : models) {
                if (m.id == activeId) {
                    auto info = modelInfoJson(m);
                    for (auto it = info.begin(); it != info.end(); ++it) {
                        v[it.key()] = it.value();
                    }
                    break;
                }
            }
            emit(buildStateUpdateEnvelope(turnId, "active_model", std::move(v)));
        }

        // available_models — flatten every model from every registered
        // provider so the unified picker shows all options. Each entry
        // includes provider_id + provider_name so the viewer can group
        // them and provider_switch can find the right owner.
        {
            nlohmann::json arr = nlohmann::json::array();
            for (auto* p : bootstrap::providers()) {
                for (const auto& m : p->models()) {
                    nlohmann::json e = modelInfoJson(m);
                    e["provider_id"]   = p->id();
                    e["provider_name"] = p->display_name();
                    arr.push_back(std::move(e));
                }
            }
            emit(buildStateUpdateEnvelope(turnId, "available_models", std::move(arr)));
        }

        // custom_providers — list user-configured custom OpenAI-compatible
        // providers so the Settings UI can show them with delete buttons.
        {
            nlohmann::json arr = nlohmann::json::array();
            for (auto* p : bootstrap::providers()) {
                if (p->id() == "anthropic") continue;
                auto* oc = dynamic_cast<provider::OpenAiCompatible*>(p);
                if (!oc) continue;
                const auto& c = oc->config();
                nlohmann::json e;
                e["id"]           = c.id;
                e["display_name"] = c.display_name;
                e["base_url"]     = c.base_url;
                e["model_id"]     = c.model_id;
                e["model_display"]= c.model_display;
                e["has_api_key"]  = !c.api_key.empty();
                arr.push_back(std::move(e));
            }
            emit(buildStateUpdateEnvelope(turnId, "custom_providers", std::move(arr)));
        }
    }

    // Initial provider-status badges. For each registered provider, emit
    // a key-source hint ("keystore" / "env" / "none") so the UI can
    // distinguish the DPAPI-backed entry from the env-var fallback for
    // Anthropic, and from the legacy plaintext in custom_providers.json.
    auto emitStatus = [&](provider::IProvider* p) {
        bool inKeystore = false;
        if (auto* ks = bootstrap::keystore()) inKeystore = ks->has(p->id());
        bool hasAnyKey = p->has_api_key();
        nlohmann::json v;
        if (!hasAnyKey)         v["status"] = "not_set";
        else                    v["status"] = "set_untested";
        v["message"]    = "";
        v["key_source"] = inKeystore ? "keystore"
                                     : (hasAnyKey ? "fallback" : "none");
        emit(buildStateUpdateEnvelope(turnId,
            "provider_status." + std::string(p->id()), std::move(v)));
    };
    for (auto* p : bootstrap::providers()) emitStatus(p);

    // Session stats (currently zero on mount).
    {
        auto s = a->stats();
        nlohmann::json v;
        v["input_tokens"]                  = s.input_tokens;
        v["output_tokens"]                 = s.output_tokens;
        v["cache_read_input_tokens"]       = s.cache_read_input_tokens;
        v["cache_creation_input_tokens"]   = s.cache_creation_input_tokens;
        v["turns"]                         = s.turns;
        v["cost_usd"]                      = s.cost_usd;
        emit(buildStateUpdateEnvelope(turnId, "session_stats", std::move(v)));
    }

    // User-rules preview + path (first ~10 lines).
    {
        std::string rules = agent::loadUserRules();
        std::string preview;
        int lines = 0;
        for (char c : rules) {
            if (c == '\n') {
                ++lines;
                if (lines >= 10) break;
            }
            preview.push_back(c);
        }
        nlohmann::json v;
        v["path"]    = agent::userRulesPath();
        v["preview"] = preview;
        v["bytes"]   = (int)rules.size();
        emit(buildStateUpdateEnvelope(turnId, "user_rules", std::move(v)));
    }

    // Session — resolve current model's session key under the viewer's
    // chosen strategy, load on-disk history if any, hand it to the
    // agent, and push to the viewer so the chat surface rehydrates.
    //
    // Fallback ladder (a viewer reload — refresh, resize, panel re-dock —
    // can happen mid-conversation, so the agent may already hold the
    // authoritative history in memory when no on-disk save has happened
    // yet):
    //   1. If the on-disk session file loads, push its messages and
    //      replace the agent's history with it.
    //   2. Else, if the agent's in-memory conversation is non-empty,
    //      push those messages. This is the path a mid-turn refresh or
    //      a fresh model (no save yet) takes.
    //   3. Otherwise, declare "fresh chat" so the viewer shows the
    //      placeholder.
    {
        std::string key = agent::currentSessionKey(strategy, customSessionKey);
        a->setSessionKey(key);

        nlohmann::json info;
        info["key"]            = key;
        info["strategy"]       = agent::strategyName(strategy);
        info["custom_key"]     = customSessionKey;
        info["session_file"]   = agent::sessionFilePath(key);
        info["sessions_dir"]   = agent::sessionsDir();
        info["anchor_missing"] = key.empty();

        auto messagesToJson = [](const std::vector<agent::Message>& msgs) {
            nlohmann::json arr = nlohmann::json::array();
            for (const auto& m : msgs) {
                nlohmann::json e;
                switch (m.role) {
                    case agent::Role::User:      e["role"] = "user"; break;
                    case agent::Role::Assistant: e["role"] = "assistant"; break;
                    case agent::Role::Tool:      e["role"] = "tool"; break;
                    case agent::Role::System:    e["role"] = "system"; break;
                }
                e["text"] = m.text;
                arr.push_back(std::move(e));
            }
            return arr;
        };

        // ANCHOR GUARD: currentSessionKey() returns "" when there's no
        // in-model ModelerAI object to anchor against (panel opened
        // before addmodelerai ran, or no model open at all). In that
        // case there's nothing to load and nothing to save — the chat
        // is purely in-memory until the user runs addmodelerai. The
        // viewer renders the "no anchor — run addmodelerai" banner
        // based on info["anchor_missing"].
        std::optional<agent::Conversation> loaded;
        if (key.empty()) {
            consolePrint("[ModelerAI] session restore SKIPPED (no anchor — no in-model ModelerAI object)\n");
        } else {
            loaded = agent::loadSession(key);
        }
        // Capture the current modeldir so the Conversation we hand to
        // the agent knows which .fsm it belongs to. The autosave path
        // then preserves this field on subsequent saves so the Session
        // Memory UI can show "this session is for C:\foo\bar\" etc.
        std::string currentModelDir;
        try {
            const char* d = modeldir_cstr();
            if (d) currentModelDir = d;
        } catch (...) {}

        agent::Conversation effective;
        effective.model_path = currentModelDir;

        if (loaded.has_value()) {
            int n = (int)loaded->messages.size();
            info["messages"] = messagesToJson(loaded->messages);
            info["loaded"]   = true;
            info["count"]    = n;
            effective = std::move(*loaded);
            effective.model_path = currentModelDir;  // refresh stale path from older saves
            consolePrint(("[ModelerAI] session loaded key=" + key
                          + " messages=" + std::to_string(n) + "\n"));
        } else {
            // Fall back to whatever's in memory. This is what a viewer
            // refresh during an unsaved session sees.
            agent::Conversation mem = a->conversationCopy();
            int n = (int)mem.messages.size();
            if (n > 0) {
                info["messages"] = messagesToJson(mem.messages);
                info["loaded"]   = true;
                info["count"]    = n;
                consolePrint(("[ModelerAI] no disk session for key=" + key
                              + "; using in-memory history (" + std::to_string(n)
                              + " messages)\n"));
            } else {
                info["loaded"]   = false;
                info["count"]    = 0;
                info["messages"] = nlohmann::json::array();
                consolePrint(("[ModelerAI] no session for key=" + key + " (fresh chat)\n"));
            }
        }
        // Always replace — clears any prior in-memory conversation. Critical
        // for the model-switch re-anchor flow: if the user opens a different
        // .fsm and the new model has no saved session, we must NOT keep
        // showing the previous model's chat in the panel.
        a->replaceConversation(std::move(effective));

        emit(buildStateUpdateEnvelope(turnId, "session", std::move(info)));
    }

    // Turn-in-flight flag. A viewer refresh / resize during an active
    // turn must not re-enable the send button — the agent is still
    // working. Push the current state so the viewer can lock its
    // composer back down.
    {
        nlohmann::json v;
        v["in_flight"] = a->turnInFlight();
        emit(buildStateUpdateEnvelope(turnId, "turn_in_flight", std::move(v)));
    }

    // mode — Ask / Plan / Apply selection. Drives the header segmented
    // control state on mount.
    {
        nlohmann::json v;
        v["mode"] = std::string(agent::modeName(a->mode()));
        emit(buildStateUpdateEnvelope(turnId, "mode", std::move(v)));
    }

    // settings — effective resolved view + the raw global / project
    // layers (so the UI can render scope toggles + show which scope
    // currently provides each value).
    {
        auto eff     = settings::resolveEffectiveSettings();
        auto global  = settings::loadGlobalSettings();
        auto project = settings::loadProjectSettings();
        nlohmann::json v;
        v["effective"]     = settings::toJson(eff);
        v["global"]        = global;
        v["project"]       = project;
        v["project_avail"] = settings::projectStorageAvailable();
        emit(buildStateUpdateEnvelope(turnId, "settings", std::move(v)));
    }

    // Slash-command catalog — fuels the autocomplete popover in the viewer.
    {
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& c : agent::commandCatalog()) {
            nlohmann::json e;
            e["name"]        = c.name;
            e["description"] = c.description;
            e["details"]     = c.details;
            e["usage"]       = c.usage;
            nlohmann::json aliases = nlohmann::json::array();
            for (const auto& a : c.aliases) aliases.push_back(a);
            e["aliases"] = std::move(aliases);
            if (c.has_args) {
                nlohmann::json args;
                args["placeholder"] = c.args.placeholder;
                nlohmann::json choices = nlohmann::json::array();
                for (const auto& ch : c.args.choices) {
                    choices.push_back({
                        {"value",       ch.value},
                        {"label",       ch.label},
                        {"description", ch.description}
                    });
                }
                args["choices"] = std::move(choices);
                e["args"] = std::move(args);
            }
            arr.push_back(std::move(e));
        }
        emit(buildStateUpdateEnvelope(turnId, "slash_commands", std::move(arr)));
    }

    // remote_status — so a fresh viewer (panel just reopened, server
    // already running) sees the current truth instead of defaulting to
    // "off" and then desyncing when the user runs `/remote on` and the
    // slash dispatcher correctly answers "already running".
    emit(remote::buildRemoteStatusEnvelopeJson(turnId));
}

} // namespace

// Implemented in subscriber.cpp — not in the public header because
// it's a registry-internal fan-out path.
void enqueueToAll(const std::string& envelopeJson);

void pushEvent(const std::string& envelopeJson)
{
    logEnvelope("dll->js", envelopeJson);
    enqueueToAll(envelopeJson);
}

void pushEventTo(std::string_view sid, const std::string& envelopeJson)
{
    logEnvelope(std::string("dll->") + std::string(sid), envelopeJson);
    enqueueTo(sid, envelopeJson);
}

// Public — abi.cpp calls this from ModelerAi_bridgePoll on every FlexSim
// main-thread tick. Detects in-model context changes (different .fsm
// opened, Save-As, addmodelerai run) and triggers reanchorAfterModelChange.
// Internal helpers (LastSeenModel struct, g_lastSeenModel/g_lastSeenMutex
// globals, reanchorAfterModelChange function) live in the anonymous
// namespace above.
void onPollTick()
{
    auto* a = bootstrap::agent();
    if (!a) return;

    // Peek the current model context. Both calls touch the FlexSim tree
    // and are safe here because bridgePoll runs on the main thread.
    std::string currentSessionId = agent::peekSessionIdFromProject();
    std::string currentModelDir;
    try {
        const char* d = modeldir_cstr();
        if (d) currentModelDir = d;
    } catch (...) {}

    LastSeenModel prev;
    {
        std::lock_guard<std::mutex> lk(g_lastSeenMutex);
        prev = g_lastSeenModel;
        g_lastSeenModel.sessionId   = currentSessionId;
        g_lastSeenModel.modelDir    = currentModelDir;
        g_lastSeenModel.initialized = true;
    }

    // First poll just records state — viewer_ready handles the initial
    // anchor itself.
    if (!prev.initialized) return;

    // Nothing changed → nothing to do.
    if (prev.sessionId == currentSessionId && prev.modelDir == currentModelDir) {
        return;
    }

    reanchorAfterModelChange(prev.sessionId, prev.modelDir,
                              currentSessionId, currentModelDir);
}

std::string drainEvents()
{
    return drainSubscriber(kLocalFlexsimSubscriberId);
}

std::string handleEnvelope(std::string_view envelopeJson,
                            std::string_view requesting_sid)
{
    bool from_remote = (requesting_sid != kLocalFlexsimSubscriberId);
    logEnvelope(from_remote ? "remote->dll" : "js->dll", envelopeJson);
    consolePrint(std::string("[ModelerAI] handleEnvelope: received from sid=")
                  + std::string(requesting_sid) + "\n");
    // Diagnostic ring buffer — surfaces in /remote diag so the user
    // can see exactly what each subscriber sent us.
    recordSubscriberRecv(requesting_sid, envelopeJson);

    nlohmann::json env;
    try {
        env = nlohmann::json::parse(envelopeJson);
    } catch (...) {
        consolePrint("[ModelerAI] ERR: envelope JSON parse failed\n");
        return "err:parse";
    }
    if (!env.contains("t") || !env["t"].is_string()) {
        consolePrint("[ModelerAI] ERR: envelope missing 't' field\n");
        return "err:no_type";
    }
    std::string typeName = env["t"].get<std::string>();
    std::string turnId   = env.contains("id") && env["id"].is_string()
                               ? env["id"].get<std::string>()
                               : std::string{};

    BridgeMsgType t = parseType(typeName);
    if (t == BridgeMsgType::UserMessage) {
        if (!env.contains("p") || !env["p"].is_object()) {
            consolePrint("[ModelerAI] ERR: user_message missing payload\n");
            return "err:no_payload";
        }
        const auto& p = env["p"];
        std::string text;
        if (p.contains("text") && p["text"].is_string()) {
            text = p["text"].get<std::string>();
        }

        // Optional image attachments. JS sends them as an array of
        // { name, media_type, data_base64 }. data_base64 is the raw payload
        // (no "data:" prefix). Anthropic supports image/jpeg, image/png,
        // image/gif, image/webp. We forward everything the viewer sends
        // and let the provider adapter filter/reject; that keeps the bridge
        // policy-free.
        std::vector<agent::Attachment> attachments;
        if (p.contains("attachments") && p["attachments"].is_array()) {
            for (const auto& a : p["attachments"]) {
                if (!a.is_object()) continue;
                agent::Attachment att;
                if (a.contains("name")        && a["name"].is_string())        att.name        = a["name"].get<std::string>();
                if (a.contains("media_type")  && a["media_type"].is_string())  att.media_type  = a["media_type"].get<std::string>();
                if (a.contains("data_base64") && a["data_base64"].is_string()) att.data_base64 = a["data_base64"].get<std::string>();
                if (att.data_base64.empty() || att.media_type.empty()) continue;
                attachments.push_back(std::move(att));
            }
        }

        // Empty text is still valid when at least one image is attached
        // ("look at this and tell me what you see").
        if (text.empty() && attachments.empty()) {
            consolePrint("[ModelerAI] ERR: user_message empty text + no attachments\n");
            return "err:empty_text";
        }
        // Prompt-content echo is privacy-sensitive (model contents may
        // include proprietary data). Always log the fact that a message
        // arrived + its length, but ONLY include the text preview when
        // the user has explicitly enabled show_debug_info.
        {
            auto eff = settings::resolveEffectiveSettings();
            if (eff.show_debug_info) {
                consolePrint("[ModelerAI] user_message text=\"" + text.substr(0, 60) + (text.size() > 60 ? "...\"\n" : "\"\n"));
            } else {
                consolePrint("[ModelerAI] user_message len=" + std::to_string(text.size()) + " (content gated by show_debug_info)\n");
            }
        }

        auto* a = bootstrap::agent();
        if (!a) {
            consolePrint("[ModelerAI] ERR: bootstrap::agent() is null\n");
            return "err:no_agent";
        }

        // SLASH COMMANDS bypass the LLM entirely. The dispatcher writes
        // assistant_chunk / assistant_done envelopes directly so the
        // viewer renders the response like a normal reply.
        if (agent::isSlashCommand(text)) {
            agent::dispatchSlash(a, turnId, text);
            return "ok";
        }

        // Normal LLM turn: snapshot the FlexSim tree on THIS thread (called
        // from FlexScript = FlexSim main thread). The agent's worker thread
        // never touches the tree.
        consolePrint("[ModelerAI] handleEnvelope: condensing model tree\n");
        std::string condensed = tree::condenseActiveModel();
        consolePrint(std::string("[ModelerAI] condensed size=" + std::to_string(condensed.size()) + " bytes\n"));

        if (!attachments.empty()) {
            consolePrint("[ModelerAI] user_message has " + std::to_string(attachments.size())
                          + " image attachment(s)\n");
        }
        a->startTurn(turnId, std::move(text), std::move(condensed), std::move(attachments));
        consolePrint("[ModelerAI] startTurn dispatched\n");

        // Broadcast the user message to all OTHER subscribers so they see what
        // was just sent (shared-chat fan-out). The originator already shows
        // the message locally when they pressed Send.
        {
            // text was std::move'd into startTurn. Re-read from the original
            // env payload: env and p are both still valid here (not moved).
            const auto& pRef = env["p"];
            std::string origText;
            if (pRef.contains("text") && pRef["text"].is_string())
                origText = pRef["text"].get<std::string>();

            nlohmann::json echoP;
            echoP["text"]   = origText;
            echoP["author"] = deriveAuthor(requesting_sid);

            nlohmann::json echoEnv;
            echoEnv["t"]  = "user_message_broadcast";
            echoEnv["id"] = turnId;
            echoEnv["p"]  = std::move(echoP);
            std::string echoStr = echoEnv.dump();

            auto subs = snapshotSubscribers();
            for (const auto& s : subs) {
                if (s.id == std::string(requesting_sid)) continue;  // skip originator
                enqueueTo(s.id, echoStr);
            }
        }

        return "ok";
    }

    if (t == BridgeMsgType::CancelTurn) {
        auto* a = bootstrap::agent();
        if (!a) return "err:no_agent";
        bool wasInFlight = a->cancelCurrentTurn();
        consolePrint(("[ModelerAI] cancel_turn (in_flight=" + std::to_string((int)wasInFlight) + ")\n"));
        return wasInFlight ? "ok" : "noop";
    }

    if (t == BridgeMsgType::ViewerReady) {
        // viewer_ready is per-client: each viewer (local CEF or remote
        // phone) needs its OWN copy of the initial state to render. We
        // therefore route every state_update inside pushInitialState
        // through pushEventTo(requesting_sid), NOT pushEvent. That
        // eliminates the "phone connect wipes desktop chat" bug — the
        // phone's response no longer touches the desktop's queue.
        //
        // Only the LOCAL viewer is allowed to set the agent's session
        // strategy (it's the source of truth for that user-facing
        // preference). A remote client supplying its localStorage values
        // would clobber the desktop's choice; we read the agent's current
        // strategy in that case so the response reflects reality.
        agent::SessionStrategy strategy = agent::SessionStrategy::PerFile;
        std::string customKey;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("session_strategy") && p["session_strategy"].is_string()) {
                strategy = agent::parseStrategy(p["session_strategy"].get<std::string>());
            }
            if (p.contains("custom_session_key") && p["custom_session_key"].is_string()) {
                customKey = p["custom_session_key"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] viewer_ready: from_remote=" + std::string(from_remote ? "1" : "0")
                      + " sid=" + std::string(requesting_sid)
                      + " strategy=" + std::string(agent::strategyName(strategy))
                      + (customKey.empty() ? "" : " custom_key=" + customKey) + "\n"));

        auto* a = bootstrap::agent();
        if (!from_remote && a) {
            // Local viewer is authoritative for the strategy choice.
            a->setSessionStrategy(std::string(agent::strategyName(strategy)), customKey);
        } else if (from_remote && a) {
            // Remote — ignore claimed strategy, use what's actually loaded.
            strategy  = agent::parseStrategy(a->sessionStrategy());
            customKey = a->sessionCustomKey();
        }
        pushInitialState(turnId, strategy, customKey, /*target_sid*/ requesting_sid);
        return "ok";
    }

    if (t == BridgeMsgType::ListSessions) {
        consolePrint("[ModelerAI] list_sessions\n");
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& s : agent::listSessions()) {
            nlohmann::json e;
            e["key"]           = s.key;
            e["model_path"]    = s.model_path;
            e["message_count"] = s.message_count;
            e["last_saved_ms"] = s.last_saved_ms;
            e["bytes"]         = s.bytes;
            arr.push_back(std::move(e));
        }
        pushEvent(buildStateUpdateEnvelope(turnId, "all_sessions", std::move(arr)));
        return "ok";
    }

    if (t == BridgeMsgType::DeleteSession) {
        // Manual cleanup from Settings → Session Memory. Deletes one file
        // by key. If the deleted session happened to be the agent's
        // current key, the in-memory chat keeps running but the on-disk
        // copy is gone — next autosave will re-create the file (so this
        // is a "wipe history but keep chatting" affordance, intentional).
        std::string key;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("key") && p["key"].is_string()) key = p["key"].get<std::string>();
        }
        consolePrint(("[ModelerAI] delete_session key=" + key + "\n"));
        if (key.empty()) {
            pushEvent(buildErrorEnvelope(turnId, "invalid_delete",
                "delete_session requires a key."));
            return "ok";
        }
        bool ok = agent::deleteSession(key);
        // Re-push the list so the UI refreshes regardless of whether
        // the file actually existed (idempotent UX).
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& s : agent::listSessions()) {
            nlohmann::json e;
            e["key"]           = s.key;
            e["model_path"]    = s.model_path;
            e["message_count"] = s.message_count;
            e["last_saved_ms"] = s.last_saved_ms;
            e["bytes"]         = s.bytes;
            arr.push_back(std::move(e));
        }
        pushEvent(buildStateUpdateEnvelope(turnId, "all_sessions", std::move(arr)));
        nlohmann::json v;
        v["ok"]      = ok;
        v["key"]     = key;
        v["message"] = ok ? "Session deleted." : "Session not found (already gone?).";
        pushEvent(buildStateUpdateEnvelope(turnId, "session_delete_result", std::move(v)));
        return "ok";
    }

    if (t == BridgeMsgType::DeleteAllSessions) {
        // "Clear all session memory" — wipes every .dat under sessionsDir.
        // Doesn't touch keystore, custom_providers.json, or any other
        // state. Used from Settings → Session Memory → Delete all.
        consolePrint("[ModelerAI] delete_all_sessions\n");
        int deleted = 0;
        int failed  = 0;
        for (const auto& s : agent::listSessions()) {
            if (agent::deleteSession(s.key)) ++deleted; else ++failed;
        }
        consolePrint(("[ModelerAI] delete_all_sessions: deleted=" + std::to_string(deleted)
                      + " failed=" + std::to_string(failed) + "\n"));
        // Refresh the (now likely empty) list.
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& s : agent::listSessions()) {
            nlohmann::json e;
            e["key"]           = s.key;
            e["model_path"]    = s.model_path;
            e["message_count"] = s.message_count;
            e["last_saved_ms"] = s.last_saved_ms;
            e["bytes"]         = s.bytes;
            arr.push_back(std::move(e));
        }
        pushEvent(buildStateUpdateEnvelope(turnId, "all_sessions", std::move(arr)));
        nlohmann::json v;
        v["ok"]      = (failed == 0);
        v["deleted"] = deleted;
        v["failed"]  = failed;
        v["message"] = "Deleted " + std::to_string(deleted) + " session(s)"
                       + (failed ? " (" + std::to_string(failed) + " failed)" : "") + ".";
        pushEvent(buildStateUpdateEnvelope(turnId, "session_delete_result", std::move(v)));
        return "ok";
    }

    if (t == BridgeMsgType::ModeChange) {
        std::string mode;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("mode") && p["mode"].is_string()) {
                mode = p["mode"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] mode_change: " + mode + "\n"));
        auto* a = bootstrap::agent();
        if (a) a->setMode(agent::parseMode(mode));
        // Persist — mode is per-.fsm by convention. Falls back to
        // global if project storage isn't available.
        nlohmann::json patch;
        patch["mode"] = mode;
        if (settings::projectStorageAvailable()) {
            settings::patchProjectSettings(patch);
        } else {
            settings::patchGlobalSettings(patch);
        }
        // Echo back so the viewer can confirm the current mode.
        nlohmann::json v;
        v["mode"] = std::string(agent::modeName(a ? a->mode() : agent::AgentMode::Ask));
        pushEvent(buildStateUpdateEnvelope(turnId, "mode", std::move(v)));
        pushSettingsUpdate(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::ToolApprovalResponse) {
        std::string interactionId;
        nlohmann::json payload = nlohmann::json::object();
        if (env.contains("p") && env["p"].is_object()) {
            payload = env["p"];
            if (payload.contains("interaction_id") && payload["interaction_id"].is_string()) {
                interactionId = payload["interaction_id"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] tool_approval_response id=" + interactionId + "\n"));
        if (auto* a = bootstrap::agent()) {
            a->interactionBroker().resolve(interactionId, std::move(payload));
        }
        // Tell every OTHER viewer to dismiss its modal for this interaction.
        // The original device already cleared it client-side via the
        // resolveApproval handler, but a phone and a desktop showing the
        // same approval need the loser of the race to clean up its UI.
        // Broadcast (not pushEventTo) because we don't know which subscribers
        // are showing the modal; each viewer's JS checks by interaction_id.
        nlohmann::json resolvedEnv;
        resolvedEnv["t"]  = "interaction_resolved";
        resolvedEnv["id"] = turnId;
        nlohmann::json rp;
        rp["interaction_id"] = interactionId;
        rp["kind"]           = "tool_approval";
        resolvedEnv["p"] = std::move(rp);
        pushEvent(resolvedEnv.dump());
        return "ok";
    }

    if (t == BridgeMsgType::SetSetting) {
        std::string key, scope;
        nlohmann::json value;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("key")   && p["key"].is_string())   key   = p["key"].get<std::string>();
            if (p.contains("scope") && p["scope"].is_string()) scope = p["scope"].get<std::string>();
            if (p.contains("value")) value = p["value"];
        }
        consolePrint(("[ModelerAI] set_setting key=" + key + " scope=" + scope + "\n"));
        if (key.empty() || (scope != "global" && scope != "project")) {
            pushEvent(buildErrorEnvelope(turnId, "invalid_set_setting",
                "set_setting requires key + scope in {\"global\",\"project\"}."));
            return "ok";
        }
        nlohmann::json patch;
        patch[key] = value;
        bool ok = false;
        if (scope == "global") {
            ok = settings::patchGlobalSettings(patch);
        } else {
            if (!settings::projectStorageAvailable()) {
                pushEvent(buildErrorEnvelope(turnId, "no_project_storage",
                    "Project settings need the in-model ModelerAI object. "
                    "Run addmodelerai first (or open the panel)."));
                return "ok";
            }
            ok = settings::patchProjectSettings(patch);
        }
        if (!ok) {
            pushEvent(buildErrorEnvelope(turnId, "set_setting_failed",
                "Couldn't persist setting '" + key + "' at " + scope + " scope."));
            return "ok";
        }
        // Apply to runtime immediately so the next turn uses the new
        // setting without waiting for the viewer to round-trip.
        applyEffectiveToRuntime(settings::resolveEffectiveSettings());
        pushSettingsUpdate(turnId);
        // Some keys also need a follow-up push beyond settings: the
        // active_model envelope drives the header pill, for example.
        // Cheapest correct answer: re-fire the full initial-state push
        // so every dependent view (active_model, available_models, etc.)
        // re-syncs.
        pushInitialStateStored(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::RequestSettings) {
        consolePrint("[ModelerAI] request_settings\n");
        pushSettingsUpdate(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::ExportConversation) {
        // Snapshot the current conversation and write it to a timestamped
        // Markdown file. Includes user prompts, assistant text, and every
        // tool call's args + result so the modeler can paste a single file
        // link when diagnosing the AI's reasoning / tool choices.
        auto* a = bootstrap::agent();
        if (!a) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "agent_unavailable";
            v["message"] = "Agent not initialized; nothing to export.";
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:no_agent";
        }
        agent::Conversation conv = a->conversationCopy();
        if (conv.messages.empty()) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "empty_conversation";
            v["message"] = "Nothing to export — no messages in this session yet.";
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:empty";
        }

        // Format a timestamp for both the filename and the header.
        auto now   = std::chrono::system_clock::now();
        auto nowT  = std::chrono::system_clock::to_time_t(now);
        std::tm tmLocal{};
#ifdef _WIN32
        localtime_s(&tmLocal, &nowT);
#else
        localtime_r(&nowT, &tmLocal);
#endif
        char fileStamp[64], headerStamp[64];
        std::strftime(fileStamp,   sizeof(fileStamp),   "%Y-%m-%d_%H%M%S", &tmLocal);
        std::strftime(headerStamp, sizeof(headerStamp), "%Y-%m-%d %H:%M:%S", &tmLocal);

        // Render the conversation as Markdown.
        auto roleName = [](agent::Role r) -> const char* {
            switch (r) {
                case agent::Role::User:      return "user";
                case agent::Role::Assistant: return "assistant";
                case agent::Role::Tool:      return "tool";
                case agent::Role::System:    return "system";
            }
            return "unknown";
        };
        // Try to pretty-print a JSON string. If it doesn't parse, fall back
        // to the raw input so we still produce something useful.
        auto prettyJson = [](const std::string& raw) -> std::string {
            if (raw.empty()) return "(empty)";
            try {
                auto j = nlohmann::json::parse(raw);
                return j.dump(2);
            } catch (...) {
                return raw;
            }
        };

        std::ostringstream md;
        md << "# ModelerAI conversation export\n\n";
        md << "- Exported: " << headerStamp << "\n";
        if (!conv.id.empty())         md << "- Conversation id: " << conv.id << "\n";
        if (!conv.model_path.empty()) md << "- Model path: " << conv.model_path << "\n";
        if (auto* p = a->provider()) {
            md << "- Provider: " << p->display_name() << "\n";
            md << "- Active model: " << p->active_model_id() << "\n";
        }
        md << "- Session key: " << a->sessionKey() << "\n";
        md << "- Strategy: " << a->sessionStrategy() << "\n";
        md << "- Messages: " << conv.messages.size() << "\n";
        {
            auto s = a->stats();
            md << "- Stats: turns=" << s.turns
               << ", input_tokens=" << s.input_tokens
               << ", output_tokens=" << s.output_tokens
               << ", cost_usd=" << s.cost_usd << "\n";
        }
        md << "\n---\n\n";

        for (size_t i = 0; i < conv.messages.size(); ++i) {
            const auto& m = conv.messages[i];
            md << "## [" << roleName(m.role) << "]";
            if (m.role == agent::Role::Tool && m.tool_call_id.has_value()) {
                md << " (response to " << *m.tool_call_id << ")";
            }
            md << "\n\n";
            if (!m.text.empty()) {
                md << m.text << "\n\n";
            }
            // Tool calls — render every call regardless of role. The same
            // ToolCallRecord appears once on the assistant (args, no result)
            // and once on the tool message (with result).
            for (const auto& c : m.tool_calls) {
                md << "### tool: " << c.name;
                if (!c.call_id.empty()) md << "  `" << c.call_id << "`";
                md << "\n\n";
                if (!c.args_json.empty()) {
                    md << "**args**\n\n```json\n" << prettyJson(c.args_json) << "\n```\n\n";
                }
                if (!c.result_json.empty()) {
                    md << "**result** " << (c.ok ? "ok" : "FAILED") << "\n\n";
                    md << "```json\n" << prettyJson(c.result_json) << "\n```\n\n";
                    if (!c.ok && !c.error_message.empty()) {
                        md << "**error**: " << c.error_message << "\n\n";
                    }
                }
            }
        }

        // Build the output path: <appDataDir>/exports/<stamp>.md.
        std::string appDir = paths::appDataDir();
        if (appDir.empty()) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "appdata_unavailable";
            v["message"] = "Could not resolve %APPDATA%/PRACSIM/ModelerAI/.";
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:no_appdata";
        }
        std::filesystem::path outDir = std::filesystem::path(appDir) / "exports";
        std::error_code ec;
        std::filesystem::create_directories(outDir, ec);
        if (ec) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "mkdir_failed";
            v["message"] = "Could not create exports dir: " + ec.message();
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:mkdir";
        }
        std::string filename = std::string("conversation_") + fileStamp + ".md";
        std::filesystem::path outPath = outDir / filename;

        std::string body = md.str();
        std::ofstream f(outPath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!f.is_open()) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "write_failed";
            v["message"] = "Could not open '" + outPath.string() + "' for writing.";
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:open";
        }
        f.write(body.data(), static_cast<std::streamsize>(body.size()));
        f.close();
        if (f.fail()) {
            nlohmann::json v;
            v["ok"]      = false;
            v["error"]   = "write_failed";
            v["message"] = "Write to '" + outPath.string() + "' failed.";
            pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
            return "err:write";
        }

        consolePrint(("[ModelerAI] conversation exported to " + outPath.string()
                      + " (" + std::to_string(body.size()) + " bytes)\n"));

        nlohmann::json v;
        v["ok"]            = true;
        v["path"]          = outPath.string();
        v["dir"]           = outDir.string();
        v["bytes"]         = (int)body.size();
        v["message_count"] = (int)conv.messages.size();
        pushEvent(buildStateUpdateEnvelope(turnId, "conversation_export", std::move(v)));
        return "ok";
    }

    if (t == BridgeMsgType::ResetSettings) {
        // Wipe user preferences in global settings back to compiled defaults.
        // Preserve audit-trail keys (onboarding_completed_*,
        // custom_provider_acks, custom_providers).
        // Project-scope settings are left alone — user can delete the in-model
        // node manually if they want a per-project reset.
        auto current = settings::loadGlobalSettings();
        nlohmann::json preserved = nlohmann::json::object();
        const char* keepKeys[] = {
            "onboarding_completed_version",
            "onboarding_completed_at_ms",
            "custom_provider_acks",
            "custom_providers"
        };
        for (auto* k : keepKeys) {
            if (current.contains(k)) preserved[k] = current[k];
        }
        bool ok = settings::saveGlobalSettings(preserved);
        consolePrint(("[ModelerAI] reset_settings ok=" + std::to_string((int)ok) + "\n"));
        // Re-apply defaults to the agent, then re-push initial state so
        // every UI mirror flips back.
        applyEffectiveToRuntime(settings::resolveEffectiveSettings());
        pushSettingsUpdate(turnId);
        pushInitialStateStored(turnId);
        return ok ? "ok" : "err:write_failed";
    }

    if (t == BridgeMsgType::CompleteOnboarding) {
        long long nowMs = (long long)
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count();
        nlohmann::json patch;
        patch["onboarding_completed_version"] = std::string(settings::kCurrentOnboardingVersion);
        patch["onboarding_completed_at_ms"]   = nowMs;
        bool ok = settings::patchGlobalSettings(patch);
        consolePrint(("[ModelerAI] complete_onboarding version=" + std::string(settings::kCurrentOnboardingVersion)
                      + " ok=" + std::to_string((int)ok) + "\n"));
        pushSettingsUpdate(turnId);
        return ok ? "ok" : "err:write_failed";
    }


    if (t == BridgeMsgType::AskUserQuestionResponse) {
        std::string interactionId;
        nlohmann::json payload = nlohmann::json::object();
        if (env.contains("p") && env["p"].is_object()) {
            payload = env["p"];
            if (payload.contains("interaction_id") && payload["interaction_id"].is_string()) {
                interactionId = payload["interaction_id"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] ask_user_question_response id=" + interactionId + "\n"));
        if (auto* a = bootstrap::agent()) {
            a->interactionBroker().resolve(interactionId, std::move(payload));
        }
        // Same shape as tool_approval_response: broadcast a dismiss so
        // viewers that aren't the one who answered can disable the
        // question bubble's input controls.
        nlohmann::json resolvedEnv;
        resolvedEnv["t"]  = "interaction_resolved";
        resolvedEnv["id"] = turnId;
        nlohmann::json rp;
        rp["interaction_id"] = interactionId;
        rp["kind"]           = "ask_user_question";
        resolvedEnv["p"] = std::move(rp);
        pushEvent(resolvedEnv.dump());
        return "ok";
    }

    if (t == BridgeMsgType::ProviderSwitch) {
        std::string modelId;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("model") && p["model"].is_string()) {
                modelId = p["model"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] provider_switch: " + modelId + "\n"));

        // Find which registered provider owns this model id.
        auto* owner = bootstrap::findProviderForModel(modelId);
        if (!owner) {
            pushEvent(buildErrorEnvelope(turnId, "unknown_model",
                "Model '" + modelId + "' is not registered with any provider."));
            return "ok";
        }
        // Set it as the agent's active provider, and tell that provider to
        // use this specific model.
        bootstrap::setActiveProvider(owner);
        owner->set_active_model(modelId);

        // Persist — active model is a user-level preference by default
        // (carries across .fsms). A future Settings UI scope toggle can
        // override per-project.
        nlohmann::json patch;
        patch["active_provider_id"] = std::string(owner->id());
        patch["active_model_id"]    = modelId;
        settings::patchGlobalSettings(patch);

        // Push updated active_model so the header pill + Settings refresh.
        nlohmann::json v;
        v["provider_id"]   = owner->id();
        v["provider_name"] = owner->display_name();
        v["has_api_key"]   = owner->has_api_key();
        v["effort"]        = effortToStr(owner->effort());
        for (const auto& m : owner->models()) {
            if (m.id == owner->active_model_id()) {
                auto info = modelInfoJson(m);
                for (auto it = info.begin(); it != info.end(); ++it) {
                    v[it.key()] = it.value();
                }
                break;
            }
        }
        pushEvent(buildStateUpdateEnvelope(turnId, "active_model", std::move(v)));
        return "ok";
    }

    if (t == BridgeMsgType::AddCustomProvider) {
        provider::CustomProviderConfig cfg;
        std::string incomingKey;  // captured separately so we route it to the keystore
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            auto str  = [&](const char* k, std::string& out) {
                if (p.contains(k) && p[k].is_string()) out = p[k].get<std::string>();
            };
            auto num  = [&](const char* k, double& out) {
                if (p.contains(k) && p[k].is_number()) out = p[k].get<double>();
            };
            auto intf = [&](const char* k, int& out) {
                if (p.contains(k) && p[k].is_number_integer()) out = p[k].get<int>();
            };
            str("display_name",  cfg.display_name);
            str("base_url",      cfg.base_url);
            str("api_key",       incomingKey);            // NOT into cfg — goes to keystore
            str("model_id",      cfg.model_id);
            str("model_display", cfg.model_display);
            intf("context_tokens", cfg.context_tokens);
            num("input_usd_per_mtok",       cfg.input_usd_per_mtok);
            num("output_usd_per_mtok",      cfg.output_usd_per_mtok);
            num("cache_write_usd_per_mtok", cfg.cache_write_usd_per_mtok);
            num("cache_read_usd_per_mtok",  cfg.cache_read_usd_per_mtok);

            // Persist the acknowledgement payload (if any) into global
            // settings under custom_provider_acks. This is our audit
            // trail proving the user accepted the third-party's terms
            // when they added the provider. Keyed by base_url so we
            // tolerate display-name edits without losing the record.
            if (p.contains("acknowledgement") && p["acknowledgement"].is_object()) {
                auto current = settings::loadGlobalSettings();
                if (!current.contains("custom_provider_acks") ||
                    !current["custom_provider_acks"].is_object())
                {
                    current["custom_provider_acks"] = nlohmann::json::object();
                }
                std::string key = cfg.base_url.empty() ? cfg.display_name : cfg.base_url;
                current["custom_provider_acks"][key] = p["acknowledgement"];
                settings::saveGlobalSettings(current);
                consolePrint(("[ModelerAI] custom_provider_ack logged for '" + key + "'\n"));
            }
        }
        consolePrint(("[ModelerAI] add_custom_provider name='" + cfg.display_name
                      + "' base=" + cfg.base_url + " model=" + cfg.model_id + "\n"));

        auto* added = bootstrap::addCustomProvider(std::move(cfg));
        if (!added) {
            pushEvent(buildErrorEnvelope(turnId, "invalid_provider",
                "Add failed: display name, base URL, and model id are required."));
            return "ok";
        }

        // Route the key (if supplied) directly into the keystore — never
        // touches disk in plaintext.
        if (!incomingKey.empty()) {
            if (auto* ks = bootstrap::keystore()) {
                ks->set(added->id(), incomingKey);
                ks->save();
            }
        }
        // Re-fire initial state so the viewer's lists refresh. Use the same
        // strategy/custom-key the session previously had — we don't need
        // to reload the conversation; just refresh provider/model lists.
        nlohmann::json ack;
        ack["ok"]      = true;
        ack["id"]      = added->id();
        ack["message"] = "Added " + added->display_name();
        pushEvent(buildStateUpdateEnvelope(turnId, "add_custom_provider_result", std::move(ack)));
        // Re-emit available_models + custom_providers without a full
        // reload of the chat.
        pushInitialStateStored(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::RemoveCustomProvider) {
        std::string id;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("id") && p["id"].is_string()) id = p["id"].get<std::string>();
        }
        consolePrint(("[ModelerAI] remove_custom_provider id=" + id + "\n"));
        if (!bootstrap::removeCustomProvider(id)) {
            pushEvent(buildErrorEnvelope(turnId, "remove_failed",
                "Couldn't remove provider '" + id + "' (not found, or it's the built-in Anthropic adapter)."));
            return "ok";
        }
        // Also drop the keystore entry so no orphan ciphertext lingers.
        if (auto* ks = bootstrap::keystore()) {
            ks->forget(id);
            ks->save();
        }
        pushInitialStateStored(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::SaveApiKey) {
        std::string provId, key;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("provider") && p["provider"].is_string()) provId = p["provider"].get<std::string>();
            if (p.contains("key")      && p["key"].is_string())      key    = p["key"].get<std::string>();
        }
        consolePrint(("[ModelerAI] save_api_key provider=" + provId + " (len=" + std::to_string(key.size()) + ")\n"));
        if (provId.empty() || key.empty()) {
            pushEvent(buildErrorEnvelope(turnId, "invalid_save",
                "save_api_key requires a provider id and a key value."));
            return "ok";
        }
        auto* ks = bootstrap::keystore();
        if (!ks) {
            pushEvent(buildErrorEnvelope(turnId, "no_keystore",
                "Keystore not initialized — restart FlexSim."));
            return "ok";
        }
        ks->set(provId, key);
        if (!ks->save()) {
            pushEvent(buildErrorEnvelope(turnId, "save_failed",
                "Couldn't persist keystore to disk. Check %APPDATA%/PRACSIM/ModelerAI."));
            return "ok";
        }
        // Refresh provider statuses so the badge flips to "set" immediately.
        // We don't auto-test the key — that's the user's next click.
        pushInitialStateStored(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::ForgetApiKey) {
        std::string provId;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("provider") && p["provider"].is_string()) provId = p["provider"].get<std::string>();
        }
        consolePrint(("[ModelerAI] forget_api_key provider=" + provId + "\n"));
        if (provId.empty()) {
            pushEvent(buildErrorEnvelope(turnId, "invalid_forget",
                "forget_api_key requires a provider id."));
            return "ok";
        }
        if (auto* ks = bootstrap::keystore()) {
            ks->forget(provId);
            ks->save();
        }
        pushInitialStateStored(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::EffortChange) {
        std::string effortStr = "off";
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("effort") && p["effort"].is_string()) {
                effortStr = p["effort"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] effort_change: " + effortStr + "\n"));

        auto* a = bootstrap::agent();
        if (!a || !a->provider()) {
            pushEvent(buildErrorEnvelope(turnId, "no_provider", "No provider configured."));
            return "ok";
        }
        a->provider()->set_effort(effortFromStr(effortStr));

        // Persist — effort is global-default by convention.
        nlohmann::json patch;
        patch["effort"] = std::string(effortToStr(a->provider()->effort()));
        settings::patchGlobalSettings(patch);

        // Echo back current effort so the UI reflects it (set_effort can
        // silently no-op if the active model doesn't support thinking).
        nlohmann::json v;
        v["effort"] = effortToStr(a->provider()->effort());
        pushEvent(buildStateUpdateEnvelope(turnId, "effort", std::move(v)));
        pushSettingsUpdate(turnId);
        return "ok";
    }

    if (t == BridgeMsgType::TestProvider) {
        std::string providerId;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("provider") && p["provider"].is_string()) {
                providerId = p["provider"].get<std::string>();
            }
        }
        consolePrint(("[ModelerAI] test_provider: " + providerId + "\n"));

        provider::IProvider* prov = providerId.empty()
            ? bootstrap::activeProvider()
            : bootstrap::findProvider(providerId);
        if (!prov) {
            pushEvent(buildErrorEnvelope(turnId, "unknown_provider",
                "Provider '" + providerId + "' not registered."));
            return "ok";
        }

        // Test on a worker so the FlexScript caller doesn't block on the
        // HTTP round-trip. Result pushed via state_update when done.
        std::thread([prov, turnId]() {
            consolePrint("[ModelerAI] test_provider: running test_key()\n");
            provider::TestKeyResult r = prov->test_key();
            nlohmann::json v;
            if (r.ok) {
                v["status"]  = "authenticated";
                v["message"] = "";
            } else if (r.http_status == 401 || r.http_status == 403) {
                v["status"]  = "invalid";
                v["message"] = r.error_message;
            } else if (r.http_status == 0) {
                v["status"]  = "invalid";
                v["message"] = "Network error: " + r.error_message;
            } else {
                v["status"]  = "invalid";
                v["message"] = r.error_message;
            }
            pushEvent(buildStateUpdateEnvelope(turnId,
                "provider_status." + std::string(prov->id()),
                std::move(v)));
        }).detach();
        return "ok";
    }

    // String-typed envelopes that aren't in the BridgeMsgType enum (yet)
    // are handled by name below.
    if (typeName == "remote_kick") {
        std::string sid;
        if (env.contains("p") && env["p"].is_object()) {
            const auto& p = env["p"];
            if (p.contains("subscriber_id") && p["subscriber_id"].is_string()) {
                sid = p["subscriber_id"].get<std::string>();
            }
        }
        if (!sid.empty()) {
            ModelerAi::remote::kickClient(sid);
            // Re-publish a fresh remote_status so the Settings panel updates.
            ModelerAi::agent::dispatchRemote(nullptr, "kick-followup", "status");
        }
        return "ok";
    }

    // Everything else queued in protocol.h is unsupported in v0.1.
    consolePrint(std::string("[ModelerAI] unsupported envelope type: " + typeName + "\n"));
    pushEvent(buildErrorEnvelope(turnId, "unsupported_in_v0",
        std::string("Bridge type '") + typeName + "' is not implemented in v0.1."));
    return "ok";
}

} // namespace ModelerAi::bridge
