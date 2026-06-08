// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "bootstrap.h"
#include "agent/agent.h"
#include "bridge/bridge.h"
#include "bridge/log.h"
#include "bridge/subscriber.h"
#include "commands/identifier_validator.h"
#include "kb/kb_index.h"
#include "keystore/keystore.h"
#include "mainthread/mainthread.h"
#include "paths.h"
#include "provider/anthropic.h"
#include "provider/builtin_chat.h"
#include "provider/gemini.h"
#include "provider/openai_compatible.h"
#include "remote/remote_server.h"
#include "third_party/json.h"
#include "tools/tool_registry.h"
#include "tools/read/ask_user_question.h"
#include "tools/read/flexsim_readonly_stubs.h"
#include "tools/read/kb_tools.h"
#include "tools/write/modelerai_call.h"
#include "tools/write/run_script.h"

#include <windows.h>     // MoveFileExA + flags
#include <fstream>
#include <memory>
#include <mutex>
#include <sstream>

namespace ModelerAi::bootstrap {

namespace {

std::mutex                                       g_mutex;
std::vector<std::unique_ptr<provider::IProvider>> g_providers;  // built-ins first (Anthropic, OpenAI, Gemini, xAI), customs after
provider::IProvider*                             g_active = nullptr;

// ---------------------------------------------------------------------------
// Curated built-in configurations. Edited in one place so adding/removing
// a model is a single-line change. Pricing snapshot as of 2026-05-27 — see
// docs/architecture/2026-05-27-multi-provider-and-setup-panel.md §3.5.
// ---------------------------------------------------------------------------
provider::BuiltinChatConfig openAiBuiltinConfig()
{
    provider::BuiltinChatConfig c;
    c.id           = "openai";
    c.display_name = "OpenAI (GPT)";
    c.base_url     = "https://api.openai.com/v1";
    c.env_var      = "OPENAI_API_KEY";
    c.default_model = "gpt-4o-mini";
    c.models = {
        provider::ModelInfo{
            "gpt-4o-mini", "GPT-4o mini", 128000,
            /*tools*/true, /*vision*/true, /*thinking*/false, /*temp*/true,
            /*in*/0.15,  /*out*/0.60,  /*cw*/0.0, /*cr*/0.075
        },
        provider::ModelInfo{
            "gpt-4o", "GPT-4o", 128000,
            true, true, false, true,
            2.50, 10.00, 0.0, 1.25
        },
        provider::ModelInfo{
            "gpt-4.1", "GPT-4.1", 1000000,
            true, true, false, true,
            2.00, 8.00, 0.0, 0.50
        },
        provider::ModelInfo{
            "o3-mini", "o3-mini (thinking)", 200000,
            true, false, true, true,
            1.10, 4.40, 0.0, 0.55
        },
        provider::ModelInfo{
            "o1", "o1 (thinking)", 200000,
            true, true, true, true,
            15.00, 60.00, 0.0, 7.50
        },
    };
    return c;
}

provider::BuiltinChatConfig xaiBuiltinConfig()
{
    provider::BuiltinChatConfig c;
    c.id           = "xai";
    c.display_name = "xAI (Grok)";
    c.base_url     = "https://api.x.ai/v1";
    c.env_var      = "XAI_API_KEY";
    c.default_model = "grok-3-mini";
    c.models = {
        provider::ModelInfo{
            "grok-3-mini", "Grok 3 Mini", 131072,
            /*tools*/true, /*vision*/false, /*thinking*/false, /*temp*/true,
            0.30, 0.50, 0.0, 0.0
        },
        provider::ModelInfo{
            "grok-3", "Grok 3", 131072,
            true, false, false, true,
            3.00, 15.00, 0.0, 0.0
        },
        provider::ModelInfo{
            "grok-2-vision-1212", "Grok 2 Vision", 32768,
            true, true, false, true,
            2.00, 10.00, 0.0, 0.0
        },
    };
    return c;
}

provider::BuiltinChatConfig mistralBuiltinConfig()
{
    provider::BuiltinChatConfig c;
    c.id           = "mistral";
    c.display_name = "Mistral";
    c.base_url     = "https://api.mistral.ai/v1";
    c.env_var      = "MISTRAL_API_KEY";
    c.default_model = "mistral-small-latest";
    c.models = {
        provider::ModelInfo{
            "mistral-small-latest", "Mistral Small", 128000,
            /*tools*/true, /*vision*/false, /*thinking*/false, /*temp*/true,
            0.20, 0.60, 0.0, 0.0
        },
        provider::ModelInfo{
            "mistral-medium-latest", "Mistral Medium", 128000,
            true, false, false, true,
            2.70, 8.10, 0.0, 0.0
        },
        provider::ModelInfo{
            "mistral-large-latest", "Mistral Large", 128000,
            true, false, false, true,
            2.00, 6.00, 0.0, 0.0
        },
        provider::ModelInfo{
            "codestral-latest", "Codestral (code-tuned)", 256000,
            true, false, false, true,
            0.30, 0.90, 0.0, 0.0
        },
    };
    return c;
}

// One-shot cleanup: pre-existing __unsaved__.dat files were created by the
// retired "no-anchor sentinel" path. They cause cross-launch session
// carry-over even after the sentinel is killed. Removing here means a
// user upgrading from an older build doesn't keep loading their last
// no-anchor chat on every FlexSim launch.
void cleanupOrphanUnsavedSession()
{
    std::string path = paths::appDataDir() + "sessions\\__unsaved__.dat";
    if (DeleteFileA(path.c_str())) {
        bridge::consolePrint("[ModelerAI] cleaned up orphan __unsaved__.dat (pre-anchor-fix carry-over)\n");
    }
}
std::unique_ptr<agent::Agent>                    g_agent;
std::unique_ptr<keystore::Keystore>              g_keystore;
std::unique_ptr<tools::ToolRegistry>             g_toolRegistry;
std::unique_ptr<kb::KbIndex>                     g_kbIndex;

provider::CustomProviderConfig configFromJson(const nlohmann::json& j)
{
    provider::CustomProviderConfig c;
    auto str = [&](const char* k, std::string& out) {
        if (j.contains(k) && j[k].is_string()) out = j[k].get<std::string>();
    };
    auto num = [&](const char* k, double& out) {
        if (j.contains(k) && j[k].is_number()) out = j[k].get<double>();
    };
    auto intf = [&](const char* k, int& out) {
        if (j.contains(k) && j[k].is_number_integer()) out = j[k].get<int>();
    };
    auto boolf = [&](const char* k, bool& out) {
        if (j.contains(k) && j[k].is_boolean()) out = j[k].get<bool>();
    };

    str("id",             c.id);
    str("display_name",   c.display_name);
    str("base_url",       c.base_url);
    str("api_key",        c.api_key);
    str("model_id",       c.model_id);
    str("model_display",  c.model_display);
    intf("context_tokens", c.context_tokens);
    boolf("supports_tools",  c.supports_tools);
    boolf("supports_vision", c.supports_vision);
    num("input_usd_per_mtok",       c.input_usd_per_mtok);
    num("output_usd_per_mtok",      c.output_usd_per_mtok);
    num("cache_write_usd_per_mtok", c.cache_write_usd_per_mtok);
    num("cache_read_usd_per_mtok",  c.cache_read_usd_per_mtok);
    return c;
}

nlohmann::json configToJson(const provider::CustomProviderConfig& c)
{
    nlohmann::json j;
    j["id"]                       = c.id;
    j["display_name"]             = c.display_name;
    j["base_url"]                 = c.base_url;
    j["api_key"]                  = c.api_key;
    j["model_id"]                 = c.model_id;
    j["model_display"]            = c.model_display;
    j["context_tokens"]           = c.context_tokens;
    j["supports_tools"]           = c.supports_tools;
    j["supports_vision"]          = c.supports_vision;
    j["input_usd_per_mtok"]       = c.input_usd_per_mtok;
    j["output_usd_per_mtok"]      = c.output_usd_per_mtok;
    j["cache_write_usd_per_mtok"] = c.cache_write_usd_per_mtok;
    j["cache_read_usd_per_mtok"]  = c.cache_read_usd_per_mtok;
    return j;
}

void persistCustomProviders_locked()
{
    std::string path = paths::customProvidersPath();
    if (path.empty()) return;
    nlohmann::json arr = nlohmann::json::array();
    for (const auto& p : g_providers) {
        if (p->id() == "anthropic") continue;  // not user-editable
        auto* oc = dynamic_cast<provider::OpenAiCompatible*>(p.get());
        if (!oc) continue;
        arr.push_back(configToJson(oc->config()));
    }
    nlohmann::json doc;
    doc["version"]   = 1;
    doc["providers"] = std::move(arr);

    std::string tmp = path + ".tmp";
    {
        std::ofstream f(tmp, std::ios::binary);
        if (!f) return;
        f << doc.dump(2);
    }
    MoveFileExA(tmp.c_str(), path.c_str(),
                MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH);
}

void loadCustomProviders_locked()
{
    std::string path = paths::customProvidersPath();
    if (path.empty()) return;
    std::ifstream f(path, std::ios::binary);
    if (!f) return;
    std::ostringstream ss;
    ss << f.rdbuf();
    std::string body = ss.str();
    if (body.empty()) return;

    bool migratedAnyKey = false;
    try {
        auto doc = nlohmann::json::parse(body);
        if (!doc.contains("providers") || !doc["providers"].is_array()) return;
        for (const auto& e : doc["providers"]) {
            auto cfg = configFromJson(e);
            if (cfg.base_url.empty() || cfg.model_id.empty() || cfg.id.empty()) continue;

            // Migration: if this entry still has a plaintext api_key field,
            // move it into the DPAPI keystore and clear the plaintext copy.
            // One-time per provider — subsequent loads find the key in the
            // keystore and the config has empty api_key.
            if (!cfg.api_key.empty() && g_keystore) {
                g_keystore->set(cfg.id, cfg.api_key);
                cfg.api_key.clear();
                migratedAnyKey = true;
                bridge::consolePrint(("[ModelerAI] migrated plaintext key for provider "
                                      + cfg.id + " to keystore\n"));
            }

            g_providers.emplace_back(
                std::make_unique<provider::OpenAiCompatible>(std::move(cfg)));
        }
        bridge::consolePrint(("[ModelerAI] loaded " + std::to_string(g_providers.size() - 1)
                              + " custom provider(s)\n"));
    } catch (...) {
        bridge::consolePrint("[ModelerAI] WARN: custom_providers.json parse failed\n");
    }

    if (migratedAnyKey) {
        if (g_keystore) g_keystore->save();
        persistCustomProviders_locked();  // strip plaintext from disk
    }
}

bool isIdInUse_locked(const std::string& id)
{
    for (const auto& p : g_providers) if (p->id() == id) return true;
    return false;
}

std::string slugify(const std::string& s)
{
    std::string out;
    out.reserve(s.size());
    for (char c : s) {
        if ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9')) out.push_back(c);
        else if (c >= 'A' && c <= 'Z') out.push_back((char)(c - 'A' + 'a'));
        else if (c == ' ' || c == '-' || c == '_') out.push_back('-');
    }
    while (!out.empty() && out.front() == '-') out.erase(out.begin());
    while (!out.empty() && out.back()  == '-') out.pop_back();
    if (out.empty()) out = "custom";
    return out;
}

} // namespace

void initialize()
{
    ModelerAi::bridge::initSubscriberRegistry();
    std::lock_guard<std::mutex> lk(g_mutex);

    // Main-thread task queue — capture the FlexSim main thread ID early.
    // bootstrap::initialize() is called from dllinitialize() which fires
    // on the FlexSim main thread, so this is the right place. Worker
    // threads spawned later can then runAndWait() back to here.
    mainthread::initMainThread();

    // Keystore loads FIRST so loadCustomProviders_locked() can migrate any
    // plaintext keys it finds in custom_providers.json into the keystore.
    g_keystore = std::make_unique<keystore::Keystore>();
    if (!g_keystore->load(paths::appDataDir())) {
        bridge::consolePrint("[ModelerAI] WARN: keystore load failed; running unencrypted\n");
    }

    g_providers.clear();
    g_providers.emplace_back(std::make_unique<provider::Anthropic>());
    g_providers.emplace_back(std::make_unique<provider::BuiltinChatCompletions>(openAiBuiltinConfig()));
    g_providers.emplace_back(std::make_unique<provider::Gemini>());
    g_providers.emplace_back(std::make_unique<provider::BuiltinChatCompletions>(xaiBuiltinConfig()));
    g_providers.emplace_back(std::make_unique<provider::BuiltinChatCompletions>(mistralBuiltinConfig()));
    loadCustomProviders_locked();

    // One-shot cleanup of any leftover __unsaved__.dat from the retired
    // sentinel path. Safe no-op if the file isn't there.
    cleanupOrphanUnsavedSession();

    // Tool registry. The AI's surface is intentionally narrow:
    //   - read-only:  ask_user_question, search_kb, read_topic, list_classes,
    //                 list_commands, plus the deferred read-only stubs
    //                 (get_model_structure / get_object_properties — Domain 0
    //                 retirement target).
    //   - mutating:   run_script (escape hatch) + modelerai_call (single
    //                 bridge into the curated modelerai_* command library
    //                 implemented under ModelerAiDLL/commands/).
    // The individual FlexSim-mutation tools (create_object, set_property,
    // delete_object, etc.) retired on 2026-06-01 — their bodies live as DLL
    // exports invoked via modelerai_call → applicationcommand → the .fsx
    // binding nodes.
    g_toolRegistry = std::make_unique<tools::ToolRegistry>();
    g_toolRegistry->registerTool(std::make_unique<tools::AskUserQuestionTool>());

    // Read-only FlexSim tools (deferred Domain 0 stubs).
    g_toolRegistry->registerTool(std::make_unique<tools::GetModelStructureTool>());
    g_toolRegistry->registerTool(std::make_unique<tools::GetObjectPropertiesTool>());

    // Mutating: the bridge tool + the escape hatch.
    g_toolRegistry->registerTool(std::make_unique<tools::ModelerAiCallTool>());
    g_toolRegistry->registerTool(std::make_unique<tools::RunScriptTool>());

    // Knowledge-base tools — search_kb / read_topic look up the
    // auto-generated FlexScript class + command index that lives under
    // KNOWLEDGE/. list_commands / list_classes pull full per-group /
    // per-module catalog tables on demand (the always-on prefix only
    // carries the compact index — these are the deeper fetches).
    g_toolRegistry->registerTool(std::make_unique<tools::SearchKbTool>());
    g_toolRegistry->registerTool(std::make_unique<tools::ReadTopicTool>());
    g_toolRegistry->registerTool(std::make_unique<tools::ListCommandsTool>());
    g_toolRegistry->registerTool(std::make_unique<tools::ListClassesTool>());

    // KB index — parses KNOWLEDGE/INDEX.json. If the file is missing or
    // malformed we keep the empty instance; search_kb returns a clear
    // "KB unavailable" error and the agent still functions without the KB.
    g_kbIndex = std::make_unique<kb::KbIndex>();
    {
        std::string moduleRoot = paths::moduleInstallDir();
        if (g_kbIndex->load(moduleRoot)) {
            bridge::consolePrint("[ModelerAI] KB index loaded: "
                + std::to_string(g_kbIndex->size()) + " topics\n");
        } else {
            bridge::consolePrint("[ModelerAI] WARN: KB index failed to load from "
                + moduleRoot + "KNOWLEDGE\\INDEX.json — search_kb will be unavailable\n");
        }
    }

    // Identifier catalog — derived from the KB. Used by the run_script
    // pre-execution validator to reject hallucinated command names
    // (createObject, createInstance, etc.) before they hit
    // executestring. Safe to call even if the KB didn't load —
    // validateIdentifiers() fail-opens when the catalog is empty.
    ModelerAi::rebuildIdentifierCatalog();

    // Active provider defaults to Anthropic (index 0). User can /model
    // switch after panel mount.
    g_active = g_providers.front().get();

    g_agent = std::make_unique<agent::Agent>(
        g_active,
        [](const std::string& envelopeJson) { bridge::pushEvent(envelopeJson); });
}

void teardown()
{
    // Tear down the remote HTTP server FIRST — before we take g_mutex or
    // touch the agent. Any in-flight HTTP request must finish (or fail)
    // before the agent it might dispatch to is destroyed below.
    // Done outside the g_mutex lock so the server thread join can't
    // deadlock against a handler that might (in future tasks) call
    // bootstrap getters which themselves take g_mutex.
    ModelerAi::remote::stop();

    std::lock_guard<std::mutex> lk(g_mutex);
    // If a turn is in flight, signal cancel and give the worker a short
    // window to wind down. The worker thread captures `Agent* self` raw
    // and reads atomics + mutexes on it; deleting the agent mid-flight
    // would dereference freed memory.
    if (g_agent) {
        g_agent->cancelCurrentTurn();
        // Poll briefly (up to ~2s) for turn-in-flight to clear. The
        // WinHTTP read loop checks cancel between chunks (sub-100ms in
        // practice), so this almost always returns immediately.
        for (int i = 0; i < 200; ++i) {
            if (!g_agent->turnInFlight()) break;
            Sleep(10);
        }
        // If the worker is still alive after the grace window, we
        // accept the worst case and proceed — better to leak the thread
        // than to hang FlexSim teardown indefinitely.
    }
    g_agent.reset();
    g_providers.clear();
    g_active = nullptr;
    g_keystore.reset();
    g_toolRegistry.reset();
    g_kbIndex.reset();
}

keystore::Keystore* keystore()
{
    return g_keystore.get();
}

tools::ToolRegistry* toolRegistry()
{
    return g_toolRegistry.get();
}

kb::KbIndex* kbIndex()
{
    return g_kbIndex.get();
}

agent::Agent* agent()
{
    return g_agent.get();
}

std::vector<provider::IProvider*> providers()
{
    std::lock_guard<std::mutex> lk(g_mutex);
    std::vector<provider::IProvider*> out;
    out.reserve(g_providers.size());
    for (const auto& p : g_providers) out.push_back(p.get());
    return out;
}

provider::IProvider* activeProvider()
{
    std::lock_guard<std::mutex> lk(g_mutex);
    return g_active;
}

provider::IProvider* findProvider(std::string_view id)
{
    std::lock_guard<std::mutex> lk(g_mutex);
    std::string target(id);
    for (const auto& p : g_providers) if (p->id() == target) return p.get();
    return nullptr;
}

provider::IProvider* findProviderForModel(std::string_view modelId)
{
    std::lock_guard<std::mutex> lk(g_mutex);
    std::string target(modelId);
    for (const auto& p : g_providers) {
        for (const auto& m : p->models()) {
            if (m.id == target) return p.get();
        }
    }
    return nullptr;
}

bool setActiveProvider(provider::IProvider* p)
{
    std::lock_guard<std::mutex> lk(g_mutex);
    bool found = false;
    for (const auto& up : g_providers) if (up.get() == p) { found = true; break; }
    if (!found) return false;
    g_active = p;
    if (g_agent) g_agent->setProvider(p);
    return true;
}

provider::IProvider* addCustomProvider(provider::CustomProviderConfig cfg)
{
    std::lock_guard<std::mutex> lk(g_mutex);

    if (cfg.base_url.empty() || cfg.model_id.empty()) return nullptr;
    if (cfg.display_name.empty()) cfg.display_name = cfg.model_display.empty()
                                  ? cfg.model_id : cfg.model_display;
    if (cfg.id.empty()) cfg.id = slugify(cfg.display_name);

    // Auto-suffix on collisions: id, id-2, id-3, ...
    std::string base = cfg.id;
    int n = 2;
    while (isIdInUse_locked(cfg.id)) {
        cfg.id = base + "-" + std::to_string(n++);
        if (n > 999) return nullptr;  // sanity
    }

    auto provider_uniq = std::make_unique<provider::OpenAiCompatible>(std::move(cfg));
    provider::IProvider* raw = provider_uniq.get();
    g_providers.emplace_back(std::move(provider_uniq));
    persistCustomProviders_locked();
    return raw;
}

bool removeCustomProvider(std::string_view id)
{
    std::lock_guard<std::mutex> lk(g_mutex);
    std::string target(id);
    // None of the built-ins are user-removable.
    if (target == "anthropic" || target == "openai"  || target == "gemini"
        || target == "xai"    || target == "mistral") return false;

    auto it = g_providers.begin();
    // Skip all built-ins (they're consecutive at the front, in the order
    // pushed by initialize()).
    while (it != g_providers.end()) {
        const std::string& pid = (*it)->id();
        if (pid == "anthropic" || pid == "openai" || pid == "gemini"
            || pid == "xai"    || pid == "mistral") {
            ++it;
            continue;
        }
        break;
    }
    for (; it != g_providers.end(); ++it) {
        if ((*it)->id() == target) {
            bool wasActive = (g_active == it->get());
            g_providers.erase(it);
            if (wasActive) {
                g_active = g_providers.front().get();  // fall back to Anthropic
                if (g_agent) g_agent->setProvider(g_active);
            }
            persistCustomProviders_locked();
            return true;
        }
    }
    return false;
}

} // namespace ModelerAi::bootstrap
