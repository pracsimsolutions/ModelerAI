// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/commands.cpp — modelerai_* command library.
//
// Each command is invoked through the FlexScript binding nodes in
// ModelerAI.fsx (sibling of modelerAiBridgeSend). Args come in via
// param(N); result is a JSON-encoded string Variant the agent parses back
// to structured data.
//
// Convention: every function takes either typed positional params (simple
// commands) or a single JSON-encoded string param (complex commands like
// add_parameter). Returns:
//   { ok: true,  ...command-specific fields }     on success
//   { ok: false, error_code, error_message }       on failure
// The agent never sees an exception — everything is reported as a
// structured tool result.

#include "module.h"
#include "commands.h"
#include "commands/antipattern.h"
#include "commands/schema_store.h"
#include "bridge/log.h"
#include "third_party/json.h"
#include "tree/condense.h"
#include "paths.h"
#include "bootstrap.h"
#include "agent/agent.h"

#include <windows.h>     // CreateEvent / SetEvent / WaitForSingleObject

#include <atomic>
#include <cctype>
#include <chrono>
#include <cmath>
#include <cstring>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <map>
#include <mutex>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

namespace {

// One result buffer per thread — the Variant returns a const char* that
// must outlive the return. FlexScript drives every modelerai_* call
// sequentially on the main thread, so one buffer is enough.
thread_local std::string g_resultBuffer;

// ----- shared helpers -----------------------------------------------------

std::string strParam(const Variant& v)
{
    if (v.type == VariantType::Null)   return "";
    if (v.type == VariantType::String) return std::string(v);
    if (v.type == VariantType::Number) return std::to_string(static_cast<double>(v));
    return "";
}

double numParam(const Variant& v, double fallback)
{
    if (v.type == VariantType::Null)   return fallback;
    if (v.type == VariantType::Number) return static_cast<double>(v);
    return fallback;
}

bool endsWith(const std::string& s, const char* suffix)
{
    size_t sl = s.size();
    size_t pl = std::strlen(suffix);
    return sl >= pl && s.compare(sl - pl, pl, suffix) == 0;
}

// Escape a string for embedding as a FlexScript string literal. Used by
// add_parameter / set_parameter when building FlexScript snippets to feed
// executestring.
std::string fsEscape(const std::string& s)
{
    std::string out;
    out.reserve(s.size() + 8);
    for (char c : s) {
        switch (c) {
            case '\\': out += "\\\\"; break;
            case '"':  out += "\\\""; break;
            case '\n': out += "\\n";  break;
            case '\r': out += "\\r";  break;
            case '\t': out += "\\t";  break;
            default:   out += c;      break;
        }
    }
    return out;
}

Variant returnJson(const nlohmann::json& out)
{
    g_resultBuffer = out.dump();
    return Variant(g_resultBuffer.c_str());
}

Variant returnError(const char* code, const std::string& message)
{
    nlohmann::json out;
    out["ok"]            = false;
    out["error_code"]    = code;
    out["error_message"] = message;
    return returnJson(out);
}

// Same as returnError but tucks a debug payload under `debug` instead of
// inlining everything into error_message. Use when the diagnostic context
// would be enormous (full generated FlexScript, big tree dumps, etc.) —
// the message stays terse for AI tool-result display, and the raw context
// is still available if a session needs it.
Variant returnErrorWithDebug(const char* code,
                              const std::string& message,
                              nlohmann::json debug)
{
    nlohmann::json out;
    out["ok"]            = false;
    out["error_code"]    = code;
    out["error_message"] = message;
    out["debug"]         = std::move(debug);
    return returnJson(out);
}

Variant returnException(const char* commandName, const char* what)
{
    ModelerAi::bridge::consolePrint(
        std::string("[ModelerAI] EXC ") + commandName + ": " + (what ? what : "unknown") + "\n");
    return returnError("exception", what ? what : "unknown");
}

// Parse a JSON array of exactly 3 numbers into (x,y,z). Returns false on
// missing / wrong shape so the caller can treat that as "not provided"
// and fall through to a default behavior.
bool parseVec3(const nlohmann::json& j, double& x, double& y, double& z)
{
    if (!j.is_array() || j.size() != 3) return false;
    if (!j[0].is_number() || !j[1].is_number() || !j[2].is_number()) return false;
    x = j[0].get<double>();
    y = j[1].get<double>();
    z = j[2].get<double>();
    return true;
}

// Resolve a JSON arg as either an explicit "name" field in an object,
// or the arg itself as a string. The bridge passes single-string args
// through as VariantType::String, and single-object args as a
// JSON-encoded string in param(1) — this helper covers both shapes for
// the simple-tool case (delete_object, inspect_connections).
std::string resolveNameArg(const Variant& v, const char* objField)
{
    if (v.type == VariantType::Null)   return "";
    if (v.type == VariantType::Number) return std::to_string(static_cast<double>(v));
    if (v.type == VariantType::String) {
        std::string s = std::string(v);
        if (!s.empty() && s.front() == '{') {
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_object() && j.contains(objField) && j[objField].is_string()) {
                    return j[objField].get<std::string>();
                }
            } catch (...) {}
        }
        return s;
    }
    return "";
}

// ============================================================================
// DESIGN NOTE — picklists are deferred to v2 (cross-cutting concern)
// ============================================================================
//
// FlexSim attaches "picklist" presets to many code-attachable surfaces:
//   - Trigger events (e.g. Source.OnExit's "Set Output Port", "Send
//     Message", "Move Object" presets, stored under
//     <Class>>behaviour/eventInfo/<event>/variables/picklist as a
//     coupling to a VIEW:/modules/.../picklists/* node).
//   - Object properties whose values are FlexScript expressions
//     (e.g. Processor.processTime's dropdown of "Statistical Distribution
//     / By Item Type / etc."). Same shape — a picklist coupling per
//     property.
//
// V1 of every code-writing curated command (set_trigger today,
// set_property's distribution-valued path tomorrow) deliberately takes
// raw FlexScript only. Justification:
//
//   1. We don't yet have a clean read of HOW the GUI stores the
//      "user picked option X" state on the instance — is the picked
//      option name stored alongside the code, or is the code just pasted
//      verbatim and the GUI back-detects which pick matched? The answer
//      decides whether our get_* tools can return "currently picked: X"
//      vs only ever returning code. Picking a wrong design here costs
//      us a public-contract change later. So v1 picks the safe path:
//      raw code only.
//
//   2. The AI is good enough at reproducing the canonical pick code
//      patterns from KB topics. We're putting the picklist VIEW node
//      paths into the per-event KB topics; search_kb retrieves them on
//      demand. The AI can write the equivalent raw code without us having
//      to model picks as a first-class arg yet.
//
// V2 will add a `pick_option` arg to set_trigger (and the matching arg
// to set_property), wire up the per-event picklist enumeration, decide
// on the storage shape (coupling vs resolved code), and round-trip the
// pick name back via the get_* tools.
//
// Until then: AI sees raw code only, KB topics tell it which picklist
// VIEW paths exist for each event so it can crib the patterns.
// ============================================================================

// ----- ParamType (add_parameter / set_parameter) --------------------------

enum class ParamType : int {
    Continuous  = 1,
    Integer     = 2,
    Discrete    = 3,
    Binary      = 4,
    Option      = 5,
    Sequence    = 6,
    Expression  = 7,
    Passthrough = 8,
    Custom      = 9,
};

bool parseParamType(const std::string& s, ParamType& out)
{
    std::string lower;
    lower.reserve(s.size());
    for (char c : s) lower.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    if      (lower == "continuous")  { out = ParamType::Continuous;  return true; }
    else if (lower == "integer")     { out = ParamType::Integer;     return true; }
    else if (lower == "discrete")    { out = ParamType::Discrete;    return true; }
    else if (lower == "binary")      { out = ParamType::Binary;      return true; }
    else if (lower == "option")      { out = ParamType::Option;      return true; }
    else if (lower == "sequence")    { out = ParamType::Sequence;    return true; }
    else if (lower == "expression")  { out = ParamType::Expression;  return true; }
    else if (lower == "passthrough" || lower == "pass-through" || lower == "pass_through") {
        out = ParamType::Passthrough; return true;
    }
    else if (lower == "custom")      { out = ParamType::Custom;      return true; }
    return false;
}

// ============================================================================
// EXECUTION DOMAIN — run-state event machinery + hook install/uninstall.
// ============================================================================
//
// FlexSim fires three ModelTriggers as the model changes state: OnRunStart,
// OnRunStop, OnModelReset. We install a tiny block at the bottom of each
// trigger that calls applicationcommand("modelerai_notify_run_state", ...).
// That call hits ModelerAi_notifyRunState in this DLL, which records the
// new state and signals a Win32 event. Run commands (run_to_time, run,
// wait_for_stop) wait on that event when they need to know "the model
// stopped" — works regardless of WHY it stopped (target reached, user stop
// time, user click, error).
//
// Hook code is marker-wrapped so we can find and remove it cleanly later.
// Stop-time mutations done for run_to_time are similarly marked via the
// dateString field so a stale entry from a crashed run can be swept on the
// next attempt (self-healing).

// Win32 event signaled when OnRunStop / OnModelReset fires. Manual-reset
// so multiple waiters could in theory pick it up (though in practice we
// have at most one).
HANDLE g_runStopEvent = nullptr;

// Last recorded state from the trigger callback. Read by waiters AFTER
// the event signals. Guarded by g_runStateMutex.
std::mutex g_runStateMutex;
std::string g_lastRunState = "Unknown";  // "Started" / "Stopped" / "Reset"
double      g_lastSimTime  = 0.0;

// Has the install pass run in this session? Cheap memoization — we still
// verify the marker is present in the trigger body before assuming it's
// installed, but this lets us skip the disk/tree walk after the first
// successful install.
std::atomic<bool> g_hooksInstallScanDone{false};

void ensureRunStopEvent()
{
    if (!g_runStopEvent) {
        // Manual-reset, initially non-signaled, anonymous.
        g_runStopEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    }
}

// Hook block. The marker pair is what we search for to detect / remove.
// Bump kHookVersion when the body changes so the install pass replaces an
// older version cleanly.
constexpr const char* kHookBeginMarker = "// === BEGIN MODELERAI HOOK (auto-managed by PracSim ModelerAI) ===";
constexpr const char* kHookEndMarker   = "// === END MODELERAI HOOK ===";
constexpr int kHookVersion = 1;

std::string hookBlockFor(const char* stateName)
{
    std::string out;
    out += kHookBeginMarker;
    out += "\n// Hook version: ";
    out += std::to_string(kHookVersion);
    out += "\n// To remove: delete from BEGIN to END (inclusive) and rebuild the node.\n";
    out += "applicationcommand(\"modelerai_notify_run_state\", \"";
    out += stateName;
    out += "\", time());\n";
    out += kHookEndMarker;
    out += "\n";
    return out;
}

// Find [begin, end] indices spanning a hook block (begin..endMarker+len),
// or return false if no hook present. body is searched as plain text.
bool findHookSpan(const std::string& body, std::size_t& outBegin, std::size_t& outEnd)
{
    auto b = body.find(kHookBeginMarker);
    if (b == std::string::npos) return false;
    auto e = body.find(kHookEndMarker, b);
    if (e == std::string::npos) return false;
    outBegin = b;
    outEnd   = e + std::strlen(kHookEndMarker);
    // Sweep one trailing newline if present to keep the file tidy.
    if (outEnd < body.size() && body[outEnd] == '\n') ++outEnd;
    return true;
}

// Install / replace the hook in a ModelTrigger by name. Creates the trigger
// via Tools.create if missing. Always builds the code node so FlexSim
// compiles the new body. The actual tree mutation is done via executestring
// so we can use the full FlexScript API (Tools.create, enablecode, etc.).
//
// stateName: "Started" / "Stopped" / "Reset" — the value passed to the
// notify callback so it knows which event fired.
// triggerName: "OnRunStart" / "OnRunStop" / "OnModelReset".
bool installHookInTrigger(const char* triggerName, const char* stateName,
                          std::string& errOut)
{
    // First read the existing code body, if any.
    std::string readScript;
    readScript += "treenode t = Tools.get(\"ModelTrigger\", \"";
    readScript += triggerName;
    readScript += "\");\n";
    readScript += "if (!t) return \"\";\n";
    readScript += "treenode c = t.find(\"code\");\n";
    readScript += "if (!c) return \"\";\n";
    readScript += "return c.value;\n";

    std::string existing;
    try {
        Variant v = executestring(readScript.c_str(), nullptr, nullptr, Variant());
        if (v.type == VariantType::String) existing = std::string(v);
    } catch (...) { /* trigger may not exist yet — treated as empty */ }

    // Compute the new body: existing (with any old hook stripped), then
    // our current-version hook appended.
    std::string newBody = existing;
    std::size_t hb = 0, he = 0;
    if (findHookSpan(newBody, hb, he)) {
        newBody.erase(hb, he - hb);
    }
    // Trim a trailing newline run so the appended block sits cleanly.
    while (!newBody.empty() && (newBody.back() == '\n' || newBody.back() == ' ' || newBody.back() == '\t' || newBody.back() == '\r')) {
        newBody.pop_back();
    }
    if (!newBody.empty()) newBody += "\n\n";
    newBody += hookBlockFor(stateName);

    // Write it back. Tools.create only creates if missing (assert-style); we
    // always then write the code, enablecode, buildnodeflexscript.
    std::string writeScript;
    writeScript += "treenode t = Tools.get(\"ModelTrigger\", \"";
    writeScript += triggerName;
    writeScript += "\");\n";
    writeScript += "if (!t) t = Tools.create(\"ModelTrigger\", \"";
    writeScript += triggerName;
    writeScript += "\");\n";
    writeScript += "if (!t) { print(\"installHook: failed to create ";
    writeScript += triggerName;
    writeScript += "\"); return 0; }\n";
    writeScript += "treenode c = t.subnodes.assert(\"code\");\n";
    writeScript += "c.value = \"";
    writeScript += fsEscape(newBody);
    writeScript += "\";\n";
    writeScript += "enablecode(c);\n";
    writeScript += "buildnodeflexscript(c);\n";
    writeScript += "return t;\n";

    try {
        Variant v = executestring(writeScript.c_str(), nullptr, nullptr, Variant());
        return v.type == VariantType::TreeNode && static_cast<treenode>(v) != nullptr;
    } catch (const std::exception& e) {
        errOut = e.what();
        return false;
    } catch (...) {
        errOut = "unknown exception during hook install";
        return false;
    }
}

bool uninstallHookFromTrigger(const char* triggerName, std::string& errOut)
{
    std::string readScript;
    readScript += "treenode t = Tools.get(\"ModelTrigger\", \"";
    readScript += triggerName;
    readScript += "\");\n";
    readScript += "if (!t) return \"\";\n";
    readScript += "treenode c = t.find(\"code\");\n";
    readScript += "if (!c) return \"\";\n";
    readScript += "return c.value;\n";

    std::string existing;
    try {
        Variant v = executestring(readScript.c_str(), nullptr, nullptr, Variant());
        if (v.type == VariantType::String) existing = std::string(v);
    } catch (...) { return true; /* nothing to uninstall */ }

    std::size_t hb = 0, he = 0;
    if (!findHookSpan(existing, hb, he)) return true;  // already gone
    std::string newBody = existing;
    newBody.erase(hb, he - hb);

    std::string writeScript;
    writeScript += "treenode t = Tools.get(\"ModelTrigger\", \"";
    writeScript += triggerName;
    writeScript += "\");\n";
    writeScript += "if (!t) return 0;\n";
    writeScript += "treenode c = t.subnodes.assert(\"code\");\n";
    writeScript += "c.value = \"";
    writeScript += fsEscape(newBody);
    writeScript += "\";\n";
    writeScript += "enablecode(c);\n";
    writeScript += "buildnodeflexscript(c);\n";
    writeScript += "return t;\n";

    try {
        executestring(writeScript.c_str(), nullptr, nullptr, Variant());
        return true;
    } catch (const std::exception& e) {
        errOut = e.what();
        return false;
    } catch (...) {
        errOut = "unknown exception during hook uninstall";
        return false;
    }
}

// Install all three hooks. Returns count of triggers successfully installed.
int ensureAllHooksInstalled(std::string& errOut)
{
    struct H { const char* trigger; const char* state; };
    H hooks[] = {
        { "OnRunStart",   "Started" },
        { "OnRunStop",    "Stopped" },
        { "OnModelReset", "Reset"   },
    };
    int ok = 0;
    for (auto& h : hooks) {
        std::string e;
        if (installHookInTrigger(h.trigger, h.state, e)) ok++;
        else if (!e.empty()) errOut += std::string(h.trigger) + ": " + e + "; ";
    }
    if (ok == 3) g_hooksInstallScanDone.store(true);
    return ok;
}

// Temp stop time marker — written into the dateString field. Sweep on each
// run_to_time call to clean up any stale entries from a crashed prior run.
constexpr const char* kTempStopPrefix = "modelerai-temp:";

// Adds a temporary stop time at the given model time. Returns the path of
// the new SDT node on success, empty string on failure.
std::string addTempStopTime(double modelTime, std::string& errOut)
{
    long long stamp = static_cast<long long>(
        std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());

    std::string script;
    script += "treenode stopTimes = Model.find(\"Tools/ModelUnits/ModelDateTimes/stopTimes\");\n";
    script += "if (!stopTimes) { print(\"addTempStopTime: stopTimes container not found\"); return 0; }\n";
    // We need a template SDT to copy from — use the last existing entry. If
    // none, fall back to creating one via the SDT template path.
    script += "treenode template = stopTimes.last;\n";
    script += "if (!template) { print(\"addTempStopTime: no template SDT to copy from\"); return 0; }\n";
    script += "treenode newStop = createcopy(template, stopTimes, 1);\n";
    script += "setsdtvalue(newStop, \"enabled\", 1);\n";
    script += "function_s(newStop, \"setModelTime\", "; script += std::to_string(modelTime); script += ");\n";
    // Marker in dateString — user-display only, doesn't affect sim semantics.
    script += "setsdtvalue(newStop, \"dateString\", \"";
    script += kTempStopPrefix;
    script += "run_to_time:";
    script += std::to_string(stamp);
    script += "\");\n";
    script += "return newStop;\n";

    try {
        Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
        if (v.type == VariantType::TreeNode) {
            treenode n = static_cast<treenode>(v);
            if (n) {
                try { return std::string(nodetomodelpath(n, 1).c_str()); } catch (...) {}
            }
        }
    } catch (const std::exception& e) { errOut = e.what(); }
      catch (...)                     { errOut = "unknown exception"; }
    return "";
}

// Sweep any temp stop entries left over from prior runs. Called defensively
// before adding a new one.
void sweepTempStopTimes()
{
    std::string script;
    script += "treenode stopTimes = Model.find(\"Tools/ModelUnits/ModelDateTimes/stopTimes\");\n";
    script += "if (!stopTimes) return 0;\n";
    script += "int swept = 0;\n";
    script += "for (int i = stopTimes.subnodes.length; i >= 1; i--) {\n";
    script += "    treenode n = stopTimes.subnodes[i];\n";
    script += "    treenode ds = n.find(\"dateString\");\n";
    script += "    if (ds && stringstartswith(ds.value, \"";
    script += kTempStopPrefix;
    script += "\")) { n.destroy(); swept++; }\n";
    script += "}\n";
    script += "return swept;\n";
    try { executestring(script.c_str(), nullptr, nullptr, Variant()); } catch (...) {}
}

// Identify which user-managed stop time (if any) fired at the given sim
// time. Returns its index (1-based) and dateString. Empty result if none
// matched (within epsilon).
struct FiredStopInfo {
    int          index = 0;     // 0 if none
    std::string  dateString;
    double       modelTime = 0.0;
};

FiredStopInfo identifyFiredStop(double simTime)
{
    FiredStopInfo out;
    std::string script;
    script += "treenode stopTimes = Model.find(\"Tools/ModelUnits/ModelDateTimes/stopTimes\");\n";
    script += "if (!stopTimes) return 0;\n";
    script += "for (int i = 1; i <= stopTimes.subnodes.length; i++) {\n";
    script += "    treenode n = stopTimes.subnodes[i];\n";
    script += "    treenode mt = n.find(\"modelTime\");\n";
    script += "    treenode en = n.find(\"enabled\");\n";
    script += "    if (!mt || !en) continue;\n";
    script += "    if (en.value == 0) continue;\n";
    script += "    if (fabs(mt.value - "; script += std::to_string(simTime); script += ") < 1e-3) {\n";
    script += "        treenode ds = n.find(\"dateString\");\n";
    script += "        Map info = Map();\n";
    script += "        info[\"index\"] = i;\n";
    script += "        info[\"modelTime\"] = mt.value;\n";
    script += "        info[\"dateString\"] = ds ? string(ds.value) : \"\";\n";
    script += "        return info;\n";
    script += "    }\n";
    script += "}\n";
    script += "return 0;\n";
    try {
        Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
        if (v.type == VariantType::Map) {
            Map m = static_cast<Map>(v);
            out.index     = static_cast<int>(double(m["index"]));
            out.modelTime = static_cast<double>(double(m["modelTime"]));
            out.dateString = std::string(m["dateString"]);
        }
    } catch (...) {}
    return out;
}

// ----------------------------------------------------------------------------
// Shared helpers for the run-tools surface (run_to_time / run_to_end /
// run_until). See docs/superpowers/specs/2026-06-07-run-tools-design.md.
//
// All three tools share the same 100ms polled wait loop, the same
// cancellation check, and the same sim-time-progress watchdog. Extracting
// these as small inline helpers keeps each tool's body focused on its
// driver (go() vs step()) and exit conditions.
// ----------------------------------------------------------------------------

// Read time() once. Returns NaN on engine error so callers can detect it.
double currentSimTime()
{
    try {
        Variant v = executestring("return time();", nullptr, nullptr, Variant());
        if (v.type == VariantType::Number) return double(v);
    } catch (...) {}
    return std::nan("");
}

// Read eventqty() once. Returns -1 on engine error.
int currentEventQty()
{
    try {
        Variant v = executestring("return eventqty();", nullptr, nullptr, Variant());
        if (v.type == VariantType::Number) return static_cast<int>(double(v));
    } catch (...) {}
    return -1;
}

// Issue the engine's stop sequence. Swallows failures — the caller is
// already in a degraded path (timeout / cancel / stall) and there's
// nothing useful to do if stop() itself throws.
void issueEngineStop()
{
    try {
        executestring("updatestates(); stop(1); applicationcommand(\"switchRunning\", 0); return 0;",
                      nullptr, nullptr, Variant());
    } catch (...) {}
}

// Sim-time-progress watchdog state. The polled loop instantiates one of
// these at the top of the run; each 100ms tick calls check(), which
// returns true exactly once when a stall is detected.
//
// Mechanism: track (last_wall, last_sim). Every 5 wall seconds, compute
// Δsim. If Δsim < 5.0, declare stall. Otherwise update baseline and
// keep going. Threshold matches Josh's design: "we're not a physics
// simulator; nobody wants real-time simulation for months of planning."
class SimProgressWatchdog
{
public:
    explicit SimProgressWatchdog(double startSimTime)
        : last_wall_ms_(GetTickCount64()), last_sim_(startSimTime) {}

    // Returns true on the first tick where a stall is detected.
    bool check()
    {
        ULONGLONG now_ms = GetTickCount64();
        if (now_ms - last_wall_ms_ < kWindowMs) return false;
        double sim_now = currentSimTime();
        if (std::isnan(sim_now)) {
            // Engine read failed — don't flag a stall on a transient
            // read error; just reset the window so we try again.
            last_wall_ms_ = now_ms;
            return false;
        }
        double dsim = sim_now - last_sim_;
        last_wall_ms_ = now_ms;
        last_sim_     = sim_now;
        return dsim < kMinSimSecPerWindow;
    }

    double lastSimTime() const { return last_sim_; }

private:
    static constexpr ULONGLONG kWindowMs              = 5000;   // 5 wall sec
    static constexpr double    kMinSimSecPerWindow    = 5.0;    // 5 sim sec
    ULONGLONG last_wall_ms_;
    double    last_sim_;
};

// Exit reasons for the three run tools. Stringified for the JSON return.
// Kept here so callers don't drift on spelling.
const char* exitReasonName(int code)
{
    switch (code) {
        case 0:  return "target_reached";
        case 1:  return "events_drained";
        case 2:  return "condition_met";
        case 3:  return "events_drained_before_condition";
        case 4:  return "user_stopped";
        case 5:  return "wall_timeout";
        case 6:  return "stalled";
        case 7:  return "earlier_stop_fired";
        case 8:  return "already_past_target";
        case 9:  return "reset_during_run";
        default: return "unknown";
    }
}

} // namespace

// ============================================================================
// modelerai_ping — smoke test for the binding pipeline.
// ============================================================================
modelerai_export Variant ModelerAi_ping(FLEXSIMINTERFACE)
{
    try {
        ModelerAi::bridge::consolePrint("[ModelerAI] ModelerAi_ping invoked\n");
        g_resultBuffer = "pong";
        return Variant(g_resultBuffer.c_str());
    } catch (const std::exception& e) { return returnException("ping", e.what()); }
      catch (...)                     { return returnException("ping", "unknown"); }
}

// ============================================================================
// modelerai_create_object(json):
//   { class, name?, location?:[x,y,z], rotation?:[x,y,z], size?:[x,y,z], parent? }
//
// Canonical FlexScript shape (DLL generates and executestring's it so the
// modern API is used verbatim):
//   Object newObj = Object.create("Source");
//   newObj.name = "Source1";              // if name given
//   newObj.setProperty("Location", Vec3(x,y,z));
//   newObj.setProperty("Rotation", Vec3(x,y,z));
//   newObj.setProperty("Size",     Vec3(x,y,z));
//   newObj.up = Model.find("Plane1");     // if parent given
// ============================================================================
modelerai_export Variant ModelerAi_createObject(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_object expects a JSON-encoded string arg "
                "with at least { class }.");
        }
        std::string argsJson = std::string(arg);

        std::string className, objName, parentName;
        double lx, ly, lz, rx, ry, rz, sx, sy, sz;
        bool hasLoc = false, hasRot = false, hasSize = false;
        nlohmann::json propertiesMap;  // optional extra setProperty calls

        try {
            auto j = nlohmann::json::parse(argsJson);
            if (!j.is_object()) {
                return returnError("bad_args_shape",
                    "modelerai_create_object expects a JSON object.");
            }
            className  = j.value("class", std::string(""));
            objName    = j.value("name", std::string(""));
            parentName = j.value("parent", std::string(""));
            if (j.contains("location"))  hasLoc  = parseVec3(j["location"],  lx, ly, lz);
            if (j.contains("rotation"))  hasRot  = parseVec3(j["rotation"],  rx, ry, rz);
            if (j.contains("size"))      hasSize = parseVec3(j["size"],      sx, sy, sz);
            // Extra properties to apply immediately after Object.create. Map
            // of { propertyName -> value }. Lets the AI ship "make 5 red
            // Sources" as 5 single calls instead of 5+5 (create + color).
            if (j.contains("properties") && j["properties"].is_object()) {
                propertiesMap = j["properties"];
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json",
                std::string("Couldn't parse args JSON: ") + e.what());
        }

        if (className.empty()) {
            return returnError("missing_class",
                "class field is required (FlexSim class name, e.g. \"Source\", "
                "\"AGV::ControlPoint\").");
        }

        // Build the canonical FlexScript and run it.
        std::string script;
        script += "Object newObj = Object.create(\"" + fsEscape(className) + "\");\n";
        script += "if (!objectexists(newObj)) return 0;\n";
        if (!objName.empty()) {
            script += "newObj.name = \"" + fsEscape(objName) + "\";\n";
        }
        auto emitVec3 = [&](const char* prop, double a, double b, double c) {
            script += "newObj.setProperty(\""; script += prop; script += "\", Vec3(";
            script += std::to_string(a); script += ", ";
            script += std::to_string(b); script += ", ";
            script += std::to_string(c); script += "));\n";
        };
        if (hasLoc)  emitVec3("Location", lx, ly, lz);
        if (hasRot)  emitVec3("Rotation", rx, ry, rz);
        if (hasSize) emitVec3("Size",     sx, sy, sz);
        if (!parentName.empty()) {
            script += "Object parentObj = Model.find(\"" + fsEscape(parentName) + "\");\n";
            script += "if (objectexists(parentObj)) newObj.up = parentObj;\n";
        }
        // Extra-properties map. Each property gets its own try/catch in
        // the FlexScript so one bad property name doesn't abort the
        // create — failures are collected into a `failed` array and
        // surfaced in the response so the AI can retry the remainders.
        // Uses the Array(N) form for Vec3/Color values (lowest-common
        // denominator that handles strict setValue scripts; see
        // set_property's codegen for the same trick).
        std::vector<std::string> propertyOrder;
        if (propertiesMap.is_object() && !propertiesMap.empty()) {
            script += "Array __failedProps = Array();\n";
            for (auto it = propertiesMap.begin(); it != propertiesMap.end(); ++it) {
                std::string propName = it.key();
                const auto& v = it.value();
                propertyOrder.push_back(propName);
                script += "try {\n";
                if (v.is_boolean()) {
                    script += "    newObj.setProperty(\"" + fsEscape(propName) + "\", " +
                              (v.get<bool>() ? std::string("1") : std::string("0")) + ");\n";
                } else if (v.is_number()) {
                    std::ostringstream s; s << v.get<double>();
                    script += "    newObj.setProperty(\"" + fsEscape(propName) + "\", " +
                              s.str() + ");\n";
                } else if (v.is_string()) {
                    script += "    newObj.setProperty(\"" + fsEscape(propName) + "\", \"" +
                              fsEscape(v.get<std::string>()) + "\");\n";
                } else if (v.is_array() && v.size() == 3 &&
                           v[0].is_number() && v[1].is_number() && v[2].is_number()) {
                    std::ostringstream s;
                    s << "    Array __vv = Array();"
                      << " __vv.push(" << v[0].get<double>() << ");"
                      << " __vv.push(" << v[1].get<double>() << ");"
                      << " __vv.push(" << v[2].get<double>() << ");\n";
                    script += s.str();
                    script += "    newObj.setProperty(\"" + fsEscape(propName) + "\", __vv);\n";
                } else if (v.is_array() && v.size() == 4 &&
                           v[0].is_number() && v[1].is_number() &&
                           v[2].is_number() && v[3].is_number()) {
                    std::ostringstream s;
                    s << "    Array __vv = Array();"
                      << " __vv.push(" << v[0].get<double>() << ");"
                      << " __vv.push(" << v[1].get<double>() << ");"
                      << " __vv.push(" << v[2].get<double>() << ");"
                      << " __vv.push(" << v[3].get<double>() << ");\n";
                    script += s.str();
                    script += "    newObj.setProperty(\"" + fsEscape(propName) + "\", __vv);\n";
                } else {
                    script += "    __failedProps.push(\"" + fsEscape(propName) +
                              "\");\n";  // unsupported shape — skip the setProperty
                }
                script += "} catch (...) { __failedProps.push(\"" + fsEscape(propName) + "\"); }\n";
            }
            // Return a Map carrying both the new object and the failed list.
            script += "Map __result = Map();\n";
            script += "__result[\"obj\"]    = newObj;\n";
            script += "__result[\"failed\"] = __failedProps;\n";
            script += "return __result;\n";
        } else {
            script += "return newObj;\n";
        }

        Variant v;
        try {
            v = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("create_failed",
                std::string("Object.create / setProperty threw: ") + e.what());
        } catch (...) {
            return returnError("create_failed",
                "Object.create / setProperty threw a non-std exception.");
        }

        // Two return shapes possible:
        //   - TreeNode   (the simple no-properties-map path)
        //   - Map { obj: TreeNode, failed: Array<String> }  (with map)
        treenode created = nullptr;
        std::vector<std::string> failedProps;
        if (v.type == VariantType::Map) {
            Map m = static_cast<Map>(v);
            Variant objV = m["obj"];
            if (objV.type == VariantType::TreeNode) created = static_cast<treenode>(objV);
            Variant failedV = m["failed"];
            if (failedV.type == VariantType::Array) {
                Array fa = static_cast<Array>(failedV);
                for (int i = 1; i <= fa.length; ++i) failedProps.push_back(std::string(fa[i]));
            }
        } else if (v.type == VariantType::TreeNode) {
            created = static_cast<treenode>(v);
        }

        if (!created) {
            // unknown_class returns nearest matches from the schema cache so
            // the AI doesn't have to call list_classes after a typo.
            nlohmann::json suggestions = nlohmann::json::array();
            if (!className.empty()) {
                std::string needle = className;
                for (char& c : needle) c = static_cast<char>(std::tolower((unsigned char)c));
                std::string prefix = needle.substr(0, std::min<size_t>(3, needle.size()));
                // The schema store gives us O(1) per lookup; we sweep its
                // cache once with a prefix match. Hot-path classes are
                // already cached after the first list_properties call.
                std::filesystem::path schemaDir =
                    std::filesystem::path(ModelerAi::paths::moduleInstallDir()) / "KNOWLEDGE" / "schemas";
                std::error_code ec;
                if (std::filesystem::exists(schemaDir, ec) && !prefix.empty()) {
                    for (const auto& entry : std::filesystem::directory_iterator(schemaDir, ec)) {
                        if (suggestions.size() >= 8) break;
                        if (!entry.is_regular_file()) continue;
                        if (entry.path().extension() != ".json") continue;
                        std::string stem = entry.path().stem().string();
                        // Restore "::" from "__" in filename-safe form.
                        for (size_t pos = 0; (pos = stem.find("__", pos)) != std::string::npos;) {
                            stem.replace(pos, 2, "::");
                            pos += 2;
                        }
                        std::string lower = stem;
                        for (char& c : lower) c = static_cast<char>(std::tolower((unsigned char)c));
                        if (lower.rfind(prefix, 0) == 0) suggestions.push_back(stem);
                    }
                }
            }
            nlohmann::json err;
            err["ok"]            = false;
            err["error_code"]    = "unknown_class";
            err["error_message"] = "Object.create(\"" + className + "\") did not return "
                                   "a treenode — the class is likely not in the library. "
                                   "Confirm the class name and module prefix (e.g. "
                                   "\"AGV::ControlPoint\").";
            err["class"]         = className;
            if (!suggestions.empty()) err["nearest"] = std::move(suggestions);
            return returnJson(err);
        }
        if (!objectexists(created)) {
            return returnError("unknown_class",
                "Object.create(\"" + className + "\") returned null.");
        }

        std::string actualName = getname(created);

        nlohmann::json out;
        out["ok"]      = true;
        out["created"] = true;
        out["class"]   = className;
        out["name"]    = actualName;
        // Name collision detection — FlexSim silently appends ~N (or
        // sometimes keeps the auto-generated default) when the requested
        // name collides. AI MUST know its requested name isn't what's in
        // the model; otherwise subsequent calls reference the wrong object.
        if (!objName.empty() && actualName != objName) {
            out["name_collision"] = true;
            out["requested_name"] = objName;
            out["assigned_name"]  = actualName;
            out["warning"]        = "Requested name '" + objName + "' was already "
                                    "in use; FlexSim assigned '" + actualName +
                                    "'. USE THE ASSIGNED NAME (or its path) for "
                                    "any subsequent tool calls — the requested "
                                    "name still belongs to the prior object.";
        }
        try {
            const char* p = nodetomodelpath_cstr(created, 1);
            if (p) out["path"] = std::string(p);
        } catch (...) {}
        nlohmann::json pos;
        pos["x"] = xloc(created); pos["y"] = yloc(created); pos["z"] = zloc(created);
        out["location"] = std::move(pos);
        // Surface any properties from the `properties` map that didn't
        // stick. Object still exists; AI can retry the failed entries with
        // set_property (which has the full schema-validation path) to find
        // out why each one rejected.
        if (propertiesMap.is_object() && !propertiesMap.empty()) {
            int requested = (int)propertiesMap.size();
            int failed    = (int)failedProps.size();
            out["properties_requested"] = requested;
            out["properties_applied"]   = requested - failed;
            if (failed > 0) {
                nlohmann::json fa = nlohmann::json::array();
                for (const auto& n : failedProps) fa.push_back(n);
                out["properties_failed"] = std::move(fa);
                out["properties_failed_note"] = "These properties either don't exist "
                    "on the class, expected a value shape we don't generate, or "
                    "rejected at setValue time. Call modelerai_set_property on "
                    "each to get the specific failure reason.";
            }
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_object", e.what()); }
      catch (...)                     { return returnException("create_object", "unknown"); }
}

// ============================================================================
// modelerai_set_property({ object, property, value })
//
// Schema-gated. Resolves the object, gets its class via classobject(), and
// validates the (property, value) pair against KNOWLEDGE/schemas/<Class>.json
// BEFORE dispatching to FlexSim. The gate catches "property doesn't exist
// on this class" and "value shape doesn't match value_kind" early, with
// actionable errors the AI can recover from.
//
// Schema miss (no JSON for the class) is non-fatal — we fall through to
// FlexSim and let its setProperty error surface verbatim. That keeps the
// tool usable for module classes that haven't been re-extracted yet.
//
// JSON value -> FlexScript codegen:
//   number               -> obj.setProperty("X", 5.0)
//   boolean              -> obj.setProperty("X", 1) or 0
//   string + non-script  -> obj.setProperty("X", "value")
//   string + script-body -> obj.setProperty("X", "Object current = ownerobject(c);
//                                                 Object item    = param(1);
//                                                 return <value>;")
//   [a, b, c]            -> obj.setProperty("X", Vec3(a, b, c))
//   [r, g, b, a]         -> obj.setProperty("X", Color(r, g, b, a))  (ColorProperty)
//
// Script-body wrap: numeric / expression-shaped property kinds
// (number / number_with_unit / boolean / any / unknown) are stored as
// FlexScript bodies on the getValue node. Bare expressions from the AI
// get the standard header (`current` + `item`) and a `return ...;`
// wrapped around them automatically. AI can also hand-roll the full
// body — if the string already contains `return `, we pass it through
// without re-wrapping.
//
// Common transforms (Location, Rotation, Size) take Vec3; ColorProperty
// takes a 4-element [r,g,b,a] number array.
modelerai_export Variant ModelerAi_setProperty(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_property expects { object, property, value } JSON.");
        }
        std::string objectName, propertyName;
        nlohmann::json valueJson;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName   = j.value("object",   std::string(""));
            propertyName = j.value("property", std::string(""));
            if (!j.contains("value")) {
                return returnError("missing_value", "value field is required.");
            }
            valueJson = j["value"];
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || propertyName.empty()) {
            return returnError("missing_args", "object and property are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Schema lookup. Class resolution: classobject(obj) returns the
        // library class node; getname() gives the class name.
        std::string className;
        try {
            TreeNode* clsNode = classobject(obj);
            if (clsNode) className = getname(clsNode);
        } catch (...) {}

        const ModelerAi::Schema::PropertyEntry* schemaEntry = nullptr;
        const ModelerAi::Schema::ClassSchema*   classSchema = nullptr;
        if (!className.empty()) {
            classSchema = ModelerAi::Schema::get(className);
            if (classSchema) {
                schemaEntry = ModelerAi::Schema::findProperty(className, propertyName);
            }
        }

        // If we have a class schema but the property isn't in it, that's a
        // hard error — the AI is asking to set something that doesn't exist
        // on this class. Include a few nearest matches so the AI can recover.
        if (classSchema && !schemaEntry) {
            nlohmann::json suggestions = nlohmann::json::array();
            // Cheap suggestion: case-insensitive prefix match (3 chars). No
            // edit-distance — keeps the gate fast.
            std::string needle = propertyName;
            for (char& c : needle) c = static_cast<char>(std::tolower((unsigned char)c));
            if (needle.size() >= 3) {
                std::string prefix = needle.substr(0, 3);
                for (const auto& propName : classSchema->property_order) {
                    if (suggestions.size() >= 8) break;
                    std::string h = propName;
                    for (char& c : h) c = static_cast<char>(std::tolower((unsigned char)c));
                    if (h.rfind(prefix, 0) == 0) suggestions.push_back(propName);
                }
            }
            nlohmann::json err;
            err["error"]       = "property_not_on_class";
            err["object"]      = objectName;
            err["class"]       = className;
            err["property"]    = propertyName;
            err["suggestions"] = std::move(suggestions);
            err["note"]        = "Call modelerai_list_properties({class:\"" + className +
                                 "\"}) for the full list of settable properties.";
            return returnJson(err);
        }

        // Schema-guided value-kind validation. Only applies when we got an
        // entry; without a schema we fall through to the generic codegen.
        std::string expectedKind = schemaEntry ? schemaEntry->value_kind : std::string("");

        auto kindMismatch = [&](const std::string& gotShape) -> Variant {
            nlohmann::json err;
            err["error"]         = "value_kind_mismatch";
            err["object"]        = objectName;
            err["class"]         = className;
            err["property"]      = propertyName;
            err["expected_kind"] = expectedKind;
            err["got_shape"]     = gotShape;
            if (schemaEntry && expectedKind == "enum") {
                nlohmann::json opts = nlohmann::json::array();
                for (const auto& kv : schemaEntry->options) {
                    nlohmann::json o;
                    o["name"]  = kv.first;
                    o["value"] = kv.second;
                    opts.push_back(std::move(o));
                }
                err["valid_options"] = std::move(opts);
            }
            return returnJson(err);
        };

        // Detect JSON shape for the validation gate.
        auto jsonShape = [&]() -> std::string {
            if (valueJson.is_boolean()) return "boolean";
            if (valueJson.is_number())  return "number";
            if (valueJson.is_string())  return "string";
            if (valueJson.is_array()) {
                if (valueJson.size() == 3 &&
                    valueJson[0].is_number() && valueJson[1].is_number() && valueJson[2].is_number())
                    return "vec3";
                if (valueJson.size() == 4 &&
                    valueJson[0].is_number() && valueJson[1].is_number() &&
                    valueJson[2].is_number() && valueJson[3].is_number())
                    return "color_rgba";
                return "array";
            }
            return "object";
        };
        std::string gotShape = jsonShape();

        if (!expectedKind.empty()) {
            bool ok = false;
            if      (expectedKind == "number"            && gotShape == "number")  ok = true;
            else if (expectedKind == "number"            && gotShape == "string")  ok = true; // FlexScript expression body
            else if (expectedKind == "number_with_unit"  && gotShape == "number")  ok = true;
            else if (expectedKind == "number_with_unit"  && gotShape == "string")  ok = true; // FlexScript expression body
            else if (expectedKind == "boolean"           && gotShape == "boolean") ok = true;
            else if (expectedKind == "boolean"           && gotShape == "number")  ok = true; // 0/1 acceptable
            else if (expectedKind == "boolean"           && gotShape == "string")  ok = true; // FlexScript expression body
            else if (expectedKind == "enum"              && gotShape == "number")  ok = true;
            else if (expectedKind == "enum"              && gotShape == "string")  ok = true; // resolved below
            else if (expectedKind == "vec3"              && gotShape == "vec3")    ok = true;
            else if (expectedKind == "color_rgba"        && gotShape == "color_rgba") ok = true;
            else if (expectedKind == "any")                                        ok = true;
            else if (expectedKind == "unknown")                                    ok = true; // BaseProperty fallback
            if (!ok) return kindMismatch(gotShape);

            // Enum-name resolution: if AI passed a string and we have static
            // options, translate to the matching integer value.
            if (expectedKind == "enum" && gotShape == "string" && schemaEntry &&
                schemaEntry->options_kind == "static") {
                std::string named = valueJson.get<std::string>();
                long long resolved = -1;
                for (const auto& kv : schemaEntry->options) {
                    if (kv.first == named) { resolved = kv.second; break; }
                }
                if (resolved < 0) {
                    nlohmann::json err;
                    err["error"]    = "unknown_enum_option";
                    err["object"]   = objectName;
                    err["class"]    = className;
                    err["property"] = propertyName;
                    err["got"]      = named;
                    nlohmann::json opts = nlohmann::json::array();
                    for (const auto& kv : schemaEntry->options) {
                        nlohmann::json o; o["name"] = kv.first; o["value"] = kv.second;
                        opts.push_back(std::move(o));
                    }
                    err["valid_options"] = std::move(opts);
                    return returnJson(err);
                }
                valueJson = resolved;     // re-shape JSON for downstream codegen
                gotShape  = "number";
            }
        }

        // Script-body wrapping. Numeric/expression-shaped properties
        // (NumberProperty / UnitValueProperty / UniversalProperty / etc.)
        // are typically stored as FlexScript bodies — the property's
        // getValue subnode is itself a FlexScript node. When the AI passes
        // a STRING for one of these, the string lands inside that body
        // verbatim, which means the body needs a `return` and the standard
        // header (`current` and `item` declared) to even compile.
        //
        // We wrap when:
        //   - value_kind is numeric / expression-shaped (number,
        //     number_with_unit, boolean, any, unknown), AND
        //   - the AI passed a string, AND
        //   - that string doesn't already contain a `return ` (so we
        //     don't double-wrap an AI-authored full body).
        //
        // Direct number / boolean / vec3 / color values keep the existing
        // codegen path — those don't need wrapping.
        bool wrapAsScript = false;
        if (gotShape == "string" &&
            (expectedKind == "number" ||
             expectedKind == "number_with_unit" ||
             expectedKind == "boolean" ||
             expectedKind == "any" ||
             expectedKind == "unknown")) {
            wrapAsScript = true;
        }

        // Build the FlexScript script. For Vec3/Color we construct an
        // Array(3)/Array(4) and pass that — every Vec3Property /
        // ColorProperty setValue is written to cast an Array of the right
        // length to Vec3/Color (`Vec3 toLoc = param(2);` etc.). Passing
        // a Vec3 literal works for some properties (e.g. Source.Location)
        // but breaks others (e.g. AGV::Path.StartLocation), so the Array
        // form is the lowest-common-denominator that handles both.
        std::string script;
        script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";

        std::string valueKind;
        if (gotShape == "boolean") {
            valueKind = "boolean";
            std::string lit = valueJson.get<bool>() ? "1" : "0";
            script += "obj.setProperty(\"" + fsEscape(propertyName) + "\", " + lit + ");\n";
        } else if (gotShape == "number") {
            valueKind = "number";
            std::ostringstream s; s << valueJson.get<double>();
            script += "obj.setProperty(\"" + fsEscape(propertyName) + "\", " + s.str() + ");\n";
        } else if (gotShape == "string") {
            std::string raw = valueJson.get<std::string>();
            if (wrapAsScript) {
                // Only wrap if the body isn't already complete. The detection
                // is intentionally loose — "return " anywhere means the AI
                // wrote their own headers and return; pass through as-is.
                bool alreadyComplete = (raw.find("return ") != std::string::npos);
                if (!alreadyComplete) {
                    raw = "Object current = ownerobject(c);\n"
                          "Object item = param(1);\n"
                          "return " + raw + ";";
                }
                valueKind = "flexscript_body";
                // Scan the FlexScript body for hallucination / freeze-risk
                // patterns BEFORE writing it. An expression-valued property
                // (ProcessTime, MTBFMTTR, etc.) is evaluated every time
                // FlexSim needs the value — a `while (1)` there freezes the
                // run the moment the property is queried.
                std::string anName, anRem;
                if (ModelerAi::scanAntiPatterns(raw, anName, anRem)) {
                    nlohmann::json err;
                    err["ok"]            = false;
                    err["error_code"]    = "antipattern_in_property_value";
                    err["error_message"] = "FlexScript expression for property '" + propertyName +
                                           "' contains a forbidden pattern (" + anName +
                                           "). Refuse to write it.";
                    err["antipattern"]   = anName;
                    err["remediation"]   = anRem;
                    err["object"]        = objectName;
                    err["property"]      = propertyName;
                    return returnJson(err);
                }
            } else {
                valueKind = "string";
            }
            script += "obj.setProperty(\"" + fsEscape(propertyName) + "\", \""
                      + fsEscape(raw) + "\");\n";
        } else if (gotShape == "vec3") {
            valueKind = "Vec3";
            // Array(N) doesn't auto-grow on index assignment; use push().
            std::ostringstream s;
            s << "Array v = Array();"
              << " v.push(" << valueJson[0].get<double>() << ");"
              << " v.push(" << valueJson[1].get<double>() << ");"
              << " v.push(" << valueJson[2].get<double>() << ");\n";
            script += s.str();
            script += "obj.setProperty(\"" + fsEscape(propertyName) + "\", v);\n";
        } else if (gotShape == "color_rgba") {
            valueKind = "Color";
            std::ostringstream s;
            s << "Array v = Array();"
              << " v.push(" << valueJson[0].get<double>() << ");"
              << " v.push(" << valueJson[1].get<double>() << ");"
              << " v.push(" << valueJson[2].get<double>() << ");"
              << " v.push(" << valueJson[3].get<double>() << ");\n";
            script += s.str();
            script += "obj.setProperty(\"" + fsEscape(propertyName) + "\", v);\n";
        } else {
            return returnError("unsupported_value_type",
                "value must be a number, boolean, string, 3-element [x,y,z] (Vec3), "
                "or 4-element [r,g,b,a] (Color). Arbitrary arrays / nested objects "
                "not supported by set_property — use a more specific tool, or "
                "run_script if no curated tool exists.");
        }
        script += "return obj;\n";

        try {
            executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("setproperty_failed",
                "setProperty(\"" + propertyName + "\", ...) threw: " + e.what() +
                ". Property name may not be valid for class " +
                (className.empty() ? std::string("<unknown>") : className) + ".");
        } catch (...) {
            return returnError("setproperty_failed",
                "setProperty(\"" + propertyName + "\", ...) threw a non-std exception.");
        }

        nlohmann::json out;
        out["ok"]         = true;
        out["object"]     = objectName;
        out["class"]      = className;
        out["property"]   = propertyName;
        out["value_kind"] = valueKind;
        if (!classSchema) out["schema_miss"] = true;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_property", e.what()); }
      catch (...)                     { return returnException("set_property", "unknown"); }
}

// ============================================================================
// modelerai_set_label({ object, name, value })
//
// Asserts the label (creates if missing) and sets its value. Maps JSON
// shapes to FlexSim's seven native label kinds (Number, String, Pointer,
// Array, FlexScript, Bundle, TrackedVariable). FlexSim itself has no
// boolean label — JSON `true`/`false` lands as a Number with value 1/0.
//
// Value shape -> label kind:
//   42              -> Number     (booleans also land here as 1/0)
//   "hi"            -> String
//   [v1, v2, ...]   -> Array      (mixed types; each elem is number / string
//                                  / { object | node_path } pointer)
//   { object | node_path }     -> Pointer  (treenode reference)
//   { flexscript: "expr" }     -> FlexScript (wrapped with the standard
//                                  header so `current` and `item` are in
//                                  scope, then the script flag is toggled
//                                  on the label node)
//   { bundle:
//       { fields: [{name, type}], rows?: [[v1,...], ...] } }
//                                -> Bundle  (types: number / int / string /
//                                  node — maps to BUNDLE_FIELD_TYPE_*)
//   { tracked_variable:
//       { type?: "time_series",
//         start_value?: 0, flags?: 0 } }
//                                -> TrackedVariable  (default time-series @ 0)
// ============================================================================
modelerai_export Variant ModelerAi_setLabel(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_label expects { object, name, value } JSON.");
        }
        std::string objectName, labelName;
        nlohmann::json valueJson;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName = j.value("object", std::string(""));
            labelName  = j.value("name",   std::string(""));
            if (!j.contains("value")) {
                return returnError("missing_value", "value field is required.");
            }
            valueJson = j["value"];
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || labelName.empty()) {
            return returnError("missing_args", "object and name are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Convert one JSON element to a FlexScript literal expression
        // that resolves to a Variant. Used both for the top-level
        // Number/String/Pointer cases and for Array element conversion.
        // Returns empty string + sets `err` on failure.
        auto elemToLiteral = [&](const nlohmann::json& v, std::string& err) -> std::string {
            if (v.is_boolean()) return v.get<bool>() ? "1" : "0";
            if (v.is_number()) {
                std::ostringstream s; s << v.get<double>();
                return s.str();
            }
            if (v.is_string()) {
                return "\"" + fsEscape(v.get<std::string>()) + "\"";
            }
            if (v.is_object()) {
                std::string nodePath  = v.value("node_path", std::string(""));
                std::string objectRef = v.value("object",    std::string(""));
                if (!nodePath.empty()) {
                    return "node(\"" + fsEscape(nodePath) + "\", nullptr)";
                }
                if (!objectRef.empty()) {
                    return "Model.find(\"" + fsEscape(objectRef) + "\")";
                }
                err = "object element must have `node_path` or `object`";
                return "";
            }
            err = "unsupported element type";
            return "";
        };

        // Detect label kind from the JSON shape.
        enum class Kind {
            Number, String, Pointer, Array, FlexScript, Bundle, TrackedVariable
        };
        Kind kind;
        if      (valueJson.is_boolean())               kind = Kind::Number; // 1/0
        else if (valueJson.is_number())                kind = Kind::Number;
        else if (valueJson.is_string())                kind = Kind::String;
        else if (valueJson.is_array())                 kind = Kind::Array;
        else if (valueJson.is_object()) {
            if      (valueJson.contains("flexscript"))        kind = Kind::FlexScript;
            else if (valueJson.contains("bundle"))            kind = Kind::Bundle;
            else if (valueJson.contains("tracked_variable"))  kind = Kind::TrackedVariable;
            else if (valueJson.contains("node_path") ||
                     valueJson.contains("object"))            kind = Kind::Pointer;
            else {
                return returnError("bad_value_shape",
                    "Object value must contain one of: node_path/object (pointer), "
                    "flexscript, bundle, tracked_variable.");
            }
        }
        else if (valueJson.is_null()) {
            return returnError("null_value",
                "Label value cannot be null. Use modelerai_remove_label to delete "
                "a label entirely, or set a sentinel value (0 / \"\") to clear it.");
        } else {
            return returnError("unsupported_value_type",
                "Unsupported JSON type for label value.");
        }

        // Build the FlexScript script per kind. Each branch must end with
        // a script that references `treenode lbl` as the asserted label node.
        std::string script;
        script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";
        script += "treenode lbl = obj.labels.assert(\"" + fsEscape(labelName) + "\");\n";

        std::string valueKind;
        switch (kind) {
            case Kind::Number: {
                valueKind = "number";
                std::string lit;
                if (valueJson.is_boolean()) lit = valueJson.get<bool>() ? "1" : "0";
                else { std::ostringstream s; s << valueJson.get<double>(); lit = s.str(); }
                script += "lbl.value = " + lit + ";\n";
                break;
            }
            case Kind::String: {
                valueKind = "string";
                script += "lbl.value = \"" + fsEscape(valueJson.get<std::string>()) + "\";\n";
                break;
            }
            case Kind::Pointer: {
                valueKind = "pointer";
                std::string err;
                std::string lit = elemToLiteral(valueJson, err);
                if (lit.empty()) return returnError("bad_value_shape", err);
                script += "lbl.value = " + lit + ";\n";
                break;
            }
            case Kind::Array: {
                valueKind = "array";
                script += "Array a = Array();\n";
                for (const auto& el : valueJson) {
                    std::string err;
                    std::string lit = elemToLiteral(el, err);
                    if (lit.empty()) {
                        return returnError("bad_array_element",
                            "Array element error: " + err);
                    }
                    script += "a.push(" + lit + ");\n";
                }
                script += "lbl.value = a;\n";
                break;
            }
            case Kind::FlexScript: {
                valueKind = "flexscript";
                if (!valueJson["flexscript"].is_string()) {
                    return returnError("bad_value_shape",
                        "{ flexscript } must be a string body.");
                }
                std::string body = valueJson["flexscript"].get<std::string>();
                // Wrap with the standard header if AI gave us a bare
                // expression (no `return` keyword).
                if (body.find("return ") == std::string::npos) {
                    body = "Object current = ownerobject(c);\n"
                           "Object item = param(1);\n"
                           "return " + body + ";";
                }
                // Set the body text, flip the node's FlexScript flag on,
                // then build/compile the script tree so it can evaluate.
                // switch_flexscript(node, 1) is the documented "force ON"
                // form; -1 would query, 0 would force OFF.
                script += "lbl.value = \"" + fsEscape(body) + "\";\n";
                script += "switch_flexscript(lbl, 1);\n";
                script += "buildnodeflexscript(lbl);\n";
                break;
            }
            case Kind::Bundle: {
                valueKind = "bundle";
                const auto& spec = valueJson["bundle"];
                if (!spec.is_object()) {
                    return returnError("bad_value_shape",
                        "{ bundle } must be an object with fields + optional rows.");
                }
                if (!spec.contains("fields") || !spec["fields"].is_array()) {
                    return returnError("bad_value_shape",
                        "Bundle requires a `fields` array.");
                }
                // Initialize the bundle. clearbundle resets to empty state.
                script += "clearbundle(lbl);\n";
                std::vector<std::string> fieldTypes; // remember for row coercion
                for (const auto& f : spec["fields"]) {
                    if (!f.is_object() || !f.contains("name") || !f.contains("type")) {
                        return returnError("bad_value_shape",
                            "Each bundle field needs `name` and `type`.");
                    }
                    std::string fname = f.value("name", std::string(""));
                    std::string ftype = f.value("type", std::string(""));
                    std::string typeMacro;
                    if      (ftype == "number" || ftype == "double") typeMacro = "BUNDLE_FIELD_TYPE_DOUBLE";
                    else if (ftype == "int")                          typeMacro = "BUNDLE_FIELD_TYPE_INT";
                    else if (ftype == "string")                       typeMacro = "BUNDLE_FIELD_TYPE_VARCHAR";
                    else if (ftype == "node"   || ftype == "pointer") typeMacro = "BUNDLE_FIELD_TYPE_NODEREF";
                    else {
                        return returnError("bad_value_shape",
                            "Bundle field type '" + ftype + "' unsupported. "
                            "Use one of: number, int, string, node.");
                    }
                    fieldTypes.push_back(ftype);
                    script += "addbundlefield(lbl, \"" + fsEscape(fname) + "\", " + typeMacro + ");\n";
                }
                // Optional initial rows.
                if (spec.contains("rows") && spec["rows"].is_array()) {
                    int rowIdx = 1;
                    for (const auto& row : spec["rows"]) {
                        if (!row.is_array()) continue;
                        script += "addbundleentry(lbl, 1);\n";
                        for (size_t c = 0; c < row.size() && c < fieldTypes.size(); ++c) {
                            std::string err;
                            std::string lit = elemToLiteral(row[c], err);
                            if (lit.empty()) {
                                return returnError("bad_bundle_row",
                                    "Bundle row " + std::to_string(rowIdx) +
                                    " col " + std::to_string(c + 1) + ": " + err);
                            }
                            script += "setbundlevalue(lbl, " + std::to_string(rowIdx) +
                                      ", " + std::to_string(c + 1) + ", " + lit + ");\n";
                        }
                        ++rowIdx;
                    }
                }
                break;
            }
            case Kind::TrackedVariable: {
                valueKind = "tracked_variable";
                const auto& spec = valueJson["tracked_variable"];
                std::string tvTypeMacro = "STAT_TYPE_TIME_SERIES";
                double startValue = 0.0;
                int    flags      = -1; // SDK default
                if (spec.is_object()) {
                    std::string t = spec.value("type", std::string(""));
                    if      (t == "" || t == "time_series")    tvTypeMacro = "STAT_TYPE_TIME_SERIES";
                    else if (t == "level_history")             tvTypeMacro = "STAT_TYPE_LEVEL_HISTORY";
                    else if (t == "discrete_value")            tvTypeMacro = "STAT_TYPE_DISCRETE_VALUE";
                    else if (t == "discrete_change")           tvTypeMacro = "STAT_TYPE_DISCRETE_CHANGE";
                    else if (t == "categorical")               tvTypeMacro = "STAT_TYPE_CATEGORICAL";
                    else if (t == "categorical_combo")         tvTypeMacro = "STAT_TYPE_CATEGORICAL_COMBO";
                    else {
                        return returnError("bad_value_shape",
                            "TrackedVariable type '" + t + "' unknown. Use one of: "
                            "time_series, level_history, discrete_value, discrete_change, "
                            "categorical, categorical_combo.");
                    }
                    if (spec.contains("start_value") && spec["start_value"].is_number()) {
                        startValue = spec["start_value"].get<double>();
                    }
                    if (spec.contains("flags") && spec["flags"].is_number_integer()) {
                        flags = spec["flags"].get<int>();
                    }
                }
                std::ostringstream s;
                s << "TrackedVariable.init(lbl, " << tvTypeMacro
                  << ", " << startValue
                  << ", " << flags << ");\n";
                script += s.str();
                break;
            }
        }
        script += "return obj;\n";

        try {
            executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("setlabel_failed",
                "Setting label '" + labelName + "' (kind=" + valueKind +
                ") threw: " + e.what());
        } catch (...) {
            return returnError("setlabel_failed",
                "Setting label '" + labelName + "' threw a non-std exception.");
        }

        nlohmann::json out;
        out["ok"]         = true;
        out["object"]     = objectName;
        out["label"]      = labelName;
        out["value_kind"] = valueKind;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_label", e.what()); }
      catch (...)                     { return returnException("set_label", "unknown"); }
}

// ============================================================================
// modelerai_get_label({ object, name })
//
// Reads one label, detecting the kind from the underlying node:
//   DATA_BUNDLE        -> bundle  (returns row + field count, not raw rows)
//   DATA_SIMPLE        -> tracked_variable (returns current value)
//   FlexScript flag    -> flexscript (returns raw body, NOT evaluated)
//   Variant Array      -> array
//   Variant TreeNode   -> pointer ({ name, path })
//   Variant Number     -> number
//   Variant String     -> string
//
// Missing label returns label_not_found (not an error) so AI distinguishes
// "label is zero" from "label doesn't exist".
// ============================================================================
modelerai_export Variant ModelerAi_getLabel(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_label expects { object, name } JSON.");
        }
        std::string objectName, labelName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName = j.value("object", std::string(""));
            labelName  = j.value("name",   std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || labelName.empty()) {
            return returnError("missing_args", "object and name are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Probe script returns a Map with dt, isScript, plus the value
        // (or metadata for Bundle / TrackedVariable shapes). Missing label
        // returns nullvar.
        std::string script;
        script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";
        script += "string target = \"" + fsEscape(labelName) + "\";\n";
        script += "treenode lbl = nullvar;\n";
        script += "for (int i = 1; i <= obj.labels.length; i++) {\n";
        script += "    treenode l = obj.labels[i];\n";
        script += "    if (string(getname(l)) == target) { lbl = l; break; }\n";
        script += "}\n";
        script += "if (!lbl) return nullvar;\n";
        script += "Map out = Map();\n";
        script += "out[\"dt\"] = lbl.dataType;\n";
        script += "int scrFlag = switch_flexscript(lbl, -1);\n";
        script += "out[\"isScript\"] = scrFlag;\n";
        script += "if (lbl.dataType == 6) {\n";  // DATA_BUNDLE
        script += "    out[\"bundleRows\"]   = getbundlenrentries(lbl);\n";
        script += "    out[\"bundleFields\"] = getbundlenrfields(lbl);\n";
        script += "} else if (scrFlag == 1) {\n";
        script += "    out[\"scriptBody\"] = getnodestr(lbl);\n";
        script += "} else {\n";
        script += "    out[\"value\"] = lbl.value;\n";
        script += "}\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("getlabel_failed",
                "label lookup threw: " + std::string(e.what()));
        } catch (...) {
            return returnError("getlabel_failed", "label lookup threw a non-std exception.");
        }

        if (result.type == VariantType::Null) {
            nlohmann::json err;
            err["error"]  = "label_not_found";
            err["object"] = objectName;
            err["label"]  = labelName;
            return returnJson(err);
        }
        if (result.type != VariantType::Map) {
            return returnError("getlabel_failed",
                "probe returned unexpected Variant type "
                + std::to_string(static_cast<int>(result.type)));
        }

        // Local lambda: Variant -> JSON. Shared shape with get_property /
        // list_labels element conversion.
        auto variantToJson = [](const Variant& v) -> nlohmann::json {
            switch (v.type) {
                case VariantType::Number: {
                    double n = static_cast<double>(v);
                    if (n == static_cast<double>(static_cast<long long>(n))
                        && n >= -9.2e18 && n <= 9.2e18)
                        return nlohmann::json(static_cast<long long>(n));
                    return nlohmann::json(n);
                }
                case VariantType::String:
                    return nlohmann::json(std::string(v));
                case VariantType::TreeNode: {
                    TreeNode* n = static_cast<TreeNode*>(v);
                    if (objectexists(n)) {
                        nlohmann::json o;
                        try { o["name"] = std::string(getname(n)); } catch (...) {}
                        try {
                            const char* p = nodetomodelpath_cstr(n, 1);
                            if (p) o["path"] = std::string(p);
                        } catch (...) {}
                        return o;
                    }
                    return nullptr;
                }
                case VariantType::Array: {
                    Array a = static_cast<Array>(v);
                    nlohmann::json arr = nlohmann::json::array();
                    for (int i = 1; i <= a.length; ++i) {
                        Variant e = a[i];
                        if (e.type == VariantType::Number) {
                            double d = static_cast<double>(e);
                            if (d == static_cast<double>(static_cast<long long>(d))
                                && d >= -9.2e18 && d <= 9.2e18) arr.push_back(static_cast<long long>(d));
                            else arr.push_back(d);
                        } else if (e.type == VariantType::String) {
                            arr.push_back(std::string(e));
                        } else if (e.type == VariantType::TreeNode) {
                            TreeNode* n = static_cast<TreeNode*>(e);
                            if (objectexists(n)) {
                                nlohmann::json ref;
                                try { ref["name"] = std::string(getname(n)); } catch (...) {}
                                try {
                                    const char* p = nodetomodelpath_cstr(n, 1);
                                    if (p) ref["path"] = std::string(p);
                                } catch (...) {}
                                arr.push_back(std::move(ref));
                            } else {
                                arr.push_back(nullptr);
                            }
                        } else {
                            arr.push_back(nullptr);
                        }
                    }
                    return arr;
                }
                default:
                    return nullptr;
            }
        };

        Map m = static_cast<Map>(result);
        int dt       = static_cast<int>(static_cast<double>(m["dt"]));
        int isScript = static_cast<int>(static_cast<double>(m["isScript"]));

        nlohmann::json out;
        out["ok"]     = true;
        out["object"] = objectName;
        out["label"]  = labelName;

        if (dt == DATA_BUNDLE) {
            out["value_kind"] = "bundle";
            int rows   = static_cast<int>(static_cast<double>(m["bundleRows"]));
            int fields = static_cast<int>(static_cast<double>(m["bundleFields"]));
            nlohmann::json b;
            b["row_count"]   = rows;
            b["field_count"] = fields;
            b["note"]        = "Use FlexSim bundle commands (getbundlevalue, etc.) "
                               "via run_script to read individual cells.";
            out["value"] = std::move(b);
        } else if (isScript == 1) {
            out["value_kind"] = "flexscript";
            std::string body = std::string(m["scriptBody"]);
            out["value"] = body;
            out["note"]  = "Value is the raw FlexScript body; not evaluated.";
        } else {
            // Inspect the Variant under "value" for shape detection.
            Variant v = m["value"];
            // TrackedVariable: DATA_SIMPLE labels evaluate to a number on
            // read. We tag them explicitly so AI sees the difference.
            if (dt == DATA_SIMPLE) {
                out["value_kind"] = "tracked_variable";
                out["value"]      = variantToJson(v);
                out["note"]       = "Reading evaluates the TrackedVariable's current value.";
            } else {
                std::string kind;
                switch (v.type) {
                    case VariantType::Number:   kind = "number";  break;
                    case VariantType::String:   kind = "string";  break;
                    case VariantType::TreeNode: kind = "pointer"; break;
                    case VariantType::Array:    kind = "array";   break;
                    default:                    kind = "unknown"; break;
                }
                out["value_kind"] = kind;
                out["value"]      = variantToJson(v);
            }
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_label", e.what()); }
      catch (...)                     { return returnException("get_label", "unknown"); }
}

// ============================================================================
// modelerai_list_labels({ object })
//
// Compact list of every label on an object: name + value_kind + value
// preview. Lets the AI discover what's been tagged without trial calls.
// ============================================================================
modelerai_export Variant ModelerAi_listLabels(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string objectName;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            // Accept either bare object name or { object } JSON.
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) objectName = j.value("object", std::string(""));
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            } else {
                objectName = s;
            }
        }
        if (objectName.empty()) {
            return returnError("missing_args",
                "modelerai_list_labels expects { object: \"Name\" } or a bare name string.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Walk obj.labels in C++ — each label is a regular subnode under
        // the labels attribute tree. Probe per-label for dataType + script
        // flag so we can tag Bundle / TrackedVariable / FlexScript labels
        // distinctly. The probe returns Array of Arrays:
        //   [ [ name, dt, isScript, value_or_metadata ], ... ]
        // For Bundle entries the 4th slot is a 2-element [rowCount, fieldCount].
        // For FlexScript entries the 4th slot is the raw body string.
        // For other kinds it's the actual value (which may evaluate scripts).
        nlohmann::json labels = nlohmann::json::array();
        try {
            std::string script;
            script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";
            script += "Array out = Array();\n";
            script += "for (int i = 1; i <= obj.labels.length; i++) {\n";
            script += "    treenode l = obj.labels[i];\n";
            script += "    Array entry = Array();\n";
            script += "    entry.push(string(getname(l)));\n";
            script += "    entry.push(l.dataType);\n";
            script += "    int sf = switch_flexscript(l, -1);\n";
            script += "    entry.push(sf);\n";
            script += "    if (l.dataType == 6) {\n";
            script += "        Array meta = Array();\n";
            script += "        meta.push(getbundlenrentries(l));\n";
            script += "        meta.push(getbundlenrfields(l));\n";
            script += "        entry.push(meta);\n";
            script += "    } else if (sf == 1) {\n";
            script += "        entry.push(getnodestr(l));\n";
            script += "    } else {\n";
            script += "        entry.push(l.value);\n";
            script += "    }\n";
            script += "    out.push(entry);\n";
            script += "}\n";
            script += "return out;\n";
            Variant result = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (result.type == VariantType::Array) {
                Array outer = static_cast<Array>(result);
                for (int i = 1; i <= outer.length; ++i) {
                    Variant tup = outer[i];
                    if (tup.type != VariantType::Array) continue;
                    Array p = static_cast<Array>(tup);
                    if (p.length < 4) continue;
                    nlohmann::json entry;
                    entry["name"]    = std::string(p[1]);
                    int dt           = static_cast<int>(static_cast<double>(p[2]));
                    int isScript     = static_cast<int>(static_cast<double>(p[3]));
                    Variant val      = p[4];

                    std::string kind;
                    nlohmann::json valueJson;
                    if (dt == DATA_BUNDLE) {
                        kind = "bundle";
                        if (val.type == VariantType::Array) {
                            Array meta = static_cast<Array>(val);
                            nlohmann::json b;
                            if (meta.length >= 1) b["row_count"]   = static_cast<long long>(static_cast<double>(meta[1]));
                            if (meta.length >= 2) b["field_count"] = static_cast<long long>(static_cast<double>(meta[2]));
                            valueJson = std::move(b);
                        }
                    } else if (isScript == 1) {
                        kind = "flexscript";
                        if (val.type == VariantType::String) valueJson = std::string(val);
                        else valueJson = nullptr;
                    } else if (dt == DATA_SIMPLE) {
                        kind = "tracked_variable";
                        // val is the TrackedVariable's evaluated current value.
                        if (val.type == VariantType::Number) {
                            double v = static_cast<double>(val);
                            if (v == static_cast<double>(static_cast<long long>(v))
                                && v >= -9.2e18 && v <= 9.2e18) valueJson = static_cast<long long>(v);
                            else valueJson = v;
                        } else {
                            valueJson = nullptr;
                        }
                    } else {
                        switch (val.type) {
                            case VariantType::Number: {
                                double v = static_cast<double>(val);
                                if (v == static_cast<double>(static_cast<long long>(v))
                                    && v >= -9.2e18 && v <= 9.2e18) valueJson = static_cast<long long>(v);
                                else valueJson = v;
                                kind = "number";
                                break;
                            }
                            case VariantType::String:
                                valueJson = std::string(val);
                                kind = "string";
                                break;
                            case VariantType::TreeNode: {
                                TreeNode* n = static_cast<TreeNode*>(val);
                                if (objectexists(n)) {
                                    nlohmann::json ref;
                                    try { ref["name"] = std::string(getname(n)); } catch (...) {}
                                    try {
                                        const char* path = nodetomodelpath_cstr(n, 1);
                                        if (path) ref["path"] = std::string(path);
                                    } catch (...) {}
                                    valueJson = std::move(ref);
                                } else {
                                    valueJson = nullptr;
                                }
                                kind = "pointer";
                                break;
                            }
                            case VariantType::Array: {
                                Array a = static_cast<Array>(val);
                                nlohmann::json arr = nlohmann::json::array();
                                for (int ai = 1; ai <= a.length; ++ai) {
                                    Variant e = a[ai];
                                    if (e.type == VariantType::Number) {
                                        double v = static_cast<double>(e);
                                        if (v == static_cast<double>(static_cast<long long>(v))
                                            && v >= -9.2e18 && v <= 9.2e18) arr.push_back(static_cast<long long>(v));
                                        else arr.push_back(v);
                                    } else if (e.type == VariantType::String) {
                                        arr.push_back(std::string(e));
                                    } else if (e.type == VariantType::TreeNode) {
                                        TreeNode* n = static_cast<TreeNode*>(e);
                                        if (objectexists(n)) {
                                            nlohmann::json ref;
                                            try { ref["name"] = std::string(getname(n)); } catch (...) {}
                                            try {
                                                const char* path = nodetomodelpath_cstr(n, 1);
                                                if (path) ref["path"] = std::string(path);
                                            } catch (...) {}
                                            arr.push_back(std::move(ref));
                                        } else {
                                            arr.push_back(nullptr);
                                        }
                                    } else {
                                        arr.push_back(nullptr);
                                    }
                                }
                                valueJson = std::move(arr);
                                kind = "array";
                                break;
                            }
                            default:
                                valueJson = nullptr;
                                kind = "unknown";
                                break;
                        }
                    }
                    entry["value_kind"] = kind;
                    entry["value"]      = std::move(valueJson);
                    labels.push_back(std::move(entry));
                }
            }
        } catch (const std::exception& e) {
            return returnError("listlabels_failed",
                "label enumeration threw: " + std::string(e.what()));
        } catch (...) {
            return returnError("listlabels_failed",
                "label enumeration threw a non-std exception.");
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["object"]      = objectName;
        out["label_count"] = static_cast<int>(labels.size());
        out["labels"]      = std::move(labels);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_labels", e.what()); }
      catch (...)                     { return returnException("list_labels", "unknown"); }
}

// ============================================================================
// modelerai_remove_label({ object, name })
//
// Destroys a label on an object. Missing label returns label_not_found
// (not an error — idempotent feel). Same name-matching semantics as
// get_label / list_labels: case-sensitive direct match against the
// label treenode's name.
// ============================================================================
modelerai_export Variant ModelerAi_removeLabel(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_remove_label expects { object, name } JSON.");
        }
        std::string objectName, labelName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName = j.value("object", std::string(""));
            labelName  = j.value("name",   std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || labelName.empty()) {
            return returnError("missing_args", "object and name are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        std::string script;
        script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";
        script += "string target = \"" + fsEscape(labelName) + "\";\n";
        script += "int removed = 0;\n";
        script += "for (int i = 1; i <= obj.labels.length; i++) {\n";
        script += "    treenode l = obj.labels[i];\n";
        script += "    if (string(getname(l)) == target) {\n";
        script += "        destroyobject(l);\n";
        script += "        removed = 1;\n";
        script += "        break;\n";
        script += "    }\n";
        script += "}\n";
        script += "return removed;\n";

        bool removed = false;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            removed = (v.type == VariantType::Number && static_cast<double>(v) != 0.0);
        } catch (const std::exception& e) {
            return returnError("removelabel_failed",
                "label destroy threw: " + std::string(e.what()));
        } catch (...) {
            return returnError("removelabel_failed",
                "label destroy threw a non-std exception.");
        }

        if (!removed) {
            nlohmann::json err;
            err["error"]  = "label_not_found";
            err["object"] = objectName;
            err["label"]  = labelName;
            return returnJson(err);
        }

        nlohmann::json out;
        out["ok"]     = true;
        out["object"] = objectName;
        out["label"]  = labelName;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("remove_label", e.what()); }
      catch (...)                     { return returnException("remove_label", "unknown"); }
}

// ============================================================================
// modelerai_extract_class_schema({ class_name, save_to_file?: bool })
//   Walks function_s(library_class, "enumerateProperties") and emits a
//   structured schema for one class.
//
//   Internally enumerateProperties() returns an Array of treenodes — one
//   per property the class supports (inherited included). Each property
//   node carries its own metadata in tree subnodes:
//     - superclasses subnode -> couplings to parent property-type classes
//       (BaseProperty / NumberProperty / ComboProperty / Vec3Property /
//       ColorProperty / etc.). Walked to determine the property's leaf
//       type.
//     - variables/{localizedName, category} -> display fields
//     - variables/options -> for ComboProperty:
//         * STATIC: options is a regular subnode tree with inline option
//           children (name -> integer value)
//         * DYNAMIC: options is a coupling pointing at a MODEL: path; the
//           options are user-extensible at runtime
//
//   Output: { class, extracted_at, property_count, properties: [...] }.
//   With save_to_file=true, also writes to
//   <module install dir>/KNOWLEDGE/schemas/<sanitized class name>.json
//   so it can be referenced via search_kb / read_topic after re-indexing.
//
//   Class names with module prefix (AGV::Path) map to nested tree paths
//   (project/library/AGV/Path) and to underscored file names
//   (AGV__Path.json).
// ============================================================================

namespace {

// Recognized property-type class names. The walker climbs the superclass
// chain of a property node and stops at the first match. Order is just
// for readability — lookup is a set membership test.
const std::unordered_set<std::string>& kPropertyTypes()
{
    static const std::unordered_set<std::string> s = {
        "BaseProperty", "UniversalProperty", "NumberProperty",
        "UnitProperty", "UnitUniversalProperty", "UnitValueProperty",
        "ComboProperty", "CheckboxProperty",
        "TableProperty", "ArrayProperty",
        "Vec3Property", "ScalarSpatialProperty", "SubnodeArrayProperty",
        "ColorProperty", "ConnectionsProperty",
    };
    return s;
}

// Map a property-type class name to a value_kind hint that the AI uses
// to pick the right JSON value type for set_property. "any" means the
// AI should infer from context (UniversalProperty accepts heterogeneous
// values).
std::string propertyTypeToValueKind(const std::string& t)
{
    if (t == "NumberProperty")           return "number";
    if (t == "CheckboxProperty")         return "boolean";
    if (t == "UniversalProperty")        return "any";
    if (t == "UnitProperty"  ||
        t == "UnitUniversalProperty" ||
        t == "UnitValueProperty")        return "number_with_unit";
    if (t == "ComboProperty")            return "enum";
    if (t == "Vec3Property" ||
        t == "ScalarSpatialProperty")    return "vec3";
    if (t == "ColorProperty")            return "color_rgba";
    if (t == "ArrayProperty" ||
        t == "SubnodeArrayProperty")     return "array";
    if (t == "TableProperty")            return "table";
    if (t == "ConnectionsProperty")      return "connections";
    return "unknown";
}

// Resolve a coupling node's target. Returns nullptr if the node isn't a
// coupling or the target is gone.
TreeNode* couplingTarget(TreeNode* couplingNode)
{
    if (!couplingNode) return nullptr;
    if (couplingNode->dataType != DATA_POINTERCOUPLING) return nullptr;
    try {
        CouplingDataType* c = couplingNode->object<CouplingDataType>();
        if (c) return c->partner().get();
    } catch (...) {}
    return nullptr;
}

// Climb the superclasses chain of a property node, looking for the first
// class name in kPropertyTypes(). Returns "Unknown" if nothing matches.
// Hard depth cap to avoid runaway loops on malformed trees.
std::string findLeafPropertyType(TreeNode* propNode, int depth = 0)
{
    if (!propNode || depth > 8) return "Unknown";

    TreeNode* superNode = nullptr;
    try { superNode = propNode->find(">superclasses"); } catch (...) {}
    if (!superNode) return "Unknown";

    int n = 0;
    try { n = (int)superNode->subnodes.length; } catch (...) {}
    for (int i = 1; i <= n; ++i) {
        TreeNode* coupling = nullptr;
        try { coupling = superNode->subnodes[i]; } catch (...) {}
        if (!coupling) continue;
        TreeNode* parentClass = couplingTarget(coupling);
        if (!parentClass) continue;
        std::string parentName;
        try { parentName = getname(parentClass); } catch (...) {}
        if (kPropertyTypes().count(parentName)) return parentName;
        // Recurse — the matched type might be a grandparent
        std::string deeper = findLeafPropertyType(parentClass, depth + 1);
        if (deeper != "Unknown") return deeper;
    }
    return "Unknown";
}

// Read a string-valued child of a property's variables subtree. Returns
// empty if the child is missing or not a string-typed node.
std::string readVarStr(TreeNode* propNode, const char* varName)
{
    try {
        TreeNode* v = propNode->find((">variables/" + std::string(varName)).c_str());
        if (!v) return "";
        if (v->dataType == DATA_BYTEBLOCK) {
            return getnodestr(v);
        }
    } catch (...) {}
    return "";
}

// Strip the trailing "::ModuleClass" → "ModuleClass" segment for filename
// safety, OR replace "::" with "__". Pick the latter.
std::string classNameToFileSafe(std::string s)
{
    size_t pos = 0;
    while ((pos = s.find("::", pos)) != std::string::npos) {
        s.replace(pos, 2, "__");
        pos += 2;
    }
    return s;
}

// Convert class name → recursive-search tree path under project/library.
// project/library is grouped by category folders (FixedResources, TaskExecuters,
// AGV, AStar, ...) that the AI doesn't track, so we anchor at project/library
// and let `?` do a recursive name-search for the bare class name.
//   "Source"     -> "project/library/?Source"
//   "AGV::Path"  -> "project/library/AGV/?Path"
// The namespace prefix (everything before the last "::") is treated as a
// literal subtree anchor so module classes don't collide with same-named
// classes in other modules.
std::string classNameToTreePath(const std::string& className)
{
    std::string path = "project/library/";
    std::string remaining = className;
    size_t pos = 0;
    while ((pos = remaining.find("::")) != std::string::npos) {
        path += remaining.substr(0, pos) + "/";
        remaining = remaining.substr(pos + 2);
    }
    path += "?" + remaining;
    return path;
}

std::string currentYmd()
{
    time_t now = time(nullptr);
    tm tm_buf;
    localtime_s(&tm_buf, &now);
    char buf[16];
    snprintf(buf, sizeof(buf), "%04d-%02d-%02d",
             tm_buf.tm_year + 1900, tm_buf.tm_mon + 1, tm_buf.tm_mday);
    return buf;
}

} // anonymous

modelerai_export Variant ModelerAi_extractClassSchema(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_extract_class_schema expects "
                "{ class_name, save_to_file? } OR { all: true } JSON.");
        }
        std::string className;
        bool saveToFile = false;
        bool allFlag    = false;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            className  = j.value("class_name", std::string(""));
            saveToFile = j.value("save_to_file", false);
            allFlag    = j.value("all", false);
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        // `all: true` — sweep every leaf class under project/library and
        // re-extract its schema. Replaces the FlexScript loop the README
        // used to tell modelers to write by hand. We dispatch the loop
        // INSIDE FlexScript, calling ourselves per class via
        // applicationcommand. save_to_file defaults true under all
        // (the only use case is "rebuild the on-disk corpus").
        if (allFlag) {
            // save_to_file is implicitly true for the all path — bulk
            // rebuild has no other purpose.
            std::string walkScript;
            walkScript += "treenode lib = maintree().find(\"project/library\");\n";
            walkScript += "if (!lib) return -1;\n";
            walkScript += "int classCount = 0;\n";
            walkScript += "forobjecttreeunder(lib) {\n";
            walkScript += "    if (content(a) == 0) {\n";
            walkScript += "        string nm = a.name;\n";
            walkScript += "        applicationcommand(\"modelerai_extract_class_schema\", "
                          "\"{\\\"class_name\\\":\\\"\"+nm+\"\\\",\\\"save_to_file\\\":true}\");\n";
            walkScript += "        classCount = classCount + 1;\n";
            walkScript += "    }\n";
            walkScript += "}\n";
            walkScript += "return classCount;\n";

            int classCount = 0;
            try {
                Variant v = executestring(walkScript.c_str(), nullptr, nullptr, Variant());
                if (v.type == VariantType::Number) classCount = (int)double(v);
            } catch (const std::exception& e) {
                return returnError("all_walk_failed", std::string("forobjecttreeunder threw: ") + e.what());
            } catch (...) {
                return returnError("all_walk_failed", "forobjecttreeunder threw a non-std exception.");
            }
            if (classCount < 0) {
                return returnError("library_not_found",
                    "Could not resolve maintree().find(\"project/library\"). "
                    "Is a model loaded?");
            }
            // Drop the schema cache so subsequent lookups read the fresh
            // files we just wrote rather than the stale in-memory entries.
            ModelerAi::Schema::clearCache();

            nlohmann::json out;
            out["ok"]                = true;
            out["all"]               = true;
            out["classes_processed"] = classCount;
            out["save_to_file"]      = true;
            out["note"]              = "Walked project/library leaf classes; each "
                                       "wrote KNOWLEDGE/schemas/<Class>.json. "
                                       "Schema cache cleared so next lookup reads "
                                       "the fresh files. Commit the diff in git "
                                       "after a FlexSim version bump.";
            return returnJson(out);
        }

        if (className.empty()) {
            return returnError("missing_class_name", "class_name is required (or pass all: true).");
        }

        // Resolve the class node under /project/library. AGV::Path → AGV/Path.
        std::string treePath = classNameToTreePath(className);
        TreeNode* classNode = nullptr;
        try {
            // maintree() returns TreeNodeListHead* which doesn't expose find()
            // directly the way treenode does; use executestring as the
            // simplest portable path-resolution.
            std::string resolveScript;
            resolveScript += "return maintree().find(\"" + fsEscape(treePath) + "\");\n";
            Variant v = executestring(resolveScript.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::TreeNode) {
                classNode = static_cast<TreeNode*>(v);
            }
        } catch (...) {}
        if (!objectexists(classNode)) {
            return returnError("class_not_found",
                "Could not resolve class node at maintree path '" + treePath +
                "'. Confirm the class name (use module prefix like 'AGV::Path' for module classes).");
        }

        // Call enumerateProperties via function_s. Returns Array of treenodes.
        Variant propsResult;
        try {
            propsResult = function_s(classNode, "enumerateProperties");
        } catch (const std::exception& e) {
            return returnError("enumerate_failed",
                std::string("function_s(\"enumerateProperties\") threw: ") + e.what());
        } catch (...) {
            return returnError("enumerate_failed",
                "function_s(\"enumerateProperties\") threw a non-std exception.");
        }
        if (propsResult.type != VariantType::Array) {
            return returnError("enumerate_bad_return",
                "enumerateProperties did not return an Array (returned VariantType " +
                std::to_string(static_cast<int>(propsResult.type)) + ").");
        }
        Array propArray = static_cast<Array>(propsResult);

        // Build the schema. Use an ordered map keyed by base name so
        // composites can fold their .X/.Y/.Z children into the parent's
        // components[] field.
        std::map<std::string, nlohmann::json> byName;
        std::map<std::string, std::vector<std::string>> components;

        int rawCount = 0;
        for (int i = 1; i <= propArray.length; ++i) {
            Variant pv = propArray[i];
            if (pv.type != VariantType::TreeNode) continue;
            TreeNode* propNode = static_cast<TreeNode*>(pv);
            if (!objectexists(propNode)) continue;
            ++rawCount;

            std::string fullName;
            try { fullName = getname(propNode); } catch (...) {}
            if (fullName.empty()) continue;

            // Composite component? (Location.X / Color.Red / etc.)
            size_t dot = fullName.find('.');
            if (dot != std::string::npos) {
                std::string baseName = fullName.substr(0, dot);
                std::string compName = fullName.substr(dot + 1);
                components[baseName].push_back(compName);
                continue;
            }

            // Path → origin classification.
            std::string path;
            try {
                const char* p = nodetomodelpath_cstr(propNode, 1);
                if (p) path = p;
            } catch (...) {}
            std::string source = "class";
            if (path.rfind("MAIN:/project/exec/globals/", 0) == 0) source = "global";

            // Type via superclass walk.
            std::string propType = findLeafPropertyType(propNode);
            std::string valueKind = propertyTypeToValueKind(propType);

            nlohmann::json entry;
            entry["name"]       = fullName;
            entry["type"]       = propType;
            entry["value_kind"] = valueKind;
            entry["source"]     = source;
            if (!path.empty()) entry["path"] = path;

            std::string localized = readVarStr(propNode, "localizedName");
            if (!localized.empty()) entry["localized_name"] = localized;
            std::string category = readVarStr(propNode, "category");
            if (!category.empty()) entry["category"] = category;

            // ComboProperty: classify options as static (inline subnodes)
            // vs dynamic (coupling to a MODEL: path).
            if (propType == "ComboProperty") {
                TreeNode* optionsNode = nullptr;
                try { optionsNode = propNode->find(">variables/options"); } catch (...) {}
                if (optionsNode) {
                    if (optionsNode->dataType == DATA_POINTERCOUPLING) {
                        entry["options_kind"] = "dynamic";
                        TreeNode* target = couplingTarget(optionsNode);
                        if (target) {
                            try {
                                const char* p = nodetomodelpath_cstr(target, 1);
                                if (p) entry["options_path"] = std::string(p);
                            } catch (...) {}
                        }
                        entry["options_note"] = "Options are dynamic and "
                            "user-extensible at runtime. Read from "
                            "options_path before validating a set_property "
                            "value against the current list.";
                    } else {
                        entry["options_kind"] = "static";
                        nlohmann::json opts = nlohmann::json::array();
                        int nOpts = 0;
                        try { nOpts = (int)optionsNode->subnodes.length; } catch (...) {}
                        for (int k = 1; k <= nOpts; ++k) {
                            TreeNode* opt = nullptr;
                            try { opt = optionsNode->subnodes[k]; } catch (...) {}
                            if (!opt) continue;
                            std::string optName;
                            try { optName = getname(opt); } catch (...) {}
                            nlohmann::json o;
                            o["name"] = optName;
                            try {
                                double val = static_cast<double>(opt->value);
                                if (val == static_cast<double>(static_cast<long long>(val))) {
                                    o["value"] = static_cast<long long>(val);
                                } else {
                                    o["value"] = val;
                                }
                            } catch (...) {}
                            opts.push_back(std::move(o));
                        }
                        entry["options"] = std::move(opts);
                    }
                }
            }

            byName[fullName] = std::move(entry);
        }

        // Fold composite components.
        for (auto& [base, comps] : components) {
            auto it = byName.find(base);
            if (it == byName.end()) continue;
            nlohmann::json arr = nlohmann::json::array();
            for (const auto& c : comps) arr.push_back(c);
            it->second["components"] = std::move(arr);
        }

        // Emit properties in insertion order (alphabetical from std::map).
        nlohmann::json props = nlohmann::json::array();
        for (auto& [name, entry] : byName) {
            props.push_back(entry);
        }

        nlohmann::json schema;
        schema["class"]          = className;
        schema["extracted_at"]   = currentYmd();
        schema["property_count"] = static_cast<int>(props.size());
        schema["raw_path_count"] = rawCount;
        schema["properties"]     = std::move(props);

        // Optional file write under KNOWLEDGE/schemas/<class>.json
        std::string filePath;
        if (saveToFile) {
            std::string fileName = classNameToFileSafe(className) + ".json";
            std::string dir = ModelerAi::paths::moduleInstallDir() + "KNOWLEDGE\\schemas\\";
            std::error_code ec;
            std::filesystem::create_directories(dir, ec);
            if (ec) {
                return returnError("file_dir_failed",
                    "create_directories('" + dir + "') failed: " + ec.message());
            }
            filePath = dir + fileName;
            std::ofstream f(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
            if (!f.is_open()) {
                return returnError("file_write_failed",
                    "Could not open '" + filePath + "' for writing.");
            }
            std::string content = schema.dump(2);
            f.write(content.data(), static_cast<std::streamsize>(content.size()));
            f.close();
            if (f.fail()) {
                return returnError("file_write_failed",
                    "Write to '" + filePath + "' failed.");
            }
        }

        nlohmann::json out;
        out["ok"]             = true;
        out["class_name"]     = className;
        out["property_count"] = schema["property_count"];
        out["raw_path_count"] = schema["raw_path_count"];
        if (!filePath.empty()) out["file_path"] = filePath;
        out["schema"]         = std::move(schema);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("extract_class_schema", e.what()); }
      catch (...)                     { return returnException("extract_class_schema", "unknown"); }
}

// ============================================================================
// modelerai_create_port_connection — REMOVED 2026-06-02.
// Superseded by the semantic split: connect_fixed_resources,
// connect_task_executer_to_navigator, connect_fixed_resource_to_navigator,
// connect_dispatcher_to_task_executer, connect_centerport.
// Each new tool restricts to a class pair so the AI can't pick wrong.
// ============================================================================

// ============================================================================
// modelerai_delete_object(name | {name}):
//   Resolves via Model.find. Reports what got auto-disconnected (flow,
//   centerport) so the AI knows what wires were broken. AGV Navigator
//   protected — refuses if there are still AGV objects in the model.
//   Other navigators (DefaultNavigator instance, AStar::AStarNavigator,
//   GIS::GISNavigator, DefaultNetworkNavigator instance) delete fine.
// ============================================================================
modelerai_export Variant ModelerAi_deleteObject(FLEXSIMINTERFACE)
{
    try {
        std::string objectName = resolveNameArg(param(1), "name");
        if (objectName.empty()) {
            return returnError("missing_name",
                "modelerai_delete_object requires a name (string or {\"name\": ...}).");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        std::string className;
        TreeNode* clsNode = classobject(obj);
        if (clsNode) className = getname(clsNode);

        // AGV Navigator protection — only this specific navigator class
        // requires dependent cleanup first. Other navigators delete fine.
        // Walk the WHOLE model tree (not just top-level subnodes) — AGV
        // ControlPoints commonly live inside Planes or other containers
        // and the shallow walk would have missed them.
        if (isclasstype(obj, "AGV::AGVNetwork")) {
            std::vector<std::string> dependents;
            try {
                std::string script;
                script += "Array out;\n";
                script += "Object skipMe = Model.find(\"" + fsEscape(objectName) + "\");\n";
                script += "forobjecttreeunder(model()) {\n";
                script += "    treenode candidate = ownerobject(a.value);\n";
                script += "    if (!objectexists(candidate)) continue;\n";
                script += "    if (candidate == skipMe) continue;\n";
                script += "    treenode cls = classobject(candidate);\n";
                script += "    if (!objectexists(cls)) continue;\n";
                script += "    string nm = getname(cls);\n";
                script += "    if (nm.substr(0, 5) == \"AGV::\")\n";
                script += "        out.push(candidate);\n";
                script += "}\n";
                script += "return out;\n";
                Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
                if (v.type == VariantType::Array) {
                    Array arr = static_cast<Array>(v);
                    std::set<std::string> seen;  // dedupe (forobjecttreeunder can revisit)
                    for (int i = 1; i <= arr.length && dependents.size() < 10; ++i) {
                        if (arr[i].type != VariantType::TreeNode) continue;
                        TreeNode* dep = static_cast<TreeNode*>(arr[i]);
                        if (!objectexists(dep)) continue;
                        std::string nm = getname(dep);
                        if (!seen.insert(nm).second) continue;
                        TreeNode* depCls = classobject(dep);
                        std::string depClassName = depCls ? std::string(getname(depCls)) : std::string();
                        dependents.push_back(nm + " (" + depClassName + ")");
                    }
                }
            } catch (...) {}
            if (!dependents.empty()) {
                std::string msg = "Cannot delete '" + objectName + "' (AGV::AGVNetwork): "
                                  "AGV objects still depend on it (recursive scan). Delete "
                                  "these first: ";
                for (size_t i = 0; i < dependents.size(); ++i) {
                    if (i) msg += ", ";
                    msg += dependents[i];
                }
                msg += (dependents.size() >= 10 ? " (and possibly more)." : ".");
                return returnError("agv_navigator_has_dependents", msg);
            }
        }

        // Cascade report — snapshot the port arrays BEFORE destroying so we
        // can tell the AI what wires got auto-cleared.
        nlohmann::json brokenFlow      = nlohmann::json::array();
        nlohmann::json brokenCenter    = nlohmann::json::array();
        try {
            int nOut = (int)nrop(obj);
            for (int i = 1; i <= nOut; ++i) {
                TreeNode* dst = outobject(obj, i);
                if (dst) brokenFlow.push_back(objectName + " -> " + getname(dst));
            }
            int nIn = (int)nrip(obj);
            for (int i = 1; i <= nIn; ++i) {
                TreeNode* src = inobject(obj, i);
                if (src) brokenFlow.push_back(std::string(getname(src)) + " -> " + objectName);
            }
            int nCp = (int)nrcp(obj);
            for (int i = 1; i <= nCp; ++i) {
                TreeNode* peer = centerobject(obj, i);
                if (peer) brokenCenter.push_back(objectName + " <-> " + getname(peer));
            }
        } catch (...) {}

        int childCount = (int)content(obj);

        try {
            destroyobject(obj);
        } catch (const std::exception& e) {
            return returnError("destroy_failed",
                std::string("destroyobject threw: ") + e.what());
        } catch (...) {
            return returnError("destroy_failed",
                "destroyobject threw a non-std exception.");
        }

        nlohmann::json out;
        out["ok"]                 = true;
        out["deleted"]            = true;
        out["name"]               = objectName;
        out["class"]              = className;
        out["children_destroyed"] = childCount;
        if (!brokenFlow.empty())   out["disconnected_flow"]       = std::move(brokenFlow);
        if (!brokenCenter.empty()) out["disconnected_centerport"] = std::move(brokenCenter);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("delete_object", e.what()); }
      catch (...)                     { return returnException("delete_object", "unknown"); }
}

// ============================================================================
// modelerai_clone_object(json):
//   { source, name?, location?:[x,y,z], offset?:[dx,dy,dz], parent?,
//     copy_connections?:bool, count?:int }
//
// Canonical FlexScript:
//   Object src = Model.find("Source1");
//   Object newObj = src.copy(destination);
//   newObj.setProperty("Location", Vec3(...));
//   applicationcommand("recreateObjectConnections", newObj, src);  // opt-in
// ============================================================================
modelerai_export Variant ModelerAi_cloneObject(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_clone_object expects a JSON-encoded string arg "
                "with at least { source }.");
        }

        std::string sourceName, newName, parentName;
        double lx, ly, lz, dx, dy, dz;
        bool hasLoc = false, hasOffset = false;
        bool copyConnections = false;
        int count = 1;

        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            sourceName = j.value("source", std::string(""));
            newName    = j.value("name",   std::string(""));
            parentName = j.value("parent", std::string(""));
            copyConnections = j.value("copy_connections", false);
            count = j.value("count", 1);
            if (j.contains("location")) hasLoc    = parseVec3(j["location"], lx, ly, lz);
            if (j.contains("offset"))   hasOffset = parseVec3(j["offset"],   dx, dy, dz);
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (sourceName.empty()) {
            return returnError("missing_source", "source field is required.");
        }
        if (count < 1) count = 1;
        if (count > 100) {
            return returnError("count_too_high",
                "count capped at 100 per call to avoid runaway clones. "
                "Issue multiple calls if you need more.");
        }
        if (count > 1 && !newName.empty()) {
            return returnError("name_with_count",
                "name is only applicable when count == 1 (a single clone). "
                "With count > 1, FlexSim auto-numbers each clone — the `name` "
                "arg would be silently ignored, which the AI must not depend "
                "on. Either drop the name and read the assigned names from "
                "the `created` array in the response, or issue separate "
                "single-clone calls with explicit names.");
        }

        TreeNode* src = model()->find(sourceName.c_str());
        if (!objectexists(src)) {
            return returnError("source_not_found",
                "source '" + sourceName + "' did not resolve via Model.find.");
        }
        if (!parentName.empty()) {
            TreeNode* p = model()->find(parentName.c_str());
            if (!objectexists(p)) {
                return returnError("parent_not_found",
                    "parent '" + parentName + "' did not resolve via Model.find.");
            }
        }

        // Source location — used for offset stacking
        double srcX = xloc(src), srcY = yloc(src), srcZ = zloc(src);

        nlohmann::json created = nlohmann::json::array();
        for (int n = 1; n <= count; ++n) {
            std::string script;
            script += "Object src = Model.find(\"" + fsEscape(sourceName) + "\");\n";
            if (!parentName.empty()) {
                script += "Object parentObj = Model.find(\"" + fsEscape(parentName) + "\");\n";
                script += "Object newObj = src.copy(parentObj);\n";
            } else {
                script += "Object newObj = src.copy();\n";
            }
            if (count == 1 && !newName.empty()) {
                script += "newObj.name = \"" + fsEscape(newName) + "\";\n";
            }
            if (hasLoc) {
                script += "newObj.setProperty(\"Location\", Vec3(";
                script += std::to_string(lx); script += ", ";
                script += std::to_string(ly); script += ", ";
                script += std::to_string(lz); script += "));\n";
            } else if (hasOffset) {
                double tx = srcX + dx * n;
                double ty = srcY + dy * n;
                double tz = srcZ + dz * n;
                script += "newObj.setProperty(\"Location\", Vec3(";
                script += std::to_string(tx); script += ", ";
                script += std::to_string(ty); script += ", ";
                script += std::to_string(tz); script += "));\n";
            }
            if (copyConnections) {
                script += "applicationcommand(\"recreateObjectConnections\", newObj, src);\n";
            }
            script += "return newObj;\n";

            Variant v;
            try {
                v = executestring(script.c_str(), nullptr, nullptr, Variant());
            } catch (const std::exception& e) {
                return returnError("clone_failed",
                    std::string("source.copy() threw on iteration ") +
                    std::to_string(n) + ": " + e.what());
            } catch (...) {
                return returnError("clone_failed",
                    "source.copy() threw a non-std exception on iteration " +
                    std::to_string(n));
            }
            if (v.type != VariantType::TreeNode) {
                return returnError("clone_failed",
                    "source.copy() did not return a treenode on iteration " +
                    std::to_string(n));
            }
            treenode newObj = static_cast<treenode>(v);
            if (!objectexists(newObj)) {
                return returnError("clone_failed",
                    "source.copy() returned null on iteration " + std::to_string(n));
            }

            nlohmann::json e;
            e["name"] = std::string(getname(newObj));
            try {
                const char* p = nodetomodelpath_cstr(newObj, 1);
                if (p) e["path"] = std::string(p);
            } catch (...) {}
            nlohmann::json pos;
            pos["x"] = xloc(newObj); pos["y"] = yloc(newObj); pos["z"] = zloc(newObj);
            e["location"] = std::move(pos);
            created.push_back(std::move(e));
        }

        nlohmann::json out;
        out["ok"]              = true;
        out["count"]           = count;
        out["copy_connections"]= copyConnections;
        out["created"]         = std::move(created);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("clone_object", e.what()); }
      catch (...)                     { return returnException("clone_object", "unknown"); }
}

// ----------------------------------------------------------------------------
// Connection helpers — shared by the 5 connect_* tools.
// ----------------------------------------------------------------------------
namespace {

// Walk the to-node's inputs looking for the from-node. Returns the port
// index (1-based) if found, 0 if not. Used as the "already connected"
// gate for flow / dispatcher / FR-to-nav-anchor tools.
int findInPortFor(TreeNode* toNode, TreeNode* fromNode)
{
    if (!objectexists(toNode) || !objectexists(fromNode)) return 0;
    int n = (int)nrip(toNode);
    for (int i = 1; i <= n; ++i) {
        if (inobject(toNode, i) == fromNode) return i;
    }
    return 0;
}

int findOutPortFor(TreeNode* fromNode, TreeNode* toNode)
{
    if (!objectexists(toNode) || !objectexists(fromNode)) return 0;
    int n = (int)nrop(fromNode);
    for (int i = 1; i <= n; ++i) {
        if (outobject(fromNode, i) == toNode) return i;
    }
    return 0;
}

int findCenterPortFor(TreeNode* a, TreeNode* b)
{
    if (!objectexists(a) || !objectexists(b)) return 0;
    int n = (int)nrcp(a);
    for (int i = 1; i <= n; ++i) {
        if (centerobject(a, i) == b) return i;
    }
    return 0;
}

// ----------------------------------------------------------------------------
// FixedResource → navigator-system membership tree walk.
//
// FRs do NOT expose Navigator as a getProperty key (TEs do; FRs don't). The
// FR-side state of "which navigator systems am I in" lives in two tree
// locations:
//   (a) FR's >stored subtree — couplings to anchors (AGV::ControlPoint,
//       GIS::Point, AStar storage indices, etc.). For A*, the coupling
//       target's OWNER's parent is the AStarNavigator; for AGV/GIS the
//       coupling target's owner IS the anchor class.
//   (b) FR's >variables/networknodes — Network Navigator memberships,
//       stored as their own array independent of >stored.
//
// This helper runs a FlexScript walker via executestring (cleaner than
// trying to traverse FlexSim's coupling SDK directly from C++) and
// returns each anchor as a treenode. Anchor class then determines which
// system the FR is in.
// ----------------------------------------------------------------------------
struct NavMembership {
    std::string system;        // "AGV" | "Network" | "GIS" | "AStar" | "Other"
    std::string anchor_name;   // visible name
    std::string anchor_class;  // module-qualified class name
};

// Walker status — propagated to caller so failure modes can be surfaced
// to the AI as inspection_failed rather than silently treated as "no
// memberships" (which would let stale FRs slip past the duplicate-system
// guard).
struct NavWalkResult {
    std::vector<NavMembership> memberships;
    bool        ok = false;
    std::string error_detail;  // populated on !ok
};

NavWalkResult fixedResourceNavigatorMemberships(const std::string& frName)
{
    NavWalkResult result;

    std::string script;
    script += "Object fr = Model.find(\"" + fsEscape(frName) + "\");\n";
    script += "Array anchors;\n";
    script += "treenode storedNode = stored(fr);\n";
    script += "if (objectexists(storedNode)) {\n";
    script += "    forobjecttreeunder(storedNode) {\n";
    script += "        treenode target = a.value;\n";
    script += "        if (!objectexists(target)) continue;\n";
    script += "        treenode owner = ownerobject(target);\n";
    script += "        if (!objectexists(owner)) continue;\n";
    script += "        if (isclasstype(owner, \"Navigator\") ||\n";
    script += "            isclasstype(owner, \"AGV::ControlPoint\") ||\n";
    script += "            isclasstype(owner, \"GIS::Point\")) {\n";
    script += "            anchors.push(owner);\n";
    script += "        } else if (objectexists(owner.up) && isclasstype(owner.up, \"Navigator\")) {\n";
    script += "            anchors.push(owner.up);\n";
    script += "        }\n";
    script += "    }\n";
    script += "}\n";
    script += "treenode nnsNode = getvarnode(fr, \"networknodes\");\n";
    script += "if (objectexists(nnsNode)) {\n";
    script += "    forobjecttreeunder(nnsNode) {\n";
    script += "        treenode target = a.value;\n";
    script += "        if (objectexists(target)) anchors.push(ownerobject(target));\n";
    script += "    }\n";
    script += "}\n";
    script += "return anchors;\n";

    Variant v;
    try {
        v = executestring(script.c_str(), nullptr, nullptr, Variant());
    } catch (const std::exception& e) {
        result.error_detail = std::string("walker threw: ") + e.what();
        return result;
    } catch (...) {
        result.error_detail = "walker threw a non-std exception";
        return result;
    }

    if (v.type != VariantType::Array) {
        result.error_detail = "walker did not return an Array (FR may use a "
                              "non-standard >stored/>variables shape — "
                              "duplicate-system check below should not be trusted)";
        return result;
    }

    // From here on the walker SUCCEEDED — empty array is a legitimate
    // "not in any navigator system" answer. Mark ok before populating
    // memberships.
    result.ok = true;

    Array arr = static_cast<Array>(v);
    for (int i = 1; i <= arr.length; ++i) {
        Variant elem = arr[i];
        if (elem.type != VariantType::TreeNode) continue;
        TreeNode* anchor = static_cast<TreeNode*>(elem);
        if (!objectexists(anchor)) continue;

        NavMembership m;
        try { m.anchor_name = getname(anchor); } catch (...) {}
        TreeNode* cls = nullptr;
        try { cls = classobject(anchor); } catch (...) {}
        if (cls) {
            try { m.anchor_class = getname(cls); } catch (...) {}
        }

        if      (isclasstype(anchor, "AGV::ControlPoint"))    m.system = "AGV";
        else if (isclasstype(anchor, "NetworkNode"))          m.system = "Network";
        else if (isclasstype(anchor, "GIS::Point"))           m.system = "GIS";
        else if (isclasstype(anchor, "AStar::AStarNavigator"))m.system = "AStar";
        else if (isclasstype(anchor, "Navigator"))            m.system = "Other";
        else                                                  m.system = "Unknown";

        result.memberships.push_back(std::move(m));
    }
    return result;
}

// Common driver for the four A-connection tools (flow, FR-to-nav,
// dispatcher-to-TE). All hit contextdragconnection(from, to, "A") and
// then verify by walking the port arrays. The classCheckErrorCode lets
// the caller surface its specific class-validation failure code; pass
// empty to skip class checking here (Tool 4 etc. validate before).
Variant doAConnect(const char* toolName,
                   TreeNode* fromNode, const std::string& fromName,
                   TreeNode* toNode,   const std::string& toName)
{
    // Already-connected check via inputs
    int existing = findInPortFor(toNode, fromNode);
    if (existing) {
        return returnError("already_connected",
            "A-connection from '" + fromName + "' to '" + toName +
            "' already exists at to-input port " + std::to_string(existing) + ".");
    }
    try {
        contextdragconnection(fromNode, toNode, 'A');
    } catch (const std::exception& e) {
        return returnError("connect_failed",
            std::string(toolName) + ": contextdragconnection threw: " + e.what());
    } catch (...) {
        return returnError("connect_failed",
            std::string(toolName) + ": contextdragconnection threw a non-std exception.");
    }
    int fromPort = findOutPortFor(fromNode, toNode);
    int toPort   = findInPortFor(toNode,    fromNode);
    if (fromPort == 0 || toPort == 0) {
        return returnError("connect_unverified",
            std::string(toolName) +
            ": contextdragconnection from '" + fromName + "' to '" + toName +
            "' completed but the wire didn't materialize on the expected "
            "port arrays. The class pair may not actually support this "
            "connection.");
    }
    nlohmann::json out;
    out["ok"]        = true;
    out["from"]      = fromName;
    out["to"]        = toName;
    out["from_port"] = fromPort;
    out["to_port"]   = toPort;
    return returnJson(out);
}

} // anonymous

// ============================================================================
// modelerai_connect_fixed_resources({ from, to }):
//   A connection. Item flow. Both ends must be FixedResource.
// ============================================================================
modelerai_export Variant ModelerAi_connectFixedResources(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { from, to } JSON.");
        }
        std::string fromName, toName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            fromName = j.value("from", std::string(""));
            toName   = j.value("to",   std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (fromName.empty() || toName.empty()) {
            return returnError("missing_args", "both from and to are required.");
        }
        TreeNode* fromNode = model()->find(fromName.c_str());
        if (!objectexists(fromNode)) return returnError("from_not_found", "'" + fromName + "' not found");
        TreeNode* toNode   = model()->find(toName.c_str());
        if (!objectexists(toNode))   return returnError("to_not_found",   "'" + toName   + "' not found");

        if (!isclasstype(fromNode, "FixedResource")) {
            return returnError("from_not_fixed_resource",
                "'" + fromName + "' is not a FixedResource. Wrong tool for this pair. "
                "Use modelerai_connect_task_executer_to_navigator, "
                "modelerai_connect_dispatcher_to_task_executer, or "
                "modelerai_connect_centerport depending on the relationship.");
        }
        if (!isclasstype(toNode, "FixedResource")) {
            return returnError("to_not_fixed_resource",
                "'" + toName + "' is not a FixedResource. Did you mean "
                "modelerai_connect_fixed_resource_to_navigator (FR to ControlPoint / "
                "NetworkNode / GIS::Point / AStarNavigator)?");
        }
        return doAConnect("connect_fixed_resources", fromNode, fromName, toNode, toName);
    } catch (const std::exception& e) { return returnException("connect_fixed_resources", e.what()); }
      catch (...)                     { return returnException("connect_fixed_resources", "unknown"); }
}

// ============================================================================
// modelerai_connect_task_executer_to_navigator({ executer, navigator, point? }):
//   setProperty("Navigator", ...) for TE. GIS additionally needs a point +
//   contextdragconnection(te, point, "A").
// ============================================================================
modelerai_export Variant ModelerAi_connectTaskExecuterToNavigator(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { executer, navigator, point? } JSON.");
        }
        std::string teName, navName, pointName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            teName    = j.value("executer",  std::string(""));
            navName   = j.value("navigator", std::string(""));
            pointName = j.value("point",     std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (teName.empty() || navName.empty()) {
            return returnError("missing_args", "executer and navigator are required.");
        }

        TreeNode* te  = model()->find(teName.c_str());
        if (!objectexists(te))  return returnError("executer_not_found",  "'" + teName  + "' not found");
        TreeNode* nav = model()->find(navName.c_str());
        if (!objectexists(nav)) return returnError("navigator_not_found", "'" + navName + "' not found");

        if (!isclasstype(te, "TaskExecuter")) {
            return returnError("not_task_executer",
                "'" + teName + "' is not a TaskExecuter.");
        }

        // Classes (NOT instance names):
        //   AGV::AGVNetwork           — AGV system root
        //   AStar::AStarNavigator     — A* system root
        //   GIS::GISNavigator         — GIS system root
        //   NetworkNavigator          — Network Navigator system root (instance is named "DefaultNetworkNavigator" in models)
        //   Navigator                 — base class; the "default" navigator instance ("DefaultNavigator") is a direct Navigator
        // All specific navigator classes inherit from Navigator, so the
        // base check matches everything. To detect "is this the default
        // navigator (base class instance)?" we check none of the specific
        // subclasses match AND isclasstype(nav, "Navigator") holds.
        bool isGIS    = isclasstype(nav, "GIS::GISNavigator");
        bool isAGV    = isclasstype(nav, "AGV::AGVNetwork");
        bool isAStr   = isclasstype(nav, "AStar::AStarNavigator");
        bool isNN     = isclasstype(nav, "NetworkNavigator");
        bool isAnyNav = isclasstype(nav, "Navigator");

        if (!isAnyNav) {
            return returnError("not_navigator",
                "'" + navName + "' isn't a navigator. Accepted classes: "
                "Navigator (default — the instance is typically named "
                "\"DefaultNavigator\"), NetworkNavigator (instance typically "
                "\"DefaultNetworkNavigator\"), AStar::AStarNavigator, "
                "AGV::AGVNetwork, GIS::GISNavigator.");
        }

        TreeNode* pointNode = nullptr;
        if (isGIS) {
            if (pointName.empty()) {
                return returnError("missing_point",
                    "GIS::GISNavigator requires a 'point' arg (the GIS::Point to attach "
                    "the executer to).");
            }
            pointNode = model()->find(pointName.c_str());
            if (!objectexists(pointNode)) {
                return returnError("point_not_found", "'" + pointName + "' not found");
            }
        }

        // Read current navigator BEFORE — for already-connected detection
        // and to surface the prior binding in the response.
        std::string formerNavName;
        try {
            std::string readScript;
            readScript += "Object te = Model.find(\"" + fsEscape(teName) + "\");\n";
            readScript += "return te.getProperty(\"Navigator\");\n";
            Variant cur = executestring(readScript.c_str(), nullptr, nullptr, Variant());
            if (cur.type == VariantType::TreeNode) {
                TreeNode* nv = static_cast<TreeNode*>(cur);
                if (objectexists(nv)) formerNavName = getname(nv);
            }
        } catch (...) {}

        // Already on the requested navigator? For non-GIS, that's a true
        // no-op and we should reject as already_connected. For GIS, the
        // contextdragconnection would auto-append a duplicate input port,
        // so we also need to short-circuit BEFORE the wire add.
        if (!formerNavName.empty() && formerNavName == navName) {
            if (isGIS) {
                // Check if THIS specific point is already wired to the TE.
                TreeNode* pt = model()->find(pointName.c_str());
                if (objectexists(pt) && findInPortFor(te, pt) != 0) {
                    return returnError("already_connected",
                        "task executer '" + teName + "' is already on navigator '" + navName +
                        "' with GIS point '" + pointName + "' already wired. "
                        "Nothing to do.");
                }
                // Same navigator but a different point — fall through.
                // This is a legitimate add (multi-point GIS membership).
            } else {
                return returnError("already_connected",
                    "task executer '" + teName + "' is already on navigator '" + navName +
                    "'. Use modelerai_disconnect(kind=\"navigator\") first if "
                    "you want to change navigators.");
            }
        }

        // Mutation
        std::string script;
        script += "Object te  = Model.find(\"" + fsEscape(teName)  + "\");\n";
        script += "Object nav = Model.find(\"" + fsEscape(navName) + "\");\n";
        script += "te.setProperty(\"Navigator\", nav);\n";
        if (isGIS) {
            script += "Object pt = Model.find(\"" + fsEscape(pointName) + "\");\n";
            script += "contextdragconnection(te, pt, \"A\");\n";
        }
        script += "return te;\n";

        try {
            executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("setprop_failed",
                std::string("setProperty(\"Navigator\") threw: ") + e.what());
        } catch (...) {
            return returnError("setprop_failed",
                "setProperty(\"Navigator\") threw a non-std exception.");
        }

        // POST-MUTATION VERIFICATION
        // 1. Confirm the Navigator property now equals nav.
        std::string nowNavName;
        try {
            std::string verifyScript;
            verifyScript += "Object te = Model.find(\"" + fsEscape(teName) + "\");\n";
            verifyScript += "return te.getProperty(\"Navigator\");\n";
            Variant after = executestring(verifyScript.c_str(), nullptr, nullptr, Variant());
            if (after.type == VariantType::TreeNode) {
                TreeNode* nv = static_cast<TreeNode*>(after);
                if (objectexists(nv)) nowNavName = getname(nv);
            }
        } catch (...) {
            return returnError("connect_unverified",
                "setProperty completed but post-call getProperty failed; "
                "cannot confirm the navigator binding took effect.");
        }
        if (nowNavName != navName) {
            return returnError("connect_unverified",
                "setProperty(Navigator, " + navName + ") returned but task executer's "
                "Navigator is now '" + nowNavName + "'. FlexSim may have "
                "rejected the assignment.");
        }
        // 2. For GIS, also verify the wire actually landed.
        if (isGIS) {
            TreeNode* pt = model()->find(pointName.c_str());
            if (objectexists(pt) && findInPortFor(te, pt) == 0) {
                return returnError("connect_unverified",
                    "Navigator property was set but contextdragconnection "
                    "to GIS point '" + pointName + "' didn't materialize "
                    "on the task executer's input ports.");
            }
        }

        std::string navClass;
        TreeNode* cls = classobject(nav);
        if (cls) navClass = getname(cls);

        nlohmann::json out;
        out["ok"]              = true;
        out["executer"]        = teName;
        out["navigator"]       = navName;
        out["navigator_class"] = navClass;
        if (!formerNavName.empty() && formerNavName != "DefaultNavigator") {
            out["replaced_navigator"] = formerNavName;
        }
        if (isGIS) out["point"] = pointName;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("connect_task_executer_to_navigator", e.what()); }
      catch (...)                     { return returnException("connect_task_executer_to_navigator", "unknown"); }
}

// ============================================================================
// modelerai_connect_fixed_resource_to_navigator({ fr, anchor }):
//   A connection. fr is a FixedResource. anchor is per-system:
//     AGV     -> AGV::ControlPoint
//     Network -> NetworkNode
//     GIS     -> GIS::Point
//     A*      -> AStar::AStarNavigator (the system itself)
// ============================================================================
modelerai_export Variant ModelerAi_connectFixedResourceToNavigator(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { fr, anchor } JSON.");
        }
        std::string frName, anchorName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            frName     = j.value("fr",     std::string(""));
            anchorName = j.value("anchor", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (frName.empty() || anchorName.empty()) {
            return returnError("missing_args", "fr and anchor are required.");
        }

        TreeNode* fr     = model()->find(frName.c_str());
        if (!objectexists(fr))     return returnError("fr_not_found",     "'" + frName + "' not found");
        TreeNode* anchor = model()->find(anchorName.c_str());
        if (!objectexists(anchor)) return returnError("anchor_not_found", "'" + anchorName + "' not found");

        if (!isclasstype(fr, "FixedResource")) {
            return returnError("not_fixed_resource", "'" + frName + "' is not a FixedResource.");
        }

        std::string system;
        if      (isclasstype(anchor, "AGV::ControlPoint"))     system = "AGV";
        else if (isclasstype(anchor, "NetworkNode"))            system = "Network";
        else if (isclasstype(anchor, "GIS::Point"))             system = "GIS";
        else if (isclasstype(anchor, "AStar::AStarNavigator"))  system = "AStar";
        else {
            return returnError("not_anchor",
                "'" + anchorName + "' isn't a recognized navigation anchor. Accepted: "
                "AGV::ControlPoint, NetworkNode, GIS::Point, AStar::AStarNavigator.");
        }

        // Already-in-system check via the FR tree walker. FRs don't expose
        // a Navigator property (that's TE-only); membership lives in the
        // FR's >stored subtree (AGV/A*/GIS) and >variables/networknodes
        // (Network). Walk + classify by anchor class. If the FR is already
        // in the target system, reject — cross-anchor reconnect doesn't
        // cleanly replace.
        NavWalkResult walkResult = fixedResourceNavigatorMemberships(frName);
        if (!walkResult.ok) {
            // Walker failed — we can't verify duplicate-system safely.
            // Refuse rather than risk silently creating a double-membership
            // (which is hard to detect or repair post-hoc).
            return returnError("inspection_failed",
                "Could not inspect FR's current navigator memberships before "
                "connecting (would risk a silent double-membership). Detail: " +
                walkResult.error_detail + ". Tell the modeler; they may need "
                "to refine the per-FR membership walker for this FR's class.");
        }
        for (const auto& m : walkResult.memberships) {
            if (m.system == system) {
                return returnError("already_in_navigator_system",
                    "'" + frName + "' is already in navigator system '" +
                    system + "' via anchor '" + m.anchor_name + "' (" +
                    m.anchor_class + "). Disconnect first with "
                    "modelerai_disconnect(kind=\"navigator\") then retry "
                    "the connection to the new anchor.");
            }
        }

        Variant base = doAConnect("connect_fixed_resource_to_navigator", fr, frName, anchor, anchorName);
        // If error, base already carries the error JSON. If ok, add system field.
        if (base.type == VariantType::String) {
            try {
                auto j = nlohmann::json::parse(std::string(base));
                if (j.is_object() && j.value("ok", false)) {
                    j["system"]              = system;
                    j["memberships_verified"]= true;
                    j["prior_anchor_check"]  = "not_implemented";
                    return returnJson(j);
                }
            } catch (...) {}
        }
        return base;
    } catch (const std::exception& e) { return returnException("connect_fixed_resource_to_navigator", e.what()); }
      catch (...)                     { return returnException("connect_fixed_resource_to_navigator", "unknown"); }
}

// ============================================================================
// modelerai_connect_dispatcher_to_task_executer({ dispatcher, executer }):
//   A connection. Dispatcher must be a pure Dispatcher (TEs also satisfy
//   isclasstype("Dispatcher") via inheritance — exclude TE).
// ============================================================================
modelerai_export Variant ModelerAi_connectDispatcherToTaskExecuter(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { dispatcher, executer } JSON.");
        }
        std::string dispName, teName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            dispName = j.value("dispatcher", std::string(""));
            teName   = j.value("executer",   std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (dispName.empty() || teName.empty()) {
            return returnError("missing_args", "dispatcher and executer required.");
        }

        TreeNode* disp = model()->find(dispName.c_str());
        if (!objectexists(disp)) return returnError("dispatcher_not_found", "'" + dispName + "' not found");
        TreeNode* te   = model()->find(teName.c_str());
        if (!objectexists(te))   return returnError("executer_not_found",   "'" + teName   + "' not found");

        if (!isclasstype(disp, "Dispatcher") || isclasstype(disp, "TaskExecuter")) {
            return returnError("not_pure_dispatcher",
                "'" + dispName + "' is not a pure Dispatcher (TaskExecuters also "
                "satisfy isclasstype(\"Dispatcher\") via inheritance — exclude TE).");
        }
        if (!isclasstype(te, "TaskExecuter")) {
            return returnError("not_task_executer", "'" + teName + "' is not a TaskExecuter.");
        }
        return doAConnect("connect_dispatcher_to_task_executer", disp, dispName, te, teName);
    } catch (const std::exception& e) { return returnException("connect_dispatcher_to_task_executer", e.what()); }
      catch (...)                     { return returnException("connect_dispatcher_to_task_executer", "unknown"); }
}

// ============================================================================
// modelerai_connect_centerport({ from, to }):
//   S connection. Promiscuous on class pair — used for transport
//   reference (Operator centerObjects[1] = "use transport"), dispatcher
//   serving FR, FR<->FR reference, etc.
// ============================================================================
modelerai_export Variant ModelerAi_connectCenterport(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { from, to } JSON.");
        }
        std::string fromName, toName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            fromName = j.value("from", std::string(""));
            toName   = j.value("to",   std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (fromName.empty() || toName.empty()) {
            return returnError("missing_args", "from and to required.");
        }

        TreeNode* fromNode = model()->find(fromName.c_str());
        if (!objectexists(fromNode)) return returnError("from_not_found", "'" + fromName + "' not found");
        TreeNode* toNode   = model()->find(toName.c_str());
        if (!objectexists(toNode))   return returnError("to_not_found",   "'" + toName   + "' not found");

        // Already-connected check — BIDIRECTIONAL. contextdragconnection 'S'
        // writes both sides when used; but legitimate FlexSim picklists can
        // set up one-sided centerport references (e.g. a "use transport"
        // picklist setting Operator's centerObjects[1] without also setting
        // the FR's centerObjects). So either side having the peer is
        // already-connected — checking only from->to would let
        // connect_centerport(from=A, to=B) then connect_centerport(from=B,
        // to=A) silently create duplicates.
        int existingFromSide = findCenterPortFor(fromNode, toNode);
        int existingToSide   = findCenterPortFor(toNode,   fromNode);
        if (existingFromSide || existingToSide) {
            std::string msg = "Centerport connection between '" + fromName +
                              "' and '" + toName + "' already exists";
            if (existingFromSide) msg += " (from has peer at center index " + std::to_string(existingFromSide) + ")";
            if (existingToSide)   msg += " (to has peer at center index "   + std::to_string(existingToSide)   + ")";
            msg += ".";
            return returnError("already_connected", msg);
        }

        try {
            contextdragconnection(fromNode, toNode, 'S');
        } catch (const std::exception& e) {
            return returnError("connect_failed",
                std::string("contextdragconnection threw: ") + e.what());
        } catch (...) {
            return returnError("connect_failed",
                "contextdragconnection threw a non-std exception.");
        }

        int fromIdx = findCenterPortFor(fromNode, toNode);
        int toIdx   = findCenterPortFor(toNode,   fromNode);
        if (fromIdx == 0 || toIdx == 0) {
            return returnError("connect_unverified",
                "Centerport wire didn't materialize on the expected centerObjects arrays.");
        }

        nlohmann::json out;
        out["ok"]                 = true;
        out["from"]               = fromName;
        out["to"]                 = toName;
        out["from_center_index"]  = fromIdx;
        out["to_center_index"]    = toIdx;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("connect_centerport", e.what()); }
      catch (...)                     { return returnException("connect_centerport", "unknown"); }
}

// ============================================================================
// modelerai_disconnect({ from, to, kind }):
//   kind: "flow" -> contextdragconnection(from, to, "Q")
//         "centerport" -> contextdragconnection(from, to, "W")
//         "navigator" -> from.setProperty("Navigator", Model.find("DefaultNavigator"))
//                         (from must be TE; to is informational)
// ============================================================================
modelerai_export Variant ModelerAi_disconnect(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args", "expects { from, to, kind } JSON.");
        }
        std::string fromName, toName, kind;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            fromName = j.value("from", std::string(""));
            toName   = j.value("to",   std::string(""));
            kind     = j.value("kind", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (fromName.empty() || toName.empty() || kind.empty()) {
            return returnError("missing_args", "from, to, and kind required.");
        }
        if (kind != "flow" && kind != "centerport" && kind != "navigator") {
            return returnError("bad_kind",
                "kind must be 'flow', 'centerport', or 'navigator'. Got: '" + kind + "'.");
        }

        TreeNode* fromNode = model()->find(fromName.c_str());
        if (!objectexists(fromNode)) return returnError("from_not_found", "'" + fromName + "' not found");
        TreeNode* toNode   = model()->find(toName.c_str());
        if (!objectexists(toNode))   return returnError("to_not_found",   "'" + toName   + "' not found");

        if (kind == "flow") {
            int existingTo   = findInPortFor(toNode, fromNode);
            int existingFrom = findOutPortFor(fromNode, toNode);
            if (existingTo == 0 && existingFrom == 0) {
                return returnError("not_connected",
                    "No flow connection from '" + fromName + "' to '" + toName + "' to remove.");
            }
            try {
                contextdragconnection(fromNode, toNode, 'Q');
            } catch (const std::exception& e) {
                return returnError("disconnect_failed", std::string("threw: ") + e.what());
            } catch (...) {
                return returnError("disconnect_failed", "threw non-std exception");
            }
            // Verify gone — BOTH SIDES. Q-disconnect could partially clear
            // (rare but possible — leaves a stale outObjects entry while
            // inObjects is cleaned, or vice versa). The AI must see this
            // as a failure rather than success.
            int stillTo   = findInPortFor(toNode, fromNode);
            int stillFrom = findOutPortFor(fromNode, toNode);
            if (stillTo != 0 || stillFrom != 0) {
                std::string msg = "Flow wire partially present after Q-disconnect:";
                if (stillTo != 0)   msg += " to.inObjects still references from at port " + std::to_string(stillTo) + ";";
                if (stillFrom != 0) msg += " from.outObjects still references to at port " + std::to_string(stillFrom) + ";";
                return returnError("disconnect_unverified", msg);
            }
            nlohmann::json fromOut = nlohmann::json::array();
            int nOut = (int)nrop(fromNode);
            for (int i = 1; i <= nOut; ++i) {
                TreeNode* d = outobject(fromNode, i);
                if (d) fromOut.push_back(getname(d));
            }
            nlohmann::json toIn = nlohmann::json::array();
            int nIn = (int)nrip(toNode);
            for (int i = 1; i <= nIn; ++i) {
                TreeNode* s = inobject(toNode, i);
                if (s) toIn.push_back(getname(s));
            }
            nlohmann::json out;
            out["ok"]                  = true;
            out["kind"]                = "flow";
            out["removed_from_index"]  = existingTo;
            out["removed_to_index"]    = existingFrom;
            out["from_outObjects_now"] = std::move(fromOut);
            out["to_inObjects_now"]    = std::move(toIn);
            return returnJson(out);
        }

        if (kind == "centerport") {
            int existing = findCenterPortFor(fromNode, toNode);
            if (existing == 0) {
                return returnError("not_connected",
                    "No centerport connection between '" + fromName + "' and '" + toName + "'.");
            }
            try {
                contextdragconnection(fromNode, toNode, 'W');
            } catch (const std::exception& e) {
                return returnError("disconnect_failed", std::string("threw: ") + e.what());
            } catch (...) {
                return returnError("disconnect_failed", "threw non-std exception");
            }
            if (findCenterPortFor(fromNode, toNode) != 0) {
                return returnError("disconnect_unverified",
                    "Centerport wire still present after W-disconnect call.");
            }
            nlohmann::json fromC = nlohmann::json::array();
            int nC = (int)nrcp(fromNode);
            for (int i = 1; i <= nC; ++i) {
                TreeNode* p = centerobject(fromNode, i);
                if (p) fromC.push_back(getname(p));
            }
            nlohmann::json toC = nlohmann::json::array();
            int nC2 = (int)nrcp(toNode);
            for (int i = 1; i <= nC2; ++i) {
                TreeNode* p = centerobject(toNode, i);
                if (p) toC.push_back(getname(p));
            }
            nlohmann::json out;
            out["ok"]                      = true;
            out["kind"]                    = "centerport";
            out["removed_from_index"]      = existing;
            out["from_centerObjects_now"]  = std::move(fromC);
            out["to_centerObjects_now"]    = std::move(toC);
            return returnJson(out);
        }

        // kind == "navigator" — different paths per object kind:
        //   TaskExecuter: setProperty("Navigator", DefaultNavigator) — clean,
        //                  unified for all 5 navigator systems.
        //   FixedResource: FRs don't expose the Navigator property; walk the
        //                  FR's >stored + >variables/networknodes for current
        //                  anchors, then contextdragconnection Q each one to
        //                  disconnect. Disconnects from ALL navigator systems
        //                  the FR is in (a single FR is rarely in more than
        //                  one anyway; report what got removed).
        if (isclasstype(fromNode, "TaskExecuter")) {
            // READ current navigator BEFORE mutation so we can:
            //   (a) report the actual former value (not the AI's claim)
            //   (b) verify the disconnect actually changed something
            //   (c) reject if `to` doesn't match what's actually set
            std::string actualFormerName;
            std::string actualFormerClass;
            try {
                std::string readScript;
                readScript += "Object te = Model.find(\"" + fsEscape(fromName) + "\");\n";
                readScript += "return te.getProperty(\"Navigator\");\n";
                Variant cur = executestring(readScript.c_str(), nullptr, nullptr, Variant());
                if (cur.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(cur);
                    if (objectexists(nv)) {
                        actualFormerName = getname(nv);
                        TreeNode* nvCls = classobject(nv);
                        if (nvCls) actualFormerClass = getname(nvCls);
                    }
                }
            } catch (...) {
                return returnError("read_navigator_failed",
                    "Could not read task executer's current Navigator before disconnecting.");
            }

            // Verify `to` matches the actual current navigator. The AI is
            // claiming "disconnect Operator1 from AGVNetwork1"; if the TE
            // isn't actually on AGVNetwork1, the AI's mental model is
            // wrong and we should NOT silently flip it to DefaultNavigator.
            if (actualFormerName != toName) {
                return returnError("not_connected",
                    "task executer '" + fromName + "' is not on '" + toName +
                    "'. Current navigator: '" + actualFormerName + "' (" +
                    actualFormerClass + "). Either retry the disconnect "
                    "with the correct `to`, or use inspect_connections to "
                    "see the actual current Navigator.");
            }

            // Already on the default navigator means there's nothing to
            // do. The default navigator's INSTANCE name is "DefaultNavigator"
            // (the CLASS is "Navigator", but Navigator is the base class
            // and matches every navigator type, so we check by instance
            // name instead).
            if (actualFormerName == "DefaultNavigator") {
                return returnError("already_default",
                    "task executer '" + fromName + "' is already on DefaultNavigator — "
                    "no specific navigator system to leave.");
            }

            // Mutation
            std::string script;
            script += "Object obj = Model.find(\"" + fsEscape(fromName) + "\");\n";
            script += "obj.setProperty(\"Navigator\", Model.find(\"DefaultNavigator\"));\n";
            script += "return obj;\n";
            try {
                executestring(script.c_str(), nullptr, nullptr, Variant());
            } catch (const std::exception& e) {
                return returnError("disconnect_failed",
                    std::string("setProperty(Navigator, DefaultNavigator) threw: ") + e.what());
            } catch (...) {
                return returnError("disconnect_failed",
                    "setProperty(Navigator, DefaultNavigator) threw a non-std exception.");
            }

            // Verify the disconnect actually took.
            std::string nowNavName;
            try {
                std::string verifyScript;
                verifyScript += "Object te = Model.find(\"" + fsEscape(fromName) + "\");\n";
                verifyScript += "return te.getProperty(\"Navigator\");\n";
                Variant after = executestring(verifyScript.c_str(), nullptr, nullptr, Variant());
                if (after.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(after);
                    if (objectexists(nv)) nowNavName = getname(nv);
                }
            } catch (...) {
                return returnError("disconnect_unverified",
                    "setProperty completed but post-call getProperty failed; "
                    "cannot confirm disconnect took effect.");
            }
            if (nowNavName != "DefaultNavigator") {
                return returnError("disconnect_unverified",
                    "setProperty(Navigator, DefaultNavigator) returned but the "
                    "task executer's Navigator is still '" + nowNavName + "'. FlexSim may "
                    "have rejected the change (e.g., simulation running).");
            }

            nlohmann::json out;
            out["ok"]                  = true;
            out["kind"]                = "navigator";
            out["object"]              = fromName;
            out["object_kind"]         = "TaskExecuter";
            out["former_navigator"]    = actualFormerName;
            out["former_navigator_class"] = actualFormerClass;
            out["now_navigator"]       = "DefaultNavigator";
            return returnJson(out);
        }

        if (isclasstype(fromNode, "FixedResource")) {
            NavWalkResult walk = fixedResourceNavigatorMemberships(fromName);
            if (!walk.ok) {
                return returnError("inspection_failed",
                    "Could not inspect FR's navigator memberships to know "
                    "what to disconnect. Detail: " + walk.error_detail);
            }
            if (walk.memberships.empty()) {
                return returnError("not_connected",
                    "fixed resource '" + fromName + "' has no navigator memberships to disconnect.");
            }
            nlohmann::json removed = nlohmann::json::array();
            std::vector<std::string> failures;
            for (const auto& m : walk.memberships) {
                TreeNode* anchor = model()->find(m.anchor_name.c_str());
                if (!objectexists(anchor)) {
                    failures.push_back(m.anchor_name + " (anchor not resolvable)");
                    continue;
                }
                try {
                    contextdragconnection(fromNode, anchor, 'Q');
                } catch (...) {
                    failures.push_back(m.anchor_name + " (Q-disconnect threw)");
                    continue;
                }
                nlohmann::json e;
                e["system"]       = m.system;
                e["anchor"]       = m.anchor_name;
                e["anchor_class"] = m.anchor_class;
                removed.push_back(std::move(e));
            }
            nlohmann::json out;
            out["ok"]          = failures.empty();
            out["kind"]        = "navigator";
            out["object"]      = fromName;
            out["object_kind"] = "FixedResource";
            out["removed"]     = std::move(removed);
            if (!failures.empty()) {
                nlohmann::json fj = nlohmann::json::array();
                for (auto& f : failures) fj.push_back(f);
                out["failures"] = std::move(fj);
                if (removed.empty()) {
                    return returnError("disconnect_failed",
                        "All navigator detachments failed; see `failures` for details. "
                        "Per-system FR-detachment logic may need refinement; tell the "
                        "modeler which system failed.");
                }
            }
            return returnJson(out);
        }

        return returnError("not_supported",
            "navigator disconnect supported only for TaskExecuters and "
            "FixedResources. '" + fromName + "' is neither.");
    } catch (const std::exception& e) { return returnException("disconnect", e.what()); }
      catch (...)                     { return returnException("disconnect", "unknown"); }
}

// ============================================================================
// modelerai_inspect_connections(name | {object}):
//   Read-only walk. Returns inObjects, outObjects, centerObjects, navigator
//   (from getProperty("Navigator")), and groups (from >Groups subtree).
// ============================================================================
modelerai_export Variant ModelerAi_inspectConnections(FLEXSIMINTERFACE)
{
    try {
        std::string name = resolveNameArg(param(1), "object");
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_inspect_connections requires a name (string or "
                "{\"object\": ...}).");
        }
        TreeNode* obj = model()->find(name.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found", "'" + name + "' did not resolve via Model.find.");
        }

        std::string className;
        TreeNode* cls = classobject(obj);
        if (cls) className = getname(cls);

        auto walkPorts = [&](int count, auto getter) {
            nlohmann::json arr = nlohmann::json::array();
            for (int i = 1; i <= count; ++i) {
                TreeNode* peer = getter(i);
                if (!peer) continue;
                nlohmann::json e;
                e["index"] = i;
                e["name"]  = std::string(getname(peer));
                arr.push_back(std::move(e));
            }
            return arr;
        };

        nlohmann::json inArr     = walkPorts((int)nrip(obj),
            [&](int i){ return inobject(obj, i); });
        nlohmann::json outArr    = walkPorts((int)nrop(obj),
            [&](int i){ return outobject(obj, i); });
        nlohmann::json centerArr = walkPorts((int)nrcp(obj),
            [&](int i){ return centerobject(obj, i); });

        // Navigator memberships — different mechanism per object kind.
        // TaskExecuter: getProperty("Navigator") returns the system root
        //   directly (DefaultNavigator if not joined to any specific system).
        // FixedResource: walks >stored + >variables/networknodes via the
        //   helper; returns a list of anchors keyed by system.
        // Other classes: no membership, empty array.
        nlohmann::json navMemberships = nlohmann::json::array();
        std::string    navInspectionError;
        if (isclasstype(obj, "TaskExecuter")) {
            try {
                std::string script;
                script += "Object o = Model.find(\"" + fsEscape(name) + "\");\n";
                script += "return o.getProperty(\"Navigator\");\n";
                Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
                if (v.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(v);
                    if (objectexists(nv)) {
                        std::string nvName = getname(nv);
                        std::string nvClass;
                        TreeNode* nvCls = classobject(nv);
                        if (nvCls) nvClass = getname(nvCls);
                        // Navigator classes (NOT instance names):
                        //   AGV::AGVNetwork, AStar::AStarNavigator,
                        //   GIS::GISNavigator, NetworkNavigator — specific.
                        // Navigator is the base class (matches all of the
                        // above too). The "default" navigator's instance is
                        // typically named "DefaultNavigator" and is a direct
                        // instance of the Navigator base. Detect "default"
                        // by matching Navigator AND failing all specific
                        // subclass checks.
                        std::string system = "Other";
                        if      (isclasstype(nv, "AGV::AGVNetwork"))           system = "AGV";
                        else if (isclasstype(nv, "AStar::AStarNavigator"))     system = "AStar";
                        else if (isclasstype(nv, "GIS::GISNavigator"))         system = "GIS";
                        else if (isclasstype(nv, "NetworkNavigator"))          system = "Network";
                        else if (isclasstype(nv, "Navigator"))                 system = "Default";
                        // Skip emitting an entry when the TE is on DefaultNavigator —
                        // that means "not joined to any specific system."
                        if (system != "Default") {
                            nlohmann::json e;
                            e["system"]       = system;
                            e["anchor"]       = nvName;
                            e["anchor_class"] = nvClass;
                            navMemberships.push_back(std::move(e));
                        }
                    }
                }
            } catch (...) {}
        } else if (isclasstype(obj, "FixedResource")) {
            NavWalkResult walk = fixedResourceNavigatorMemberships(name);
            // Don't silently report "no memberships" when the walker
            // failed. Stash the error_detail so it gets surfaced once `out`
            // is constructed below.
            if (!walk.ok) navInspectionError = walk.error_detail;
            for (const auto& m : walk.memberships) {
                nlohmann::json e;
                e["system"]       = m.system;
                e["anchor"]       = m.anchor_name;
                e["anchor_class"] = m.anchor_class;
                navMemberships.push_back(std::move(e));
            }
        }

        // Groups — walk the >Groups attribute subtree, collect Group owners.
        // Pattern from user: forobjecttreeunder + isclasstype check.
        nlohmann::json groupsArr = nlohmann::json::array();
        try {
            std::string script;
            script += "Object o = Model.find(\"" + fsEscape(name) + "\");\n";
            script += "Array out = [];\n";
            script += "treenode g = o.find(\">Groups\");\n";
            script += "if (objectexists(g)) {\n";
            script += "    forobjecttreeunder(g) {\n";
            script += "        treenode pg = ownerobject(a.value);\n";
            script += "        if (isclasstype(pg, \"Group\"))\n";
            script += "            out.push(pg.name);\n";
            script += "    }\n";
            script += "}\n";
            script += "return out;\n";
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Array) {
                Array a = static_cast<Array>(v);
                for (int i = 1; i <= a.length; ++i) {
                    if (a[i].type == VariantType::String) {
                        groupsArr.push_back(std::string(a[i]));
                    }
                }
            }
        } catch (...) {}

        nlohmann::json out;
        out["ok"]            = true;
        out["object"]        = name;
        out["class"]         = className;
        out["inObjects"]     = std::move(inArr);
        out["outObjects"]    = std::move(outArr);
        out["centerObjects"] = std::move(centerArr);
        out["navigator_memberships"] = std::move(navMemberships);
        if (!navInspectionError.empty()) {
            out["navigator_inspection_error"] = navInspectionError;
        }
        out["groups"]                = std::move(groupsArr);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("inspect_connections", e.what()); }
      catch (...)                     { return returnException("inspect_connections", "unknown"); }
}

// ============================================================================
// modelerai_add_parameter(json_string)
//   json shape:
//     { table_name, name, type, value?, description?, display_units?,
//       lower_bound?, upper_bound?, step_size?, options?, sequence_length?,
//       values?, reference_path?, replace? }
//   type ∈ continuous, integer, discrete, binary, option, sequence,
//          expression, passthrough, custom.
//   Validates type-specific extras up front and hard-rejects cross-table
//   name duplicates (Model.parameters["X"] is GLOBAL, not table-scoped).
// ============================================================================
modelerai_export Variant ModelerAi_addParameter(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_add_parameter expects a single JSON-encoded string arg.");
        }
        std::string argsJson = std::string(arg);

        std::string tableName, name, typeStr;
        std::string description, displayUnits, referencePath;
        std::string onSetBody, onConstrainBody;
        nlohmann::json valueJson;
        std::vector<std::string> optionNames;
        std::vector<double> sequenceValues;
        double lowerBound = 0.0, upperBound = 0.0, stepSize = 0.0;
        int    sequenceLength = 0;
        bool   hasValue = false, hasLower = false, hasUpper = false, hasStep = false;
        bool   hasSeqLen = false;
        bool   replace = false;

        try {
            auto j = nlohmann::json::parse(argsJson);
            if (!j.contains("table_name") || !j["table_name"].is_string()
                || !j.contains("name")    || !j["name"].is_string()
                || !j.contains("type")    || !j["type"].is_string())
            {
                return returnError("missing_field",
                    "add_parameter requires table_name, name, type.");
            }
            tableName    = j["table_name"].get<std::string>();
            name         = j["name"].get<std::string>();
            typeStr      = j["type"].get<std::string>();
            description  = j.value("description", "");
            displayUnits = j.value("display_units", "");
            referencePath= j.value("reference_path", "");
            replace      = j.value("replace", false);
            onSetBody       = j.value("on_set",       std::string(""));
            onConstrainBody = j.value("on_constrain", std::string(""));

            if (j.contains("value")) { valueJson = j["value"]; hasValue = true; }
            if (j.contains("lower_bound") && j["lower_bound"].is_number()) {
                lowerBound = j["lower_bound"].get<double>(); hasLower = true;
            }
            if (j.contains("upper_bound") && j["upper_bound"].is_number()) {
                upperBound = j["upper_bound"].get<double>(); hasUpper = true;
            }
            if (j.contains("step_size") && j["step_size"].is_number()) {
                stepSize = j["step_size"].get<double>(); hasStep = true;
            }
            if (j.contains("sequence_length") && j["sequence_length"].is_number_integer()) {
                sequenceLength = j["sequence_length"].get<int>(); hasSeqLen = true;
            }
            if (j.contains("options") && j["options"].is_array()) {
                for (const auto& it : j["options"]) {
                    if (it.is_string()) optionNames.push_back(it.get<std::string>());
                }
            }
            if (j.contains("values") && j["values"].is_array()) {
                for (const auto& it : j["values"]) {
                    if (it.is_number()) sequenceValues.push_back(it.get<double>());
                }
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json",
                std::string("Couldn't parse args JSON: ") + e.what());
        }

        ParamType type;
        if (!parseParamType(typeStr, type)) {
            return returnError("bad_type",
                "type must be one of: continuous, integer, discrete, binary, option, sequence, expression, passthrough, custom. Got: '" + typeStr + "'.");
        }
        std::string expressionCode;
        if (type == ParamType::Expression) {
            if (!hasValue || !valueJson.is_string()) {
                return returnError("missing_expression",
                    "expression type requires `value` to be a FlexScript string.");
            }
            expressionCode = valueJson.get<std::string>();
            if (expressionCode.empty()) {
                return returnError("empty_expression",
                    "expression value must be non-empty FlexScript.");
            }
        }
        if (tableName.empty() || name.empty()) {
            return returnError("empty_name", "table_name and name must be non-empty.");
        }
        if (type == ParamType::Option && optionNames.empty()) {
            return returnError("missing_options",
                "option type requires a non-empty `options` array.");
        }
        if (type == ParamType::Sequence) {
            if (!hasSeqLen || sequenceLength <= 0) {
                return returnError("missing_sequence_length",
                    "sequence type requires sequence_length > 0.");
            }
            if (!sequenceValues.empty()) {
                if (static_cast<int>(sequenceValues.size()) != sequenceLength) {
                    return returnError("sequence_size_mismatch",
                        "values.length (" + std::to_string(sequenceValues.size())
                        + ") must equal sequence_length (" + std::to_string(sequenceLength) + ").");
                }
                std::vector<bool> seen(sequenceLength, false);
                for (double d : sequenceValues) {
                    int iv = static_cast<int>(d);
                    if (static_cast<double>(iv) != d || iv < 1 || iv > sequenceLength) {
                        return returnError("sequence_value_out_of_range",
                            "Each sequence value must be an integer in [1, " + std::to_string(sequenceLength)
                            + "]. Got: " + std::to_string(d) + ".");
                    }
                    if (seen[iv - 1]) {
                        return returnError("sequence_value_duplicated",
                            "Sequence values must be a PERMUTATION of [1.." + std::to_string(sequenceLength)
                            + "]. Got duplicate: " + std::to_string(iv) + ".");
                    }
                    seen[iv - 1] = true;
                }
            }
        }
        if (type == ParamType::Passthrough && referencePath.empty()) {
            return returnError("missing_reference",
                "passthrough type requires reference_path.");
        }

        // Build the registration FlexScript.
        std::string script;
        script.reserve(1024 + sequenceValues.size() * 16 + optionNames.size() * 32);

        // Table lookup: (1) exact match on the supplied name, (2) fall back to
        // the first existing ParameterTable (FlexSim ships every new model with
        // one — the AI shouldn't have to know its exact name), (3) create a
        // new table only if the container is empty.
        script += "treenode tbl = 0;\n";
        if (!tableName.empty()) {
            script += "tbl = Tools.get(\"ParameterTable\", \""; script += fsEscape(tableName); script += "\");\n";
        }
        script += "if (!tbl) {\n";
        script += "    treenode container = Model.find(\"Tools/ParameterTables\");\n";
        script += "    if (container && container.subnodes.length > 0) {\n";
        script += "        tbl = container.subnodes[1];\n";
        script += "    }\n";
        script += "}\n";
        script += "if (!tbl) {\n";
        script += "    tbl = Tools.create(\"ParameterTable\", \"\");\n";
        script += "    tbl.name = \""; script += fsEscape(tableName.empty() ? std::string("Parameters") : tableName); script += "\";\n";
        script += "}\n";
        script += "if (!tbl) { print(\"add_parameter: could not find or create a ParameterTable\"); return 0; }\n";

        // Cross-table uniqueness (Model.parameters["Name"] is GLOBAL).
        // Use treenode.find (subtree path search) — NodeListArray (subnodes)
        // has assert but NOT find; node.find finds a descendant by relative path.
        script += "treenode allTables = Model.find(\"Tools/ParameterTables\");\n";
        script += "treenode crossTable = 0;\n";
        script += "if (allTables) {\n";
        script += "    for (int iT = 1; iT <= allTables.subnodes.length; iT++) {\n";
        script += "        treenode t = allTables.subnodes[iT];\n";
        script += "        if (t == tbl) continue;\n";
        script += "        treenode psOther = t.find(\"variables\");\n";
        script += "        if (!psOther) continue;\n";
        script += "        psOther = psOther.find(\"parameters\");\n";
        script += "        if (!psOther) continue;\n";
        script += "        for (int iPo = 1; iPo <= psOther.subnodes.length; iPo++) {\n";
        script += "            treenode po = psOther.subnodes[iPo];\n";
        script += "            treenode nmo = po.find(\"Name\");\n";
        script += "            if (nmo && nmo.value == \""; script += fsEscape(name); script += "\") {\n";
        script += "                crossTable = t; break;\n";
        script += "            }\n";
        script += "        }\n";
        script += "        if (crossTable) break;\n";
        script += "    }\n";
        script += "}\n";
        script += "if (crossTable) { print(\"add_parameter: name '"; script += fsEscape(name);
        script += "' already exists in another table: \" + crossTable.name + \".\"); return 0; }\n";

        // Same-table duplicate.
        script += "treenode pset = tbl.subnodes.assert(\"variables\").subnodes.assert(\"parameters\");\n";
        script += "treenode existing = 0;\n";
        script += "for (int iX = 1; iX <= pset.subnodes.length; iX++) {\n";
        script += "    treenode p = pset.subnodes[iX];\n";
        script += "    treenode nm = p.find(\"Name\");\n";
        script += "    if (nm && nm.value == \""; script += fsEscape(name); script += "\") { existing = p; break; }\n";
        script += "}\n";
        if (replace) {
            script += "if (existing) existing.destroy();\n";
        } else {
            script += "if (existing) { print(\"add_parameter: duplicate name in same table (use replace=true): "; script += fsEscape(name); script += "\"); return 0; }\n";
        }

        // Create row + configure.
        script += "treenode np = function_s(tbl, \"addParameter\");\n";
        script += "np.subnodes.assert(\"Name\").value = \""; script += fsEscape(name); script += "\";\n";
        script += "treenode v = np.subnodes.assert(\"Value\");\n";
        script += "v.subnodes.assert(\"type\").value = "; script += std::to_string(static_cast<int>(type)); script += ";\n";

        auto setNum = [&](const char* sub, double n) {
            script += "v.subnodes.assert(\""; script += sub; script += "\").value = ";
            script += std::to_string(n); script += ";\n";
        };

        if (type == ParamType::Continuous || type == ParamType::Integer || type == ParamType::Discrete) {
            // ALWAYS emit bounds for numeric types. FlexSim's default
            // upperBound on a fresh row is 10, and onConstrain clamps any
            // value above that, producing "Unable to set X to Y; using 10
            // instead" warnings. Permissive defaults (-1e9 / 1e9) when the
            // caller doesn't specify, so the value assignment below succeeds.
            setNum("lowerBound", hasLower ? lowerBound : -1e9);
            setNum("upperBound", hasUpper ? upperBound :  1e9);
        }
        if (type == ParamType::Discrete && hasStep) setNum("stepSize", stepSize);

        if (type == ParamType::Option) {
            script += "treenode opts = v.subnodes.assert(\"options\");\n";
            // Bounded purge — see comment in trigger tools. FlexScript has
            // no try/catch; an uncapped while-destroy can freeze the engine.
            script += "for (int __pO = 0; __pO < 4096 && opts.subnodes.length; __pO++) opts.subnodes[1].destroy();\n";
            for (size_t i = 0; i < optionNames.size(); ++i) {
                script += "opts.subnodes.assert(\""; script += fsEscape(optionNames[i]); script += "\").value = ";
                script += std::to_string(static_cast<int>(i) + 1); script += ";\n";
            }
        }

        if (type == ParamType::Sequence) {
            setNum("sequenceLength", static_cast<double>(sequenceLength));
            if (!sequenceValues.empty()) {
                script += "Array seq = [";
                for (size_t i = 0; i < sequenceValues.size(); ++i) {
                    if (i) script += ",";
                    script += std::to_string(static_cast<int>(sequenceValues[i]));
                }
                script += "];\n";
                script += "v.value = seq;\n";
            }
        }

        if (type == ParamType::Expression) {
            script += "treenode exprNode = v.subnodes.assert(\"expression\");\n";
            script += "exprNode.value = \""; script += fsEscape(expressionCode); script += "\";\n";
            script += "enablecode(exprNode);\n";
            script += "buildnodeflexscript(exprNode);\n";
            script += "v.value = \""; script += fsEscape(expressionCode); script += "\";\n";
        }

        if (type == ParamType::Passthrough || type == ParamType::Custom) {
            if (!referencePath.empty()) {
                script += "treenode refn = Model.find(\""; script += fsEscape(referencePath); script += "\");\n";
                script += "if (!refn) { print(\"add_parameter: reference_path not found: "; script += fsEscape(referencePath); script += "\"); return 0; }\n";
                script += "v.subnodes.assert(\"reference\").value = refn;\n";
            }
        }

        if (hasValue) {
            if (type == ParamType::Sequence || type == ParamType::Passthrough || type == ParamType::Expression) {
                // ignored — handled above
            } else if (valueJson.is_number()) {
                script += "v.value = "; script += std::to_string(valueJson.get<double>()); script += ";\n";
            } else if (valueJson.is_string()) {
                script += "v.value = \""; script += fsEscape(valueJson.get<std::string>()); script += "\";\n";
            }
        }

        if (!description.empty()) {
            script += "np.subnodes.assert(\"Description\").value = \""; script += fsEscape(description); script += "\";\n";
        }
        if (!displayUnits.empty()) {
            script += "np.subnodes.assert(\"Display Units\").value = \""; script += fsEscape(displayUnits); script += "\";\n";
        }

        // on_set / on_constrain — script hooks. Same pattern as Expression:
        // write source, enablecode, buildnodeflexscript. Bodies come from
        // the picklist library (flexsim-parameter-onset). The 4-param
        // signature is mandatory:
        //   treenode reference = param(1);
        //   Variant  newValue  = param(2);
        //   Variant  oldValue  = param(3);
        //   int      isReset   = param(4);
        if (!onSetBody.empty()) {
            // Auto-prepend the 4-param signature if the body uses
            // `reference`/`newValue`/`oldValue`/`isReset` without
            // declaring them. Same QoL as PM expression bodies; the
            // picklist library template bodies declare the params, so
            // bodies copied verbatim work as-is. Bodies that just paste
            // the action without the preamble get the preamble inserted.
            bool usesRef     = onSetBody.find("reference")     != std::string::npos;
            bool declRef     = onSetBody.find("treenode reference") != std::string::npos;
            bool usesNew     = onSetBody.find("newValue")      != std::string::npos;
            bool declNew     = onSetBody.find("Variant newValue") != std::string::npos
                            || onSetBody.find("Variant  newValue") != std::string::npos;
            bool usesOld     = onSetBody.find("oldValue")      != std::string::npos;
            bool declOld     = onSetBody.find("Variant oldValue") != std::string::npos
                            || onSetBody.find("Variant  oldValue") != std::string::npos;
            bool usesReset   = onSetBody.find("isReset")       != std::string::npos;
            bool declReset   = onSetBody.find("int isReset")   != std::string::npos
                            || onSetBody.find("int  isReset")  != std::string::npos;
            std::string preamble;
            if (usesRef   && !declRef)   preamble += "treenode reference = param(1);\n";
            if (usesNew   && !declNew)   preamble += "Variant newValue = param(2);\n";
            if (usesOld   && !declOld)   preamble += "Variant oldValue = param(3);\n";
            if (usesReset && !declReset) preamble += "int isReset = param(4);\n";
            std::string finalOnSet = preamble + onSetBody;

            // Run the antipattern scanner on the body — same protection
            // as run_script. Deprecated forms in onSet bodies become
            // production bugs the modeler can't easily inspect.
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(finalOnSet, anName, anRem)) {
                return returnError("deprecated_api_in_on_set",
                    "on_set body uses a deprecated/hallucinated API: " + anRem);
            }
            script += "treenode onSetHook = np.subnodes.assert(\"onSet\");\n";
            script += "onSetHook.value = \""; script += fsEscape(finalOnSet); script += "\";\n";
            script += "enablecode(onSetHook);\n";
            script += "buildnodeflexscript(onSetHook);\n";
        }
        if (!onConstrainBody.empty()) {
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(onConstrainBody, anName, anRem)) {
                return returnError("deprecated_api_in_on_constrain",
                    "on_constrain body uses a deprecated/hallucinated API: " + anRem);
            }
            script += "treenode onConstrainHook = np.subnodes.assert(\"onConstrain\");\n";
            script += "onConstrainHook.value = \""; script += fsEscape(onConstrainBody); script += "\";\n";
            script += "enablecode(onConstrainHook);\n";
            script += "buildnodeflexscript(onConstrainHook);\n";
        }

        script += "return np;\n";

        // Execute (already on the main thread).
        std::string newPath;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type != VariantType::TreeNode) {
                ModelerAi::bridge::consolePrint(
                    "[ModelerAI] add_parameter FlexScript returned non-treenode. Script was:\n----\n"
                    + script + "----\n");
                return returnError("registration_failed",
                    "Registration FlexScript did not return a treenode. Script was:\n" + script);
            }
            treenode created = static_cast<treenode>(v);
            if (!created) {
                ModelerAi::bridge::consolePrint(
                    "[ModelerAI] add_parameter FlexScript returned null node. Script was:\n----\n"
                    + script + "----\n");
                return returnError("registration_failed",
                    "Registration FlexScript returned a null node. Script was:\n" + script);
            }
            try { newPath = std::string(nodetomodelpath(created, 1).c_str()); } catch (...) {}
        } catch (const std::exception& e) {
            ModelerAi::bridge::consolePrint(
                "[ModelerAI] add_parameter threw. Script was:\n----\n" + script + "----\n");
            return returnError("registration_failed",
                std::string(e.what()) + "\n--- script ---\n" + script);
        } catch (const char* msg) {
            ModelerAi::bridge::consolePrint(
                "[ModelerAI] add_parameter threw. Script was:\n----\n" + script + "----\n");
            return returnError("registration_failed",
                std::string(msg ? msg : "FlexScript error (unknown)") + "\n--- script ---\n" + script);
        } catch (...) {
            ModelerAi::bridge::consolePrint(
                "[ModelerAI] add_parameter threw unknown. Script was:\n----\n" + script + "----\n");
            return returnError("registration_failed",
                std::string("FlexScript threw a non-standard exception.\n--- script ---\n") + script);
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["table"]    = tableName;
        out["name"]     = name;
        out["type"]     = typeStr;
        out["path"]     = newPath;
        out["accessor"] = "Model.parameters[\"" + name + "\"]";
        if (!onSetBody.empty())       out["on_set_applied"]       = true;
        if (!onConstrainBody.empty()) out["on_constrain_applied"] = true;

        // For numeric-valued types, read the value back and surface any
        // clamping that FlexSim's onConstrain performed. Without this the
        // tool reports "ok: true" even though the stored value differs from
        // what the AI requested (common with Discrete + step_size, or values
        // outside the bounds). The AI sees `clamped: true` + the actual
        // value and can retry with compatible args.
        if (hasValue && valueJson.is_number()
            && (type == ParamType::Continuous || type == ParamType::Integer
             || type == ParamType::Discrete   || type == ParamType::Binary))
        {
            std::string readback = "return Model.parameters[\"" + name + "\"].value;";
            try {
                Variant av = executestring(readback.c_str(), nullptr, nullptr, Variant());
                if (av.type == VariantType::Number) {
                    double actual = static_cast<double>(av);
                    double requested = valueJson.get<double>();
                    out["actual_value"] = actual;
                    if (std::abs(actual - requested) > 1e-6) {
                        out["clamped"] = true;
                        out["requested_value"] = requested;
                        out["clamp_reason"] = "FlexSim's onConstrain adjusted the value. "
                                              "Likely cause: value doesn't fit step_size (Discrete) "
                                              "or is outside [lower_bound, upper_bound]. Retry with "
                                              "a value that satisfies: value == lower_bound + n*step_size "
                                              "for Discrete, or value in [lower_bound, upper_bound] "
                                              "for Continuous/Integer.";
                    }
                }
            } catch (...) {
                // readback failure is non-fatal — caller still has the row
            }
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_parameter", e.what()); }
      catch (...)                     { return returnException("add_parameter", "unknown"); }
}

// ============================================================================
// modelerai_set_parameter(json_string)
//   json shape:
//     { table_name, name, value?, description?, display_units?,
//       lower_bound?, upper_bound?, step_size?, sequence_length?, values? }
//   Pass-through update. Type validation is the caller's responsibility —
//   add_parameter validates strictly; set_parameter trusts and lets
//   FlexSim's onConstrain reject if the value violates the type contract.
// ============================================================================
modelerai_export Variant ModelerAi_setParameter(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_parameter expects a single JSON-encoded string arg.");
        }
        std::string argsJson = std::string(arg);

        std::string tableName, name;
        nlohmann::json valueJson;
        bool hasValue = false;
        std::string description, displayUnits;
        bool hasDesc = false, hasUnits = false;
        double lowerBound = 0, upperBound = 0, stepSize = 0;
        bool hasLower = false, hasUpper = false, hasStep = false;
        int sequenceLength = 0; bool hasSeqLen = false;
        std::vector<double> sequenceValues;

        try {
            auto j = nlohmann::json::parse(argsJson);
            if (!j.contains("name") || !j["name"].is_string())
            {
                return returnError("missing_field",
                    "set_parameter requires a `name` field. `table_name` is optional — parameter names are global across all ParameterTables, so set_parameter searches every table for the name.");
            }
            tableName = j.value("table_name", "");
            name      = j["name"].get<std::string>();
            if (j.contains("value")) { valueJson = j["value"]; hasValue = true; }
            if (j.contains("description") && j["description"].is_string()) {
                description = j["description"].get<std::string>(); hasDesc = true;
            }
            if (j.contains("display_units") && j["display_units"].is_string()) {
                displayUnits = j["display_units"].get<std::string>(); hasUnits = true;
            }
            if (j.contains("lower_bound") && j["lower_bound"].is_number()) {
                lowerBound = j["lower_bound"].get<double>(); hasLower = true;
            }
            if (j.contains("upper_bound") && j["upper_bound"].is_number()) {
                upperBound = j["upper_bound"].get<double>(); hasUpper = true;
            }
            if (j.contains("step_size") && j["step_size"].is_number()) {
                stepSize = j["step_size"].get<double>(); hasStep = true;
            }
            if (j.contains("sequence_length") && j["sequence_length"].is_number_integer()) {
                sequenceLength = j["sequence_length"].get<int>(); hasSeqLen = true;
            }
            if (j.contains("values") && j["values"].is_array()) {
                for (const auto& it : j["values"]) if (it.is_number()) sequenceValues.push_back(it.get<double>());
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json",
                std::string("Couldn't parse args JSON: ") + e.what());
        }

        if (!hasValue && !hasDesc && !hasUnits && !hasLower && !hasUpper && !hasStep
            && !hasSeqLen && sequenceValues.empty())
        {
            return returnError("nothing_to_update",
                "set_parameter needs at least one of: value, description, display_units, lower_bound, upper_bound, step_size, sequence_length, values.");
        }

        // Parameter names are GLOBAL — search every ParameterTable for the
        // name rather than requiring the caller to know which table holds it.
        // table_name (if supplied) is informational; we still return where
        // we found it so the caller can verify.
        std::string script;
        script.reserve(1024 + sequenceValues.size() * 16);
        script += "treenode allTables = Model.find(\"Tools/ParameterTables\");\n";
        script += "if (!allTables) { print(\"set_parameter: Tools/ParameterTables container missing\"); return 0; }\n";
        script += "treenode found = 0;\n";
        script += "treenode owningTable = 0;\n";
        script += "for (int iT = 1; iT <= allTables.subnodes.length; iT++) {\n";
        script += "    treenode t = allTables.subnodes[iT];\n";
        script += "    treenode ps = t.find(\"variables\");\n";
        script += "    if (!ps) continue;\n";
        script += "    ps = ps.find(\"parameters\");\n";
        script += "    if (!ps) continue;\n";
        script += "    for (int iX = 1; iX <= ps.subnodes.length; iX++) {\n";
        script += "        treenode p = ps.subnodes[iX];\n";
        script += "        treenode nm = p.find(\"Name\");\n";
        script += "        if (nm && nm.value == \""; script += fsEscape(name); script += "\") { found = p; owningTable = t; break; }\n";
        script += "    }\n";
        script += "    if (found) break;\n";
        script += "}\n";
        script += "if (!found) { print(\"set_parameter: parameter not found in any ParameterTable: "; script += fsEscape(name); script += "\"); return 0; }\n";
        script += "treenode v = found.subnodes.assert(\"Value\");\n";

        // Bounds + structural fields FIRST, then value. If we set value before
        // bounds, FlexSim's onConstrain clamps against the OLD upperBound and
        // the new value is silently lost ("Unable to set X to Y; using <old>
        // instead").
        auto setNum = [&](const char* sub, double n) {
            script += "v.subnodes.assert(\""; script += sub; script += "\").value = "; script += std::to_string(n); script += ";\n";
        };
        if (hasLower)  setNum("lowerBound", lowerBound);
        if (hasUpper)  setNum("upperBound", upperBound);
        if (hasStep)   setNum("stepSize",   stepSize);
        if (hasSeqLen) setNum("sequenceLength", static_cast<double>(sequenceLength));

        if (hasValue) {
            if (valueJson.is_number()) {
                script += "v.value = "; script += std::to_string(valueJson.get<double>()); script += ";\n";
            } else if (valueJson.is_string()) {
                script += "v.value = \""; script += fsEscape(valueJson.get<std::string>()); script += "\";\n";
            } else if (valueJson.is_array()) {
                script += "Array seq = [";
                for (size_t i = 0; i < valueJson.size(); ++i) {
                    if (i) script += ",";
                    if (valueJson[i].is_number()) script += std::to_string(valueJson[i].get<double>());
                    else script += "0";
                }
                script += "];\n";
                script += "v.value = seq;\n";
            }
        }
        if (!sequenceValues.empty()) {
            script += "Array seq2 = [";
            for (size_t i = 0; i < sequenceValues.size(); ++i) {
                if (i) script += ",";
                script += std::to_string(static_cast<int>(sequenceValues[i]));
            }
            script += "];\n";
            script += "v.value = seq2;\n";
        }
        if (hasDesc) {
            script += "found.subnodes.assert(\"Description\").value = \""; script += fsEscape(description); script += "\";\n";
        }
        if (hasUnits) {
            script += "found.subnodes.assert(\"Display Units\").value = \""; script += fsEscape(displayUnits); script += "\";\n";
        }
        script += "return found;\n";

        // Build a small debug payload helper — the generated FlexScript can
        // run to 30+ lines, which is enormous for an AI tool-result. Surface
        // a terse message; tuck the script + parameter context under `debug`
        // so curious sessions can inspect, and dump to the bridge console
        // for log-side debugging.
        auto buildDebug = [&](const char* reason) {
            nlohmann::json d;
            d["script"]       = script;
            d["table"]        = tableName;
            d["parameter"]    = name;
            d["failure_mode"] = reason;
            return d;
        };

        std::string newPath;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type != VariantType::TreeNode) {
                ModelerAi::bridge::consolePrint(
                    "[ModelerAI] set_parameter FlexScript returned non-treenode for '"
                    + name + "'.\n");
                return returnErrorWithDebug("update_failed",
                    "Update FlexScript did not return a treenode for parameter '" + name +
                    "'. See debug.script for the generated body.",
                    buildDebug("non_treenode_return"));
            }
            treenode found = static_cast<treenode>(v);
            if (!found) {
                ModelerAi::bridge::consolePrint(
                    "[ModelerAI] set_parameter FlexScript returned null for '" + name + "'.\n");
                return returnErrorWithDebug("update_failed",
                    "Update FlexScript returned a null node for parameter '" + name +
                    "'. See debug.script for the generated body.",
                    buildDebug("null_node_return"));
            }
            try { newPath = std::string(nodetomodelpath(found, 1).c_str()); } catch (...) {}
        } catch (const std::exception& e) {
            ModelerAi::bridge::consolePrint(
                std::string("[ModelerAI] set_parameter threw: ") + e.what() + "\n");
            return returnErrorWithDebug("update_failed",
                std::string("FlexScript threw on parameter '") + name + "': " + e.what(),
                buildDebug("std_exception"));
        } catch (const char* msg) {
            ModelerAi::bridge::consolePrint(
                std::string("[ModelerAI] set_parameter threw: ") + (msg ? msg : "(null)") + "\n");
            return returnErrorWithDebug("update_failed",
                std::string("FlexScript threw on parameter '") + name + "': " +
                (msg ? msg : "(unknown)"),
                buildDebug("cstr_exception"));
        } catch (...) {
            ModelerAi::bridge::consolePrint(
                "[ModelerAI] set_parameter threw a non-standard exception.\n");
            return returnErrorWithDebug("update_failed",
                "FlexScript threw a non-standard exception on parameter '" + name + "'.",
                buildDebug("unknown_exception"));
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["table"]    = tableName;
        out["name"]     = name;
        out["path"]     = newPath;
        out["accessor"] = "Model.parameters[\"" + name + "\"]";

        // Surface any onConstrain clamping that happened on the value update
        // (same mechanism as add_parameter). See the corresponding block in
        // ModelerAi_addParameter for rationale.
        if (hasValue && valueJson.is_number()) {
            std::string readback = "return Model.parameters[\"" + name + "\"].value;";
            try {
                Variant av = executestring(readback.c_str(), nullptr, nullptr, Variant());
                if (av.type == VariantType::Number) {
                    double actual = static_cast<double>(av);
                    double requested = valueJson.get<double>();
                    out["actual_value"] = actual;
                    if (std::abs(actual - requested) > 1e-6) {
                        out["clamped"] = true;
                        out["requested_value"] = requested;
                        out["clamp_reason"] = "FlexSim's onConstrain adjusted the value. "
                                              "Likely cause: value doesn't fit step_size or is "
                                              "outside the current [lower_bound, upper_bound]. "
                                              "Retry with a value that satisfies the constraints, "
                                              "or update the bounds/step in the same call.";
                    }
                }
            } catch (...) {}
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_parameter", e.what()); }
      catch (...)                     { return returnException("set_parameter", "unknown"); }
}

// ============================================================================
// modelerai_remove_parameter({ name, table_name? })
//
// Closes the parameter trio (add / set / remove). Parameter names are
// global across all ParameterTables — table_name is optional and only
// used to disambiguate if the same name exists in multiple tables.
// Returns not_found if the parameter doesn't exist anywhere.
// ============================================================================
modelerai_export Variant ModelerAi_removeParameter(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_remove_parameter expects { name, table_name? } JSON.");
        }
        std::string tableName, paramName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            tableName = j.value("table_name", std::string(""));
            paramName = j.value("name",       std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (paramName.empty()) {
            return returnError("missing_args", "name is required.");
        }

        std::string script;
        script += "treenode allTables = Model.find(\"Tools/ParameterTables\");\n";
        script += "if (!allTables) return 0;\n";
        script += "treenode found = 0;\n";
        script += "string ownerName = \"\";\n";
        script += "string wantTable = \"" + fsEscape(tableName) + "\";\n";
        script += "for (int iT = 1; iT <= allTables.subnodes.length; iT++) {\n";
        script += "    treenode t = allTables.subnodes[iT];\n";
        script += "    if (wantTable != \"\" && string(getname(t)) != wantTable) continue;\n";
        script += "    treenode ps = t.find(\"variables\");\n";
        script += "    if (!ps) continue;\n";
        script += "    ps = ps.find(\"parameters\");\n";
        script += "    if (!ps) continue;\n";
        script += "    for (int iX = 1; iX <= ps.subnodes.length; iX++) {\n";
        script += "        treenode p = ps.subnodes[iX];\n";
        script += "        treenode nm = p.find(\"Name\");\n";
        script += "        if (nm && nm.value == \"" + fsEscape(paramName) + "\") {\n";
        script += "            found = p;\n";
        script += "            ownerName = string(getname(t));\n";
        script += "            break;\n";
        script += "        }\n";
        script += "    }\n";
        script += "    if (found) break;\n";
        script += "}\n";
        script += "if (!found) return 0;\n";
        script += "destroyobject(found);\n";
        script += "return ownerName;\n";

        std::string owner;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::String) owner = std::string(v);
            else if (v.type == VariantType::Number && static_cast<double>(v) == 0.0) {
                nlohmann::json err;
                err["error"] = "not_found";
                err["name"]  = paramName;
                if (!tableName.empty()) err["table_name"] = tableName;
                err["note"]  = tableName.empty()
                    ? "Parameter not found in any ParameterTable."
                    : "Parameter not found in the specified table.";
                return returnJson(err);
            }
        } catch (const std::exception& e) {
            return returnError("remove_failed",
                "ParameterTable walk threw: " + std::string(e.what()));
        } catch (...) {
            return returnError("remove_failed", "ParameterTable walk threw a non-std exception.");
        }

        nlohmann::json out;
        out["ok"]         = true;
        out["name"]       = paramName;
        out["table_name"] = owner;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("remove_parameter", e.what()); }
      catch (...)                     { return returnException("remove_parameter", "unknown"); }
}

// ============================================================================
// EXECUTION DOMAIN — commands
// ============================================================================

// ============================================================================
// modelerai_notify_run_state(state_name, sim_time)
//   Trigger callback. Invoked from FlexScript inside our installed hook blocks
//   in OnRunStart / OnRunStop / OnModelReset. Records the state + time and
//   signals the run-stop event so any waiter wakes.
// ============================================================================
modelerai_export Variant ModelerAi_notifyRunState(FLEXSIMINTERFACE)
{
    try {
        std::string state = strParam(param(1));
        double simTime    = numParam(param(2), 0.0);
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            g_lastRunState = state.empty() ? std::string("Unknown") : state;
            g_lastSimTime  = simTime;
        }
        ensureRunStopEvent();
        // Signal only on Stopped / Reset — Started signaling would wake the
        // waiter immediately after a run starts, which isn't what waiters want.
        if (state == "Stopped" || state == "Reset") {
            SetEvent(g_runStopEvent);
        }
        ModelerAi::bridge::consolePrint(
            std::string("[ModelerAI] run-state notify: ") + state +
            " @ t=" + std::to_string(simTime) + "\n");
        return Variant(0.0);  // FlexScript callers don't use the return.
    } catch (...) {
        return Variant(0.0);
    }
}

// ============================================================================
// modelerai_install_run_hooks()
//   Idempotent. Ensures the three ModelTriggers exist and that our marker
//   block is at the bottom of each. Auto-called lazily by run / run_to_time;
//   exposed as an explicit command so the AI can pre-install (or re-install
//   after a hook-version bump) on demand.
// ============================================================================
modelerai_export Variant ModelerAi_installRunHooks(FLEXSIMINTERFACE)
{
    try {
        std::string err;
        int n = ensureAllHooksInstalled(err);
        nlohmann::json out;
        out["ok"] = (n == 3);
        out["installed"] = n;
        out["hook_version"] = kHookVersion;
        if (!err.empty()) out["partial_errors"] = err;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("install_run_hooks", e.what()); }
      catch (...)                     { return returnException("install_run_hooks", "unknown"); }
}

// ============================================================================
// modelerai_uninstall_run_hooks()
//   Finds and removes our marker blocks from all three triggers, rebuilds.
//   Useful before sharing a model with someone who doesn't have ModelerAI.
// ============================================================================
modelerai_export Variant ModelerAi_uninstallRunHooks(FLEXSIMINTERFACE)
{
    try {
        const char* triggers[] = { "OnRunStart", "OnRunStop", "OnModelReset" };
        int removed = 0;
        std::string err;
        for (const char* t : triggers) {
            std::string e;
            if (uninstallHookFromTrigger(t, e)) removed++;
            else if (!e.empty()) err += std::string(t) + ": " + e + "; ";
        }
        nlohmann::json out;
        out["ok"] = removed > 0;
        out["removed"] = removed;
        if (!err.empty()) out["partial_errors"] = err;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("uninstall_run_hooks", e.what()); }
      catch (...)                     { return returnException("uninstall_run_hooks", "unknown"); }
}

// ============================================================================
// modelerai_reset_model()
//   Full reset matching the GUI's behavior: notify document listeners, clear
//   stats-delegate "initialized" flags, then resetmodel(1).
// ============================================================================
modelerai_export Variant ModelerAi_resetModel(FLEXSIMINTERFACE)
{
    try {
        std::string script;
        // Clear stats-delegate initialized vars (matches the GUI's Execute/reset block).
        script += "treenode delegates = node(\"MAIN:/project/exec/globals/serverinterface/delegates\");\n";
        script += "if (delegates && content(delegates) > 0) {\n";
        script += "    for (int ci = 1; ci <= content(delegates); ci++) {\n";
        script += "        treenode container = rank(delegates, ci);\n";
        script += "        for (int i = 1; i <= content(container); i++) {\n";
        script += "            setvarnum(rank(container, i), \"initialized\", 0);\n";
        script += "        }\n";
        script += "    }\n";
        script += "}\n";
        script += "applicationcommand(\"notifydoclistenersoncustomaction\", \"Reset\");\n";
        script += "resetmodel(1);\n";
        script += "applicationcommand(\"switchRunning\", 0);\n";
        script += "return time();\n";

        double finalTime = 0.0;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Number) finalTime = double(v);
        } catch (...) {}

        nlohmann::json out;
        out["ok"] = true;
        out["sim_time"] = finalTime;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("reset_model", e.what()); }
      catch (...)                     { return returnException("reset_model", "unknown"); }
}

// ============================================================================
// modelerai_run_to_time(target_sim_time)
//   Adds a temporary stop time at `target`, kicks off go(1), waits on the
//   run-stop event (signaled by our OnRunStop hook), reads final state +
//   sim time, identifies WHICH stop fired (target / earlier user stop /
//   other), cleans up the temp stop, returns.
// ============================================================================
modelerai_export Variant ModelerAi_runToTime(FLEXSIMINTERFACE)
{
    try {
        // Accept whatever shape the AI sends:
        //   - bare number:    600
        //   - 1-arg pos:      [600]
        //   - 2-arg pos:      [600, 60]   (target + timeout_seconds)
        //   - JSON object:    "{\"target_sim_time\": 600, \"timeout_seconds\": 60}"
        //                     also accepts "target" or "time" as the key
        //                     and "timeout" as the timeout key.
        //
        // Original tool only took a bare positional Number. Frustrated the
        // AI into 2-4 wasted calls per run (export 2026-06-07 showed
        // {"time": 600} and {"target_sim_time": 600} both rejected before
        // the AI guessed the right shape). Default timeout stays 300s.
        Variant arg     = param(1);
        Variant arg2    = param(2);
        double  target      = 0.0;
        double  timeoutSec  = 300.0;
        bool    haveTarget  = false;

        if (arg.type == VariantType::Number) {
            target = double(arg);
            haveTarget = true;
            if (arg2.type == VariantType::Number) timeoutSec = double(arg2);
        } else if (arg.type == VariantType::String) {
            std::string s = std::string(arg);
            size_t lead = s.find_first_not_of(" \t\r\n");
            char head = (lead == std::string::npos) ? '\0' : s[lead];
            if (head == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) {
                        // Try each common key for the target.
                        for (const char* k : { "target_sim_time", "target", "time", "sim_time" }) {
                            if (j.contains(k) && j[k].is_number()) {
                                target = j[k].get<double>();
                                haveTarget = true;
                                break;
                            }
                        }
                        for (const char* k : { "timeout_seconds", "timeout", "wall_timeout" }) {
                            if (j.contains(k) && j[k].is_number()) {
                                timeoutSec = j[k].get<double>();
                                break;
                            }
                        }
                    }
                } catch (...) {
                    // Fall through to the missing_target error below.
                }
            }
        }

        if (!haveTarget) {
            nlohmann::json out;
            out["ok"] = false;
            out["error_code"]    = "missing_target";
            out["error_message"] = "modelerai_run_to_time expects a numeric target sim time. "
                                   "Any of these shapes work: bare number 600, positional [600], "
                                   "positional with timeout [600, 60], or JSON "
                                   "{\"target_sim_time\": 600, \"timeout_seconds\": 60}. "
                                   "(Also accepted: target / time / sim_time as the target key, "
                                   "timeout / wall_timeout as the timeout key.)";
            return returnJson(out);
        }

        // Ensure hooks are installed (lazy first-use).
        if (!g_hooksInstallScanDone.load()) {
            std::string instErr;
            ensureAllHooksInstalled(instErr);
        }
        ensureRunStopEvent();

        // Already past target? Bail (AI should reset first).
        double now = 0.0;
        try {
            Variant nt = executestring("return time();", nullptr, nullptr, Variant());
            if (nt.type == VariantType::Number) now = double(nt);
        } catch (...) {}
        if (now >= target) {
            nlohmann::json out;
            out["ok"] = true;
            out["completed"] = false;
            out["target_sim_time"] = target;
            out["final_sim_time"] = now;
            out["run_state"] = "Stopped";
            out["reason"] = "already_past_target";
            out["note"] = "Current sim time is already at or past the target. Reset the model first.";
            return returnJson(out);
        }

        // Sweep any stale temp stops from a previous (crashed) run.
        sweepTempStopTimes();

        // Install our temp stop at the target.
        std::string addErr;
        std::string newStopPath = addTempStopTime(target, addErr);
        if (newStopPath.empty()) {
            nlohmann::json out;
            out["ok"] = false;
            out["error_code"] = "temp_stop_install_failed";
            out["error_message"] = addErr.empty() ? std::string("Could not add the temporary stop time.") : addErr;
            return returnJson(out);
        }

        // Reset the event + kick off the run.
        ResetEvent(g_runStopEvent);
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            g_lastRunState = "Running";
            g_lastSimTime  = now;
        }
        try { executestring("go(1);", nullptr, nullptr, Variant()); }
        catch (...) {
            sweepTempStopTimes();
            nlohmann::json out;
            out["ok"] = false;
            out["error_code"] = "go_failed";
            out["error_message"] = "go(1) threw — model may not be in a runnable state.";
            return returnJson(out);
        }

        // Wait for OnRunStop / OnModelReset hook to signal, or for our
        // real-world timeout to expire, or for the user to click Stop.
        //
        // Previously this was a single WaitForSingleObject(INFINITE) which
        // meant a runaway trigger inside the model would lock this thread
        // forever — viewer Stop button couldn't reach us, and the only
        // recovery was Task Manager → kill FlexSim. Now we poll in 100ms
        // ticks and check the agent's cancel flag between each, so a Stop
        // click unblocks within ~100ms.
        bool timedOut     = false;
        bool userStopped  = false;
        bool noMoreEvents = false;
        DWORD totalMs     = (timeoutSec < 0.0) ? 0
                                               : static_cast<DWORD>(timeoutSec * 1000.0);
        DWORD elapsedMs   = 0;
        const DWORD kTickMs = 100;
        ModelerAi::agent::Agent* a = ModelerAi::bootstrap::agent();
        // Resolve the events node once. The event queue is hung off
        // maintree at project/exec/events; nothing left in its subnodes
        // means the simulation has exhausted every scheduled event, so
        // sim time can't advance regardless of what target was asked for.
        TreeNode* eventsNode = nullptr;
        try { eventsNode = maintree()->find("project/exec/events"); } catch (...) {}
        while (true) {
            DWORD waitResult = WaitForSingleObject(g_runStopEvent, kTickMs);
            if (waitResult == WAIT_OBJECT_0) {
                break;  // model stopped (OnRunStop fired) — normal path
            }
            if (waitResult != WAIT_TIMEOUT) {
                // Wait failed (handle invalid?) — bail to avoid spinning.
                timedOut = true;
                break;
            }
            // Poll cancellation. cancelRequested() is set when the user
            // clicks Stop in the viewer — the bridge sets the flag from a
            // different thread than this one, and this loop notices.
            if (a && a->cancelRequested()) {
                userStopped = true;
                break;
            }
            // Empty event queue ⇒ nothing more to do. Skip waiting for an
            // OnRunStop that may not fire reliably on exhaustion.
            if (eventsNode) {
                int eventsLeft = 0;
                try { eventsLeft = (int)eventsNode->subnodes.length; } catch (...) {}
                if (eventsLeft == 0) {
                    noMoreEvents = true;
                    break;
                }
            }
            // Wall-clock budget.
            elapsedMs += kTickMs;
            if (timeoutSec >= 0.0 && elapsedMs >= totalMs) {
                timedOut = true;
                break;
            }
        }
        if (timedOut || userStopped || noMoreEvents) {
            // Stop the model so it doesn't keep churning in the background
            // after we return. Swallow any stop() failure — we're already
            // in a degraded path.
            try {
                executestring("updatestates(); stop(1); applicationcommand(\"switchRunning\", 0); return 0;",
                              nullptr, nullptr, Variant());
            } catch (...) {}
        }

        // Capture final state.
        std::string state;
        double finalTime = 0.0;
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            state     = g_lastRunState;
            finalTime = g_lastSimTime;
        }

        // Clean up our temp stop. Identify which stop fired BEFORE cleanup
        // (otherwise our entry could be the "match").
        bool reachedTarget = std::abs(finalTime - target) < 1e-3;
        FiredStopInfo fired;
        if (!reachedTarget) {
            fired = identifyFiredStop(finalTime);
            // If the fired one is our own marker, treat as target reached
            // (shouldn't happen with the epsilon check, but defensive).
            if (fired.dateString.rfind(kTempStopPrefix, 0) == 0) {
                reachedTarget = true;
                fired = FiredStopInfo{};
            }
        }
        sweepTempStopTimes();

        std::string reason;
        if (userStopped)          reason = "user_stopped";
        else if (noMoreEvents)    reason = "no_more_events";
        else if (timedOut)        reason = "wall_timeout";
        else if (reachedTarget)   reason = "target_reached";
        else if (fired.index > 0) reason = "earlier_stop_fired";
        else if (state == "Reset") reason = "reset_during_run";
        else reason = "stopped_early";

        nlohmann::json out;
        out["ok"] = true;
        out["completed"]     = reachedTarget;
        out["timed_out"]     = timedOut;
        out["user_stopped"]  = userStopped;
        out["no_more_events"] = noMoreEvents;
        out["target_sim_time"] = target;
        out["final_sim_time"] = finalTime;
        out["run_state"] = state;
        out["reason"] = reason;
        if (noMoreEvents) {
            out["events_note"] = "Event queue is empty — the simulation ran every "
                                 "scheduled event before sim time reached the target. "
                                 "Common causes: Source's arrival rate produced N items "
                                 "and stopped (e.g. ArrivalSchedule exhausted), every "
                                 "flowitem reached the Sink, or no Source is producing. "
                                 "Inspect via modelerai_get_model_summary.";
        }
        if (timedOut) {
            out["timeout_seconds"] = timeoutSec;
            out["timeout_note"]    = "Wall-clock timeout fired before the model "
                                     "reached its target. The model has been stopped. "
                                     "Common causes: deadlocked flow (everything's "
                                     "stuck), no Source generating items, a trigger "
                                     "body in an infinite loop, or the model is "
                                     "genuinely slower than the budget. Inspect via "
                                     "modelerai_get_model_summary.";
        }
        if (userStopped) {
            out["stop_note"] = "User clicked Stop in the viewer; the model has been "
                               "stopped at its current sim time. Any work the AI "
                               "did before this point is preserved.";
        }
        if (fired.index > 0) {
            nlohmann::json f;
            f["index"] = fired.index;
            f["model_time"] = fired.modelTime;
            f["date_string"] = fired.dateString;
            out["fired_stop"] = std::move(f);
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run_to_time", e.what()); }
      catch (...)                     { return returnException("run_to_time", "unknown"); }
}

// ============================================================================
// modelerai_run_to_end([timeout_seconds])
//   Drives go(1) and waits for the event queue to drain naturally. No temp
//   stop installed; the model runs until something stops it (FlexSim's own
//   stop times, OnRunStop, eventqty going to zero, the user clicking Stop,
//   the wall-clock budget, or the sim-time-progress watchdog firing).
//
//   See docs/superpowers/specs/2026-06-07-run-tools-design.md (Tool 2).
// ============================================================================
modelerai_export Variant ModelerAi_runToEnd(FLEXSIMINTERFACE)
{
    try {
        // Parse timeout — accepts bare number, [N], or { "timeout_seconds": N }.
        // No required args.
        double timeoutSec = 300.0;
        Variant arg = param(1);
        if (arg.type == VariantType::Number) {
            timeoutSec = double(arg);
        } else if (arg.type == VariantType::String) {
            std::string s = std::string(arg);
            size_t lead = s.find_first_not_of(" \t\r\n");
            char head = (lead == std::string::npos) ? '\0' : s[lead];
            if (head == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) {
                        for (const char* k : { "timeout_seconds", "timeout", "wall_timeout" }) {
                            if (j.contains(k) && j[k].is_number()) {
                                timeoutSec = j[k].get<double>();
                                break;
                            }
                        }
                    }
                } catch (...) {}
            }
        }

        if (!g_hooksInstallScanDone.load()) {
            std::string instErr;
            ensureAllHooksInstalled(instErr);
        }
        ensureRunStopEvent();

        // Reset the run-stop event, mark state, kick off the run.
        ResetEvent(g_runStopEvent);
        double now = currentSimTime();
        if (std::isnan(now)) now = 0.0;
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            g_lastRunState = "Running";
            g_lastSimTime  = now;
        }
        try { executestring("go(1);", nullptr, nullptr, Variant()); }
        catch (...) {
            return returnError("go_failed",
                "go(1) threw — model may not be in a runnable state. "
                "Try modelerai_reset_model and retry.");
        }

        // Polled wait loop. Exit on: OnRunStop signal, agent cancel, event
        // queue drained, wall timeout, sim-time-progress stall.
        bool userStopped  = false;
        bool timedOut     = false;
        bool noMoreEvents = false;
        bool stalled      = false;
        DWORD totalMs     = (timeoutSec < 0.0) ? 0
                                               : static_cast<DWORD>(timeoutSec * 1000.0);
        DWORD elapsedMs   = 0;
        const DWORD kTickMs = 100;
        ModelerAi::agent::Agent* a = ModelerAi::bootstrap::agent();
        SimProgressWatchdog watchdog(now);

        while (true) {
            DWORD waitResult = WaitForSingleObject(g_runStopEvent, kTickMs);
            if (waitResult == WAIT_OBJECT_0) break;  // OnRunStop fired
            if (waitResult != WAIT_TIMEOUT) { timedOut = true; break; }
            if (a && a->cancelRequested()) { userStopped = true; break; }
            if (currentEventQty() == 0)    { noMoreEvents = true; break; }
            if (watchdog.check())          { stalled = true; break; }
            elapsedMs += kTickMs;
            if (timeoutSec >= 0.0 && elapsedMs >= totalMs) { timedOut = true; break; }
        }

        // Recovery: any non-OnRunStop exit needs an explicit engine stop.
        if (userStopped || timedOut || noMoreEvents || stalled) {
            issueEngineStop();
        }

        // Capture final state.
        std::string state;
        double finalTime = 0.0;
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            state     = g_lastRunState;
            finalTime = g_lastSimTime;
        }

        // Identify which fired stop (if any) terminated the run early.
        FiredStopInfo fired;
        if (!userStopped && !timedOut && !noMoreEvents && !stalled) {
            fired = identifyFiredStop(finalTime);
        }

        std::string reason;
        if      (userStopped)       reason = exitReasonName(4);  // user_stopped
        else if (stalled)           reason = exitReasonName(6);  // stalled
        else if (noMoreEvents)      reason = exitReasonName(1);  // events_drained
        else if (timedOut)          reason = exitReasonName(5);  // wall_timeout
        else if (fired.index > 0)   reason = exitReasonName(7);  // earlier_stop_fired
        else if (state == "Reset")  reason = exitReasonName(9);  // reset_during_run
        else                        reason = exitReasonName(1);  // events_drained (OnRunStop signaled w/o other cause)

        nlohmann::json out;
        out["ok"]            = true;
        out["completed"]     = (reason == "events_drained");
        out["user_stopped"]  = userStopped;
        out["timed_out"]     = timedOut;
        out["stalled"]       = stalled;
        out["final_sim_time"]= finalTime;
        out["run_state"]     = state;
        out["reason"]        = reason;
        if (stalled) {
            out["stalled_note"] = "Sim time stopped advancing — the model likely has a "
                                  "runaway trigger body or a recursive event loop. Check "
                                  "the most recently-set triggers / FlexScript-valued "
                                  "properties (OnEntry / OnExit / SendToPort / ProcessTime). "
                                  "If FlexSim's UI itself is unresponsive, kill the process "
                                  "from Task Manager.";
        }
        if (timedOut) {
            out["timeout_seconds"] = timeoutSec;
            out["timeout_note"]    = "Wall-clock budget elapsed before the event queue "
                                     "drained. Either raise timeout_seconds or check the "
                                     "model — a Source that never stops producing will run "
                                     "forever in mode 2.";
        }
        if (userStopped) {
            out["stop_note"] = "User clicked Stop in the viewer.";
        }
        if (fired.index > 0) {
            nlohmann::json f;
            f["index"]       = fired.index;
            f["model_time"]  = fired.modelTime;
            f["date_string"] = fired.dateString;
            out["fired_stop"] = std::move(f);
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run_to_end", e.what()); }
      catch (...)                     { return returnException("run_to_end", "unknown"); }
}

// ============================================================================
// modelerai_run_until({ expression | pm + op + value }, [timeout_seconds])
//   Step-driven loop. After each step, evaluates a user-supplied condition;
//   exits when condition is truthy. Two condition shapes:
//
//     A) FlexScript expression: { "expression": "<expr>" }
//        Wrapped as `return (<expr>) ? 1 : 0;` and executestring'd. Soft-
//        fails per-step on eval errors (treats as false). Caller should
//        prefer simple expressions like Sink stats / label reads.
//
//     B) Structured PM: { "pm": "<name>", "op": "<op>", "value": <n> }
//        Resolves Model.performanceMeasures.<name> once at start; after
//        each step, evaluates the PM and compares to value via op.
//        op ∈ {">=", ">", "<=", "<", "==", "!="}.
//
//   step() ignores stop times — if the modeler has user-defined stop times
//   in the model, run_until will blow through them silently. Prefer
//   run_to_time when stop times matter.
//
//   See docs/superpowers/specs/2026-06-07-run-tools-design.md (Tool 3).
// ============================================================================
modelerai_export Variant ModelerAi_runUntil(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_condition",
                "modelerai_run_until expects a JSON object with either "
                "\"expression\" (FlexScript) or \"pm\" + \"op\" + \"value\" "
                "(structured PM check). Also accepts \"timeout_seconds\".");
        }

        std::string expression;
        std::string pmName;
        std::string opStr;
        double      threshold   = 0.0;
        double      timeoutSec  = 300.0;
        bool        haveExpr    = false;
        bool        haveStruct  = false;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) {
                return returnError("bad_args_shape",
                    "modelerai_run_until expects a JSON object.");
            }
            if (j.contains("expression") && j["expression"].is_string()) {
                expression = j["expression"].get<std::string>();
                haveExpr = !expression.empty();
            }
            if (j.contains("pm") && j["pm"].is_string()
                && j.contains("op") && j["op"].is_string()
                && j.contains("value") && j["value"].is_number())
            {
                pmName    = j["pm"].get<std::string>();
                opStr     = j["op"].get<std::string>();
                threshold = j["value"].get<double>();
                haveStruct = !pmName.empty() && !opStr.empty();
            }
            for (const char* k : { "timeout_seconds", "timeout", "wall_timeout" }) {
                if (j.contains(k) && j[k].is_number()) {
                    timeoutSec = j[k].get<double>();
                    break;
                }
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (!haveExpr && !haveStruct) {
            return returnError("missing_condition",
                "Supply either \"expression\" (FlexScript) or "
                "\"pm\" + \"op\" + \"value\" (structured PM check).");
        }
        if (haveExpr && haveStruct) {
            return returnError("bad_condition_shape",
                "Supply EITHER \"expression\" OR \"pm\" + \"op\" + \"value\", "
                "not both.");
        }
        if (haveStruct) {
            static const std::set<std::string> kOps = {
                ">=", ">", "<=", "<", "==", "!="
            };
            if (!kOps.count(opStr)) {
                return returnError("bad_operator",
                    "op must be one of >=, >, <=, <, ==, !=.");
            }
        }

        // For the structured shape, resolve the PM once. We evaluate via
        // its accessor each step rather than walking the tree.
        if (haveStruct) {
            std::string probe = "return Model.performanceMeasures."
                              + pmName + ".value;";
            try {
                Variant v = executestring(probe.c_str(), nullptr, nullptr, Variant());
                if (v.type != VariantType::Number) {
                    return returnError("pm_not_found",
                        "PerformanceMeasure '" + pmName + "' did not resolve "
                        "to a numeric value. Check the PM name with "
                        "modelerai_list_performance_measures.");
                }
            } catch (const std::exception& e) {
                return returnError("pm_not_found",
                    "Could not resolve PerformanceMeasure '" + pmName
                    + "': " + e.what());
            }
        }

        if (!g_hooksInstallScanDone.load()) {
            std::string instErr;
            ensureAllHooksInstalled(instErr);
        }

        double startSim = currentSimTime();
        if (std::isnan(startSim)) startSim = 0.0;
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            g_lastRunState = "Running";
            g_lastSimTime  = startSim;
        }

        // Build the per-step condition evaluator. Both shapes reduce to a
        // double; we compare against threshold (struct) or treat non-zero
        // as truthy (expression).
        std::string condScript;
        if (haveExpr) {
            condScript = "return (" + expression + ") ? 1 : 0;";
        } else {
            condScript = "return Model.performanceMeasures." + pmName + ".value;";
        }

        auto evalCondition = [&](double& outValue, bool& outFailed) -> bool {
            outFailed = false;
            try {
                Variant v = executestring(condScript.c_str(), nullptr, nullptr, Variant());
                if (v.type == VariantType::Number) {
                    outValue = double(v);
                    if (haveExpr) return outValue != 0.0;
                    // Structured: compare via op.
                    if      (opStr == ">=") return outValue >= threshold;
                    else if (opStr == ">")  return outValue >  threshold;
                    else if (opStr == "<=") return outValue <= threshold;
                    else if (opStr == "<")  return outValue <  threshold;
                    else if (opStr == "==") return outValue == threshold;
                    else if (opStr == "!=") return outValue != threshold;
                    return false;
                }
            } catch (...) {
                outFailed = true;
            }
            return false;
        };

        // Step loop. step() blocks while it fires one event; between steps
        // we check condition, eventqty, cancel, wall timeout, watchdog.
        bool conditionMet = false;
        bool userStopped  = false;
        bool timedOut     = false;
        bool noMoreEvents = false;
        bool stalled      = false;
        long long stepsTaken     = 0;
        long long evalAttempts   = 0;
        long long evalFailures   = 0;
        double    lastCondValue  = 0.0;
        DWORD     totalMs     = (timeoutSec < 0.0) ? 0
                                                   : static_cast<DWORD>(timeoutSec * 1000.0);
        ULONGLONG startWallMs = GetTickCount64();
        ModelerAi::agent::Agent* a = ModelerAi::bootstrap::agent();
        SimProgressWatchdog watchdog(startSim);

        // Check the condition once before any step — maybe it's already
        // true (e.g. the modeler asks "until sink has >= 0 items" or the
        // model is already in the target state).
        {
            double val = 0.0; bool failed = false;
            if (evalCondition(val, failed)) {
                conditionMet = true;
                lastCondValue = val;
            }
            evalAttempts++;
            if (failed) evalFailures++;
        }

        while (!conditionMet) {
            // Cheap pre-step checks first.
            if (a && a->cancelRequested()) { userStopped = true; break; }
            if (currentEventQty() == 0)    { noMoreEvents = true; break; }
            // Wall timeout BEFORE the step so we don't overshoot.
            ULONGLONG nowMs = GetTickCount64();
            if (timeoutSec >= 0.0 && (nowMs - startWallMs) >= totalMs) {
                timedOut = true;
                break;
            }
            if (watchdog.check()) { stalled = true; break; }

            // Fire one event.
            try {
                executestring("step();", nullptr, nullptr, Variant());
                stepsTaken++;
            } catch (...) {
                return returnError("step_failed",
                    "step() threw mid-loop. The model may have entered "
                    "an invalid state. Check the most recently-fired "
                    "trigger or event.");
            }

            // Re-check the condition.
            double val = 0.0; bool failed = false;
            evalAttempts++;
            if (evalCondition(val, failed)) {
                conditionMet = true;
                lastCondValue = val;
            } else {
                lastCondValue = val;
            }
            if (failed) evalFailures++;
        }

        // Capture final sim time before stopping the engine.
        double finalSim = currentSimTime();
        if (std::isnan(finalSim)) finalSim = watchdog.lastSimTime();

        // Recovery: explicit stop for any non-condition_met exit.
        if (!conditionMet) issueEngineStop();

        // If every single evaluation failed, the expression itself is bad.
        // Soft-fail per-step (false), but escalate on a 100% failure rate.
        if (haveExpr && evalAttempts > 0 && evalFailures == evalAttempts) {
            return returnError("expression_eval_failed",
                "FlexScript expression failed to evaluate on every step. "
                "Common causes: typo, missing variable, or referencing "
                "an object that doesn't exist. Expression: " + expression);
        }

        std::string reason;
        if      (conditionMet)  reason = exitReasonName(2);  // condition_met
        else if (userStopped)   reason = exitReasonName(4);  // user_stopped
        else if (stalled)       reason = exitReasonName(6);  // stalled
        else if (noMoreEvents)  reason = exitReasonName(3);  // events_drained_before_condition
        else if (timedOut)      reason = exitReasonName(5);  // wall_timeout
        else                    reason = "unknown";

        nlohmann::json out;
        out["ok"]                 = true;
        out["completed"]          = conditionMet;
        out["user_stopped"]       = userStopped;
        out["timed_out"]          = timedOut;
        out["stalled"]            = stalled;
        out["final_sim_time"]     = finalSim;
        out["steps_taken"]        = stepsTaken;
        out["final_condition_value"] = lastCondValue;
        out["reason"]             = reason;
        if (haveExpr && evalFailures > 0) {
            out["eval_failures"] = evalFailures;
            out["eval_attempts"] = evalAttempts;
        }
        if (stalled) {
            out["stalled_note"] = "Sim time stopped advancing during step()-loop. "
                                  "The condition may never become true with the "
                                  "current event flow, or a trigger body has gone "
                                  "into an infinite loop. Check the most recently-"
                                  "set triggers.";
        }
        if (timedOut) {
            out["timeout_seconds"] = timeoutSec;
            out["timeout_note"]    = "Wall-clock budget elapsed before condition was "
                                     "met. Final condition value: "
                                   + std::to_string(lastCondValue)
                                   + ". Either raise timeout_seconds or relax the condition.";
        }
        if (noMoreEvents) {
            out["events_note"] = "Event queue drained before the condition was met. "
                                 "Final condition value: " + std::to_string(lastCondValue)
                               + ". The model finished its work without reaching the target.";
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run_until", e.what()); }
      catch (...)                     { return returnException("run_until", "unknown"); }
}

// ============================================================================
// modelerai_run()
//   Async kickoff. Fires go(1) and returns immediately. AI can later call
//   modelerai_wait_for_stop() to block until the model stops for any reason.
// ============================================================================
modelerai_export Variant ModelerAi_run(FLEXSIMINTERFACE)
{
    try {
        if (!g_hooksInstallScanDone.load()) {
            std::string e; ensureAllHooksInstalled(e);
        }
        ensureRunStopEvent();
        ResetEvent(g_runStopEvent);
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            g_lastRunState = "Running";
        }
        executestring("go(1); applicationcommand(\"switchRunning\", 1); return 1;",
                      nullptr, nullptr, Variant());
        nlohmann::json out;
        out["ok"] = true;
        out["started"] = true;
        out["run_state"] = "Running";
        out["note"] = "Async — call modelerai_wait_for_stop() to block until the model stops.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run", e.what()); }
      catch (...)                     { return returnException("run", "unknown"); }
}

// ============================================================================
// modelerai_wait_for_stop(timeout_seconds?)
//   Blocks until the run-stop event signals (our OnRunStop hook fires) or
//   the timeout expires. Default timeout: infinite. Returns whether we saw
//   the stop and the final state.
// ============================================================================
modelerai_export Variant ModelerAi_waitForStop(FLEXSIMINTERFACE)
{
    try {
        double timeoutSec = numParam(param(1), -1.0);  // -1 → infinite
        ensureRunStopEvent();
        DWORD waitMs = (timeoutSec < 0.0) ? INFINITE
                                          : static_cast<DWORD>(timeoutSec * 1000.0);
        DWORD r = WaitForSingleObject(g_runStopEvent, waitMs);
        std::string state; double finalTime = 0.0;
        {
            std::lock_guard<std::mutex> lk(g_runStateMutex);
            state = g_lastRunState; finalTime = g_lastSimTime;
        }
        nlohmann::json out;
        out["ok"] = true;
        out["timed_out"] = (r == WAIT_TIMEOUT);
        out["run_state"] = state;
        out["sim_time"] = finalTime;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("wait_for_stop", e.what()); }
      catch (...)                     { return returnException("wait_for_stop", "unknown"); }
}

// ============================================================================
// modelerai_stop_model()
//   Stops a running sim. Mirrors the GUI's stop block: updatestates flushes
//   pending state changes, stop(1) halts, repaintall refreshes viewport.
// ============================================================================
modelerai_export Variant ModelerAi_stopModel(FLEXSIMINTERFACE)
{
    try {
        executestring("updatestates(); stop(1); repaintall(); applicationcommand(\"switchRunning\", 0); return time();",
                      nullptr, nullptr, Variant());
        double t = 0.0;
        try {
            Variant v = executestring("return time();", nullptr, nullptr, Variant());
            if (v.type == VariantType::Number) t = double(v);
        } catch (...) {}
        nlohmann::json out;
        out["ok"] = true;
        out["sim_time"] = t;
        out["run_state"] = "Stopped";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("stop_model", e.what()); }
      catch (...)                     { return returnException("stop_model", "unknown"); }
}

// ============================================================================
// modelerai_step_model()
//   Single-event step. Synchronous — returns when the event has been
//   processed.
// ============================================================================
modelerai_export Variant ModelerAi_stepModel(FLEXSIMINTERFACE)
{
    try {
        executestring("clearundohistory(NULL); step(); repaintall(); return time();",
                      nullptr, nullptr, Variant());
        double t = 0.0;
        try {
            Variant v = executestring("return time();", nullptr, nullptr, Variant());
            if (v.type == VariantType::Number) t = double(v);
        } catch (...) {}
        nlohmann::json out;
        out["ok"] = true;
        out["sim_time"] = t;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("step_model", e.what()); }
      catch (...)                     { return returnException("step_model", "unknown"); }
}

// ============================================================================
// modelerai_get_run_state()
//   Returns current run state ("Running" / "Stopped" / "Paused" / "Reset")
//   plus current sim time. Non-blocking — no event wait.
// ============================================================================
modelerai_export Variant ModelerAi_getRunState(FLEXSIMINTERFACE)
{
    try {
        // FlexSim's getrunstate returns an int; map common values.
        std::string state = "Unknown";
        double t = 0.0;
        try {
            Variant rs = executestring("return getrunstate();", nullptr, nullptr, Variant());
            if (rs.type == VariantType::Number) {
                int v = static_cast<int>(double(rs));
                switch (v) {
                    case 0: state = "Stopped"; break;
                    case 1: state = "Running"; break;
                    case 2: state = "Paused";  break;
                    default: state = "Unknown"; break;
                }
            }
            Variant tv = executestring("return time();", nullptr, nullptr, Variant());
            if (tv.type == VariantType::Number) t = double(tv);
        } catch (...) {}
        nlohmann::json out;
        out["ok"] = true;
        out["run_state"] = state;
        out["sim_time"] = t;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_run_state", e.what()); }
      catch (...)                     { return returnException("get_run_state", "unknown"); }
}

// ============================================================================
// modelerai_add_stop_time(seconds, enabled?, label?)
//   Persistent user-managed stop time. Distinct from the temp stops used
//   internally by run_to_time. enabled defaults to 1; label (optional) is
//   written into dateString for display.
// ============================================================================
modelerai_export Variant ModelerAi_addStopTime(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::Number) {
            nlohmann::json out;
            out["ok"] = false;
            out["error_code"] = "missing_seconds";
            out["error_message"] = "modelerai_add_stop_time(seconds, enabled?, label?) requires a numeric seconds arg.";
            return returnJson(out);
        }
        double seconds = double(arg);
        double enabled = numParam(param(2), 1.0);
        std::string label = strParam(param(3));

        std::string script;
        script += "treenode stopTimes = Model.find(\"Tools/ModelUnits/ModelDateTimes/stopTimes\");\n";
        script += "if (!stopTimes) { print(\"add_stop_time: stopTimes container missing\"); return 0; }\n";
        script += "treenode template = stopTimes.last;\n";
        script += "if (!template) { print(\"add_stop_time: no template SDT to copy\"); return 0; }\n";
        script += "treenode newStop = createcopy(template, stopTimes, 1);\n";
        script += "setsdtvalue(newStop, \"enabled\", "; script += std::to_string(enabled != 0.0 ? 1 : 0); script += ");\n";
        script += "function_s(newStop, \"setModelTime\", "; script += std::to_string(seconds); script += ");\n";
        if (!label.empty()) {
            script += "setsdtvalue(newStop, \"dateString\", \"";
            script += fsEscape(label);
            script += "\");\n";
        }
        script += "return newStop;\n";

        std::string path;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::TreeNode) {
                treenode n = static_cast<treenode>(v);
                if (n) {
                    try { path = std::string(nodetomodelpath(n, 1).c_str()); } catch (...) {}
                }
            }
        } catch (const std::exception& e) {
            return returnError("add_stop_time_failed", e.what());
        }

        if (path.empty()) {
            return returnError("add_stop_time_failed",
                "Could not add stop time — see console for cause.");
        }

        nlohmann::json out;
        out["ok"] = true;
        out["path"] = path;
        out["model_time"] = seconds;
        out["enabled"] = (enabled != 0.0);
        if (!label.empty()) out["label"] = label;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_stop_time", e.what()); }
      catch (...)                     { return returnException("add_stop_time", "unknown"); }
}

// ============================================================================
// modelerai_set_warmup_time(seconds, enabled?)
//   Sets the single warmupTime SDT. Different from stop times — there's only
//   ONE warmup entry under ModelDateTimes/warmupTime.
// ============================================================================
modelerai_export Variant ModelerAi_setWarmupTime(FLEXSIMINTERFACE)
{
    try {
        // Accept any of the shapes the AI is likely to try:
        //   - bare number:  60
        //   - 1-arg array:  [60]                    (already worked positionally)
        //   - 2-arg array:  [60, 1]
        //   - JSON object:  "{\"seconds\": 60}"     (with optional "enabled")
        //
        // The export from 2026-06-07 showed the AI burning 4 tool calls
        // probing for the right shape because the previous error message
        // ("requires a numeric seconds arg") didn't say HOW. Accept all
        // three; the error message now lists what was accepted.
        Variant arg     = param(1);
        Variant arg2    = param(2);
        double  seconds = 0.0;
        double  enabled = 1.0;
        bool    haveSeconds = false;
        bool    haveEnabled = false;

        if (arg.type == VariantType::Number) {
            seconds = double(arg);
            haveSeconds = true;
            if (arg2.type == VariantType::Number) {
                enabled = double(arg2);
                haveEnabled = true;
            }
        } else if (arg.type == VariantType::String) {
            std::string s = std::string(arg);
            // Trim leading whitespace for the shape sniff.
            size_t lead = s.find_first_not_of(" \t\r\n");
            char head = (lead == std::string::npos) ? '\0' : s[lead];
            if (head == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object() && j.contains("seconds")
                        && j["seconds"].is_number())
                    {
                        seconds = j["seconds"].get<double>();
                        haveSeconds = true;
                    }
                    if (j.is_object() && j.contains("enabled")) {
                        if (j["enabled"].is_boolean()) {
                            enabled = j["enabled"].get<bool>() ? 1.0 : 0.0;
                            haveEnabled = true;
                        } else if (j["enabled"].is_number()) {
                            enabled = j["enabled"].get<double>();
                            haveEnabled = true;
                        }
                    }
                } catch (...) {
                    // Fall through to the missing_seconds error below.
                }
            }
        }

        if (!haveSeconds) {
            return returnError("missing_seconds",
                "modelerai_set_warmup_time expects a number of seconds. "
                "Any of these shapes work: bare number 60, positional [60], "
                "positional with enabled [60, 1], or JSON {\"seconds\": 60, "
                "\"enabled\": true}.");
        }
        (void)haveEnabled;  // default is enabled=1 if not supplied

        std::string script;
        script += "treenode wu = Model.find(\"Tools/ModelUnits/ModelDateTimes/warmupTime\");\n";
        script += "if (!wu) { print(\"set_warmup_time: warmupTime node missing\"); return 0; }\n";
        script += "setsdtvalue(wu, \"enabled\", "; script += std::to_string(enabled != 0.0 ? 1 : 0); script += ");\n";
        script += "function_s(wu, \"setModelTime\", "; script += std::to_string(seconds); script += ");\n";
        script += "return wu;\n";

        try {
            executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("set_warmup_failed", e.what());
        }

        nlohmann::json out;
        out["ok"] = true;
        out["model_time"] = seconds;
        out["enabled"] = (enabled != 0.0);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_warmup_time", e.what()); }
      catch (...)                     { return returnException("set_warmup_time", "unknown"); }
}

// ============================================================================
// modelerai_set_run_speed(speed)
//   Sets the playback speed at MAIN:/project/exec/step. Higher = faster
//   visualization. Clamped to FlexSim's documented bounds.
// ============================================================================
modelerai_export Variant ModelerAi_setRunSpeed(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::Number) {
            return returnError("missing_speed",
                "modelerai_set_run_speed(speed) requires a numeric speed arg.");
        }
        double speed = double(arg);
        if (speed < 0.1)       speed = 0.1;
        if (speed > 1.0e9)     speed = 1.0e9;

        std::string script;
        script += "treenode s = node(\"MAIN:/project/exec/step\");\n";
        script += "if (!s) return 0;\n";
        script += "set(s, "; script += std::to_string(speed); script += ");\n";
        script += "applylinks(node(\"VIEW:/1/1\"), 1);\n";
        script += "return get(s);\n";

        double actual = speed;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Number) actual = double(v);
        } catch (const std::exception& e) {
            return returnError("set_run_speed_failed", e.what());
        }

        nlohmann::json out;
        out["ok"] = true;
        out["run_speed"] = actual;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_run_speed", e.what()); }
      catch (...)                     { return returnException("set_run_speed", "unknown"); }
}

// ============================================================================
// OUTPUT DOMAIN — performance measures + object stats + model summary
// ============================================================================
//
// PerformanceMeasures live as rows inside PerformanceMeasureTables
// (Tools.create("PerformanceMeasureTable") makes the container;
// function_s(table, "addPfm") adds a row). Each row has:
//   Name (string)
//   Value/
//     valueNode  — S-flagged script subnode (FlexScript code)
//     reference  — treenode pointer the script can use as param(1)
//     extraData  — free-form, unused by us
//   Display Units (string)
//   Description (string)
//
// Reading a PM: Model.performanceMeasures.<Name> evaluates the valueNode
// script with reference bound to param(1) and returns whatever the script
// returns. We marshal that Variant to JSON when reporting back.

// ============================================================================
// modelerai_create_performance_measure(json)
//   json: { name, expression, reference?, units?, description?, table_name?, replace? }
//   Auto-table fallback: try table_name, else first existing
//   PerformanceMeasureTable, else create a new one named "PerformanceMeasures".
// ============================================================================
modelerai_export Variant ModelerAi_createPerformanceMeasure(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_performance_measure expects a single JSON-encoded string arg.");
        }
        std::string argsJson = std::string(arg);

        std::string tableName, name, expression, referencePath, units, description;
        bool replace = false;
        try {
            auto j = nlohmann::json::parse(argsJson);
            if (!j.contains("name") || !j["name"].is_string()
                || !j.contains("expression") || !j["expression"].is_string()) {
                return returnError("missing_field",
                    "create_performance_measure requires `name` and `expression`.");
            }
            name          = j["name"].get<std::string>();
            expression    = j["expression"].get<std::string>();
            tableName     = j.value("table_name", "");
            referencePath = j.value("reference",   "");
            units         = j.value("units",       "");
            description   = j.value("description", "");
            replace       = j.value("replace",     false);
        } catch (const std::exception& e) {
            return returnError("bad_args_json",
                std::string("Couldn't parse args JSON: ") + e.what());
        }

        if (name.empty() || expression.empty()) {
            return returnError("empty_field", "`name` and `expression` must be non-empty.");
        }

        // Auto-prepend the standard PM param declarations if the expression
        // uses `reference` / `extraData` / `repData` without declaring them.
        // The AI consistently forgets these — FlexSim's PM trigger binds the
        // params as param(1)/(2)/(3) but they're not auto-named in the body.
        auto stringContains = [](const std::string& haystack, const std::string& needle) {
            return haystack.find(needle) != std::string::npos;
        };
        std::string prepend;
        if (stringContains(expression, "reference")
            && !stringContains(expression, "treenode reference")
            && !stringContains(expression, "Object reference")) {
            prepend += "treenode reference = param(1);\n";
        }
        if (stringContains(expression, "extraData")
            && !stringContains(expression, "treenode extraData")) {
            prepend += "treenode extraData = param(2);\n";
        }
        if (stringContains(expression, "repData")
            && !stringContains(expression, "treenode repData")) {
            prepend += "treenode repData = param(3);\n";
        }
        const std::string finalExpression = prepend + expression;

        // Antipattern scan on the PM body — same enforcement as run_script.
        // Catches `getoutput(reference)` (deprecated form), legacy `xloc()`,
        // etc. so the PM doesn't get saved with a body that won't work.
        {
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(finalExpression, anName, anRem)) {
                return returnError("deprecated_api_in_pm_body", anRem);
            }
        }

        // Build the FlexScript that locates / creates the table, allocates a
        // row via addPfm, and writes Name/Value/Description.
        std::string script;
        script.reserve(1024 + expression.size());

        // Table lookup with the same auto-fallback as add_parameter.
        script += "treenode tbl = 0;\n";
        if (!tableName.empty()) {
            script += "tbl = Tools.get(\"PerformanceMeasureTable\", \""; script += fsEscape(tableName); script += "\");\n";
        }
        script += "if (!tbl) {\n";
        script += "    treenode container = Model.find(\"Tools/PerformanceMeasureTables\");\n";
        script += "    if (container && container.subnodes.length > 0) tbl = container.subnodes[1];\n";
        script += "}\n";
        script += "if (!tbl) {\n";
        script += "    tbl = Tools.create(\"PerformanceMeasureTable\", \"\");\n";
        script += "    tbl.name = \""; script += fsEscape(tableName.empty() ? std::string("PerformanceMeasures") : tableName); script += "\";\n";
        script += "}\n";
        script += "if (!tbl) { print(\"create_pm: could not find or create a PerformanceMeasureTable\"); return 0; }\n";

        // Cross-table uniqueness check — like ParameterTable, the global
        // accessor Model.performanceMeasures[Name] doesn't take a table arg.
        script += "treenode allTables = Model.find(\"Tools/PerformanceMeasureTables\");\n";
        script += "treenode crossTable = 0;\n";
        script += "if (allTables) {\n";
        script += "    for (int iT = 1; iT <= allTables.subnodes.length; iT++) {\n";
        script += "        treenode t = allTables.subnodes[iT];\n";
        script += "        if (t == tbl) continue;\n";
        script += "        treenode pms = t.find(\"variables\");\n";
        script += "        if (!pms) continue;\n";
        script += "        pms = pms.find(\"performanceMeasures\");\n";
        script += "        if (!pms) continue;\n";
        script += "        for (int iP = 1; iP <= pms.subnodes.length; iP++) {\n";
        script += "            treenode pm = pms.subnodes[iP];\n";
        script += "            treenode nm = pm.find(\"Name\");\n";
        script += "            if (nm && nm.value == \""; script += fsEscape(name); script += "\") {\n";
        script += "                crossTable = t; break;\n";
        script += "            }\n";
        script += "        }\n";
        script += "        if (crossTable) break;\n";
        script += "    }\n";
        script += "}\n";
        script += "if (crossTable) { print(\"create_pm: name '"; script += fsEscape(name);
        script += "' already exists in another PM table: \" + crossTable.name + \". Names must be unique.\"); return 0; }\n";

        // Same-table duplicate handling.
        script += "treenode pset = tbl.find(\"variables\");\n";
        script += "if (!pset) pset = tbl.subnodes.assert(\"variables\");\n";
        script += "treenode pms = pset.find(\"performanceMeasures\");\n";
        script += "if (!pms) pms = pset.subnodes.assert(\"performanceMeasures\");\n";
        script += "treenode existing = 0;\n";
        script += "for (int iX = 1; iX <= pms.subnodes.length; iX++) {\n";
        script += "    treenode p = pms.subnodes[iX];\n";
        script += "    treenode nm = p.find(\"Name\");\n";
        script += "    if (nm && nm.value == \""; script += fsEscape(name); script += "\") { existing = p; break; }\n";
        script += "}\n";
        if (replace) {
            script += "if (existing) existing.destroy();\n";
        } else {
            script += "if (existing) { print(\"create_pm: duplicate name in same table (use replace=true): "; script += fsEscape(name); script += "\"); return 0; }\n";
        }

        // Allocate the row via addPfm.
        script += "treenode pm = function_s(tbl, \"addPfm\");\n";
        script += "if (!pm) { print(\"create_pm: addPfm returned null\"); return 0; }\n";
        script += "pm.subnodes.assert(\"Name\").value = \""; script += fsEscape(name); script += "\";\n";

        // Set the valueNode script + reference + description + display units.
        // valueNode is S-flagged — must enablecode + buildnodeflexscript.
        script += "treenode v  = pm.subnodes.assert(\"Value\");\n";
        script += "treenode vn = v.subnodes.assert(\"valueNode\");\n";
        script += "vn.value = \""; script += fsEscape(finalExpression); script += "\";\n";
        script += "enablecode(vn);\n";
        script += "buildnodeflexscript(vn);\n";
        if (!referencePath.empty()) {
            script += "treenode refn = Model.find(\""; script += fsEscape(referencePath); script += "\");\n";
            script += "if (refn) v.subnodes.assert(\"reference\").value = refn;\n";
            script += "else print(\"create_pm: reference_path not found (PM still created without reference): "; script += fsEscape(referencePath); script += "\");\n";
        }
        if (!description.empty()) {
            script += "pm.subnodes.assert(\"Description\").value = \""; script += fsEscape(description); script += "\";\n";
        }
        if (!units.empty()) {
            script += "pm.subnodes.assert(\"Display Units\").value = \""; script += fsEscape(units); script += "\";\n";
        }
        script += "return pm;\n";

        std::string newPath, owningTable;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type != VariantType::TreeNode) {
                return returnError("create_pm_failed",
                    "Registration script did not return a treenode (see console).");
            }
            treenode created = static_cast<treenode>(v);
            if (!created) {
                return returnError("create_pm_failed", "Registration script returned null.");
            }
            try { newPath = std::string(nodetomodelpath(created, 1).c_str()); } catch (...) {}
            // Walk up to find the owning table name (subnodes -> performanceMeasures -> variables -> table).
            try {
                if (created->up) {
                    treenode pmsNode = created->up;        // performanceMeasures
                    if (pmsNode->up) {
                        treenode varsNode = pmsNode->up;   // variables
                        if (varsNode->up) {
                            treenode tableNode = varsNode->up;
                            owningTable = std::string(getname(tableNode));
                        }
                    }
                }
            } catch (...) {}
        } catch (const std::exception& e) {
            return returnError("create_pm_failed", e.what());
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["name"]     = name;
        out["table"]    = owningTable;
        out["path"]     = newPath;
        out["accessor"] = "Model.performanceMeasures." + name;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_performance_measure", e.what()); }
      catch (...)                     { return returnException("create_performance_measure", "unknown"); }
}

// ============================================================================
// modelerai_delete_performance_measure(name)
//   Searches every PerformanceMeasureTable, destroys the matching PM row.
// ============================================================================
modelerai_export Variant ModelerAi_deletePerformanceMeasure(FLEXSIMINTERFACE)
{
    try {
        std::string name = strParam(param(1));
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_delete_performance_measure(name) requires a name.");
        }

        std::string script;
        script += "treenode allTables = Model.find(\"Tools/PerformanceMeasureTables\");\n";
        script += "if (!allTables) return 0;\n";
        script += "for (int iT = 1; iT <= allTables.subnodes.length; iT++) {\n";
        script += "    treenode t = allTables.subnodes[iT];\n";
        script += "    treenode pms = t.find(\"variables\");\n";
        script += "    if (!pms) continue;\n";
        script += "    pms = pms.find(\"performanceMeasures\");\n";
        script += "    if (!pms) continue;\n";
        script += "    for (int iP = 1; iP <= pms.subnodes.length; iP++) {\n";
        script += "        treenode pm = pms.subnodes[iP];\n";
        script += "        treenode nm = pm.find(\"Name\");\n";
        script += "        if (nm && nm.value == \""; script += fsEscape(name); script += "\") { pm.destroy(); return 1; }\n";
        script += "    }\n";
        script += "}\n";
        script += "return 0;\n";

        bool removed = false;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Number) removed = (double(v) > 0);
        } catch (const std::exception& e) {
            return returnError("delete_pm_failed", e.what());
        }

        nlohmann::json out;
        out["ok"]      = removed;
        out["name"]    = name;
        out["removed"] = removed;
        if (!removed) out["note"] = "No PM with that name found in any table.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("delete_performance_measure", e.what()); }
      catch (...)                     { return returnException("delete_performance_measure", "unknown"); }
}

// ============================================================================
// modelerai_list_performance_measures()
//   Enumerates every PM across every table. Returns metadata only (name,
//   table, description, units, reference path if set). Does NOT evaluate
//   the expressions — fast, suitable for "what's defined" introspection.
// ============================================================================
modelerai_export Variant ModelerAi_listPerformanceMeasures(FLEXSIMINTERFACE)
{
    try {
        // We'll evaluate per-table in C++ rather than have FlexScript build a
        // big nested Map — easier marshalling.
        std::string script;
        script += "treenode container = Model.find(\"Tools/PerformanceMeasureTables\");\n";
        script += "if (!container) return Array();\n";
        script += "Array out = Array();\n";
        script += "for (int iT = 1; iT <= container.subnodes.length; iT++) {\n";
        script += "    treenode t = container.subnodes[iT];\n";
        script += "    treenode pms = t.find(\"variables\");\n";
        script += "    if (!pms) continue;\n";
        script += "    pms = pms.find(\"performanceMeasures\");\n";
        script += "    if (!pms) continue;\n";
        script += "    for (int iP = 1; iP <= pms.subnodes.length; iP++) {\n";
        script += "        treenode pm = pms.subnodes[iP];\n";
        script += "        Map row = Map();\n";
        script += "        row[\"table\"] = string(t.name);\n";
        script += "        treenode nm = pm.find(\"Name\");\n";
        script += "        row[\"name\"]  = nm ? string(nm.value) : \"\";\n";
        script += "        treenode ds = pm.find(\"Description\");\n";
        script += "        if (ds) row[\"description\"] = string(ds.value);\n";
        script += "        treenode du = pm.find(\"Display Units\");\n";
        script += "        if (du) row[\"units\"] = string(du.value);\n";
        script += "        treenode v  = pm.find(\"Value\");\n";
        script += "        if (v) {\n";
        script += "            treenode r = v.find(\"reference\");\n";
        script += "            if (r && objectexists(r.value)) row[\"reference_path\"] = string(nodetomodelpath(r.value, 1));\n";
        script += "        }\n";
        script += "        out[out.length+1] = row;\n";
        script += "    }\n";
        script += "}\n";
        script += "return out;\n";

        nlohmann::json results = nlohmann::json::array();
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Array) {
                Array a = static_cast<Array>(v);
                for (int i = 1; i <= a.length; ++i) {
                    Variant rowV = a[i];
                    if (rowV.type != VariantType::Map) continue;
                    Map m = static_cast<Map>(rowV);
                    nlohmann::json row;
                    try { row["table"] = std::string(m["table"]); } catch (...) {}
                    try { row["name"]  = std::string(m["name"]);  } catch (...) {}
                    try { Variant d = m["description"]; if (d.type == VariantType::String) row["description"] = std::string(d); } catch (...) {}
                    try { Variant u = m["units"];       if (u.type == VariantType::String) row["units"]       = std::string(u); } catch (...) {}
                    try { Variant r = m["reference_path"]; if (r.type == VariantType::String) row["reference_path"] = std::string(r); } catch (...) {}
                    results.push_back(std::move(row));
                }
            }
        } catch (const std::exception& e) {
            return returnError("list_pm_failed", e.what());
        }

        nlohmann::json out;
        out["ok"]    = true;
        out["count"] = static_cast<int>(results.size());
        out["performance_measures"] = std::move(results);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_performance_measures", e.what()); }
      catch (...)                     { return returnException("list_performance_measures", "unknown"); }
}

// Internal helper: marshal a Variant returned by a PM evaluation into JSON.
nlohmann::json pmValueToJson(const Variant& v)
{
    switch (v.type) {
        case VariantType::Null:    return nullptr;
        case VariantType::Number: {
            double d = static_cast<double>(v);
            if (d == static_cast<double>(static_cast<long long>(d))
                && d >= -9.2e18 && d <= 9.2e18) return static_cast<long long>(d);
            return d;
        }
        case VariantType::String:  return std::string(v);
        case VariantType::TreeNode: {
            treenode n = static_cast<treenode>(v);
            nlohmann::json out;
            if (!n) { out["null"] = true; return out; }
            out["treenode_name"] = std::string(getname(n));
            try {
                std::string p(nodetomodelpath(n, 1).c_str());
                if (!p.empty()) out["treenode_path"] = p;
            } catch (...) {}
            return out;
        }
        case VariantType::Array: {
            nlohmann::json arr = nlohmann::json::array();
            Array a = static_cast<Array>(v);
            for (int i = 1; i <= a.length; ++i) arr.push_back(pmValueToJson(a[i]));
            return arr;
        }
        case VariantType::Map: {
            nlohmann::json obj = nlohmann::json::object();
            try {
                Map m = static_cast<Map>(v);
                Array keys = m.keys;
                for (int i = 1; i <= keys.length; ++i) {
                    std::string k = std::string(keys[i]);
                    obj[k] = pmValueToJson(m[keys[i]]);
                }
            } catch (...) {}
            return obj;
        }
        default: return nullptr;
    }
}

// ============================================================================
// modelerai_get_performance_measure(name)
//   Evaluates a single PM via Model.performanceMeasures[<name>] and returns
//   the value. Marshalled through pmValueToJson — supports number, string,
//   treenode (as path), array, map.
// ============================================================================
modelerai_export Variant ModelerAi_getPerformanceMeasure(FLEXSIMINTERFACE)
{
    try {
        std::string name = strParam(param(1));
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_get_performance_measure(name) requires a name.");
        }
        std::string script = "return Model.performanceMeasures[\"" + fsEscape(name) + "\"];";
        nlohmann::json value;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            value = pmValueToJson(v);
        } catch (const std::exception& e) {
            return returnError("eval_failed", std::string("Evaluating PM `") + name + "` threw: " + e.what());
        }

        nlohmann::json out;
        out["ok"]    = true;
        out["name"]  = name;
        out["value"] = std::move(value);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_performance_measure", e.what()); }
      catch (...)                     { return returnException("get_performance_measure", "unknown"); }
}

// ============================================================================
// modelerai_get_performance_measures()
//   Lists every PM (via list_performance_measures logic), evaluates each,
//   returns { name → value } map. The structured KPI snapshot the AI reads
//   after a run.
// ============================================================================
modelerai_export Variant ModelerAi_getPerformanceMeasures(FLEXSIMINTERFACE)
{
    try {
        // First enumerate names. We evaluate one-at-a-time in C++ so a single
        // bad PM script doesn't kill the whole snapshot.
        std::string listScript;
        listScript += "treenode container = Model.find(\"Tools/PerformanceMeasureTables\");\n";
        listScript += "if (!container) return Array();\n";
        listScript += "Array out = Array();\n";
        listScript += "for (int iT = 1; iT <= container.subnodes.length; iT++) {\n";
        listScript += "    treenode t = container.subnodes[iT];\n";
        listScript += "    treenode pms = t.find(\"variables\");\n";
        listScript += "    if (!pms) continue;\n";
        listScript += "    pms = pms.find(\"performanceMeasures\");\n";
        listScript += "    if (!pms) continue;\n";
        listScript += "    for (int iP = 1; iP <= pms.subnodes.length; iP++) {\n";
        listScript += "        treenode pm = pms.subnodes[iP];\n";
        listScript += "        treenode nm = pm.find(\"Name\");\n";
        listScript += "        if (nm && string(nm.value) != \"\") out[out.length+1] = string(nm.value);\n";
        listScript += "    }\n";
        listScript += "}\n";
        listScript += "return out;\n";

        std::vector<std::string> names;
        try {
            Variant v = executestring(listScript.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Array) {
                Array a = static_cast<Array>(v);
                for (int i = 1; i <= a.length; ++i) {
                    Variant nv = a[i];
                    if (nv.type == VariantType::String) names.push_back(std::string(nv));
                }
            }
        } catch (const std::exception& e) {
            return returnError("list_failed", e.what());
        }

        nlohmann::json values = nlohmann::json::object();
        nlohmann::json errors = nlohmann::json::object();
        for (const auto& n : names) {
            std::string script = "return Model.performanceMeasures[\"" + fsEscape(n) + "\"];";
            try {
                Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
                values[n] = pmValueToJson(v);
            } catch (const std::exception& e) {
                errors[n] = e.what();
            } catch (...) {
                errors[n] = "unknown exception";
            }
        }

        nlohmann::json out;
        out["ok"]                   = true;
        out["count"]                = static_cast<int>(names.size());
        out["performance_measures"] = std::move(values);
        if (!errors.empty()) out["errors"] = std::move(errors);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_performance_measures", e.what()); }
      catch (...)                     { return returnException("get_performance_measures", "unknown"); }
}

// ============================================================================
// modelerai_get_object_stats(object_name, verbose?)
//   Returns the standard stats for any 3D object: input/output counts,
//   content (current/min/max/avg), state-time breakdown (busy/idle/etc.),
//   throughput rate. Curated subset by default; verbose=true returns the
//   full stats tree.
// ============================================================================
modelerai_export Variant ModelerAi_getObjectStats(FLEXSIMINTERFACE)
{
    try {
        std::string objName = strParam(param(1));
        double verbose      = numParam(param(2), 0.0);
        (void)verbose;  // reserved for future expansion
        if (objName.empty()) {
            return returnError("missing_object_name",
                "modelerai_get_object_stats(object_name) requires a name.");
        }

        // Uses the same accessors as FlexSim's PerformanceMeasure picklists
        // (Examples/Performacne measure picklists.fsx): getoutput/getinput
        // for counts, getstat(obj, "Content"/"Staytime", STAT_MIN/MAX/AVERAGE)
        // for content + staytime, getvarnum(obj, "totaltraveldist") for
        // movers, and defaultstatelist walking for state times.
        //
        // Percentage calcs use Model.statisticalTime (excludes warmup) NOT
        // time(). For throughput rate we use time() since that's the wall
        // sim time the user cares about.
        std::string script;
        script += "treenode o = Model.find(\""; script += fsEscape(objName); script += "\");\n";
        script += "if (!objectexists(o)) { print(\"get_object_stats: object not found: "; script += fsEscape(objName); script += "\"); return 0; }\n";
        script += "Object obj = o;\n";
        script += "Map out = Map();\n";
        script += "out[\"object_name\"] = \""; script += fsEscape(objName); script += "\";\n";
        script += "treenode cls = classobject(o);\n";
        script += "if (cls) out[\"class_name\"] = string(getname(cls));\n";
        // Throughput counts via the modern accessor form. Sinks RECEIVE
        // items, so for Sink "throughput" the meaningful number is
        // input_count; for Source / Processor / Queue it's output_count.
        // We return both so the AI can pick the right one for the class.
        script += "try {\n";
        script += "    out[\"input_count\"]  = obj.stats.input.value;\n";
        script += "    out[\"output_count\"] = obj.stats.output.value;\n";
        script += "} catch (...) {}\n";
        // Content stats via getstat (matches GUI picklist).
        script += "try {\n";
        script += "    out[\"content_current\"] = obj.stats.content.value;\n";
        script += "    out[\"content_min\"]     = getstat(obj, \"Content\", STAT_MIN);\n";
        script += "    out[\"content_max\"]     = getstat(obj, \"Content\", STAT_MAX);\n";
        script += "    out[\"content_avg\"]     = getstat(obj, \"Content\", STAT_AVERAGE);\n";
        script += "} catch (...) {}\n";
        // Throughput per hour. We can't know if this object is a Sink
        // without doing class detection, so we compute "released per hour"
        // from output count. AI uses class_name to interpret: Sink reads
        // input_count for throughput, Source/Queue/Processor read
        // throughput_per_hour.
        script += "try {\n";
        script += "    double simHours = time() / 3600.0;\n";
        script += "    if (simHours > 0.0) out[\"throughput_per_hour\"] = obj.stats.output.value / simHours;\n";
        script += "} catch (...) {}\n";
        // Staytime via getstat — Min/Max/Average.
        script += "try {\n";
        script += "    out[\"staytime_min\"]  = getstat(obj, \"Staytime\", STAT_MIN);\n";
        script += "    out[\"staytime_max\"]  = getstat(obj, \"Staytime\", STAT_MAX);\n";
        script += "    out[\"staytime_mean\"] = getstat(obj, \"Staytime\", STAT_AVERAGE);\n";
        script += "} catch (...) {}\n";
        // Total travel distance — present on TaskExecuters / movers.
        script += "try {\n";
        script += "    out[\"total_travel_distance\"] = getvarnum(obj, \"totaltraveldist\");\n";
        script += "} catch (...) {}\n";
        // State times: walk defaultstatelist to get every state name, query
        // the time-at-value for each, compute percentage of statisticalTime.
        // Skipped if statisticalTime == 0 (model never ran or warmup not finished).
        script += "try {\n";
        script += "    treenode dsl = node(\"MAIN:/project/exec/globals/defaultstatelist\");\n";
        script += "    if (dsl && Model.statisticalTime > 0) {\n";
        script += "        Map sm = Map();\n";
        script += "        for (int i = 1; i <= content(dsl); i++) {\n";
        script += "            string sn = string(getnodename(rank(dsl, i)));\n";
        script += "            if (sn == \"\") continue;\n";
        script += "            double t = getstat(obj, \"State\", STAT_TIME_AT_VALUE, 0, sn);\n";
        script += "            if (t > 0) sm[sn] = 100.0 * t / Model.statisticalTime;\n";
        script += "        }\n";
        script += "        out[\"state_percent\"] = sm;\n";
        script += "    }\n";
        script += "} catch (...) {}\n";
        script += "out[\"sim_time\"]         = time();\n";
        script += "out[\"statistical_time\"] = Model.statisticalTime;\n";
        script += "return out;\n";

        nlohmann::json result;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Map) {
                result = pmValueToJson(v);  // same marshaller works for Maps
            } else {
                return returnError("object_not_found",
                    "Object '" + objName + "' did not resolve.");
            }
        } catch (const std::exception& e) {
            return returnError("get_stats_failed", e.what());
        }

        nlohmann::json out;
        out["ok"]    = true;
        out["stats"] = std::move(result);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_object_stats", e.what()); }
      catch (...)                     { return returnException("get_object_stats", "unknown"); }
}

// ============================================================================
// modelerai_get_model_summary()
//   High-level rollup: sim time, totals across Sources/Sinks, per-class
//   object counts, stuck-objects check. The "did the model finish cleanly"
//   sanity report.
// ============================================================================
modelerai_export Variant ModelerAi_getModelSummary(FLEXSIMINTERFACE)
{
    try {
        std::string script;
        script += "Map out = Map();\n";
        script += "out[\"sim_time\"] = time();\n";
        // Run state.
        script += "int rs = getrunstate();\n";
        script += "out[\"run_state\"] = rs == 0 ? \"Stopped\" : (rs == 1 ? \"Running\" : (rs == 2 ? \"Paused\" : \"Unknown\"));\n";
        // Walk model children, group by class.
        script += "Map classCounts = Map();\n";
        script += "double totalCreated = 0;\n";
        script += "double totalSunk    = 0;\n";
        script += "Array stuckObjects = Array();\n";
        script += "treenode root = model();\n";
        // FlexScript has NO try/catch — wrapping the loop body in one
        // makes the whole script fail to compile, executestring returns
        // 0, and the caller sees "Summary script did not return a Map."
        // (which was the actual failure surfacing as summary_failed in
        // the trace). Use defensive checks instead.
        script += "for (int i = 1; i <= root.subnodes.length; i++) {\n";
        script += "    treenode o = root.subnodes[i];\n";
        script += "    if (!o) continue;\n";
        script += "    treenode cls = classobject(o);\n";
        script += "    if (!cls) continue;\n";
        script += "    string cn = string(getname(cls));\n";
        script += "    if (cn.length == 0) continue;\n";
        // Map[missing] yields null on newer FlexScript; double-cast it to 0
        // first so the +1 increment never sees a null arithmetic operand.
        script += "    double cur = classCounts[cn];\n";
        script += "    classCounts[cn] = cur + 1;\n";
        // Totals + stuck-objects check, gated on class name so we only
        // touch .stats.* on classes that actually expose them.
        script += "    if (cn == \"Source\") {\n";
        script += "        Object oo = o;\n";
        script += "        totalCreated = totalCreated + oo.stats.output.value;\n";
        script += "    }\n";
        script += "    if (cn == \"Sink\") {\n";
        script += "        Object oo = o;\n";
        script += "        totalSunk = totalSunk + oo.stats.input.value;\n";
        script += "    }\n";
        script += "    if (rs == 0 && (cn == \"Queue\" || cn == \"Processor\" || cn == \"Combiner\" || cn == \"Separator\" || cn == \"BasicFR\")) {\n";
        script += "        Object oo = o;\n";
        script += "        double cnt = oo.stats.content.value;\n";
        script += "        if (cnt > 0) {\n";
        script += "            Map s = Map();\n";
        script += "            s[\"name\"]    = string(getname(o));\n";
        script += "            s[\"class\"]   = cn;\n";
        script += "            s[\"content\"] = cnt;\n";
        script += "            stuckObjects.push(s);\n";
        script += "        }\n";
        script += "    }\n";
        script += "}\n";
        script += "out[\"class_counts\"]   = classCounts;\n";
        script += "out[\"total_created\"]  = totalCreated;\n";
        script += "out[\"total_sunk\"]     = totalSunk;\n";
        script += "out[\"flow_balance\"]   = totalCreated - totalSunk - 0;\n";
        script += "out[\"stuck_objects\"]  = stuckObjects;\n";
        script += "return out;\n";

        nlohmann::json result;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (v.type == VariantType::Map) {
                result = pmValueToJson(v);
            } else {
                return returnError("summary_failed", "Summary script did not return a Map.");
            }
        } catch (const std::exception& e) {
            return returnError("summary_failed", e.what());
        }

        nlohmann::json out;
        out["ok"]      = true;
        out["summary"] = std::move(result);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_model_summary", e.what()); }
      catch (...)                     { return returnException("get_model_summary", "unknown"); }
}

// ============================================================================
// modelerai_get_property({ object, property })
//
// Reads a single property off an instance via FlexScript obj.getProperty().
// Uses the class schema to drive both return shape and ComboProperty name
// resolution: when value_kind == "enum" and the property has static options,
// we look up the option name for the returned integer value and include it
// in the response as `option_name`.
//
// Vec3/Color return as JSON arrays ([x,y,z] / [r,g,b,a]) so AI consumers
// get a shape symmetric with set_property's input.
//
// Schema miss is non-fatal — we return the raw Variant value typed by what
// executestring gave back, with `schema_miss: true` so the AI knows the
// kind is uncertain.
// ============================================================================
modelerai_export Variant ModelerAi_getProperty(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_property expects { object, property } JSON.");
        }
        std::string objectName, propertyName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName   = j.value("object",   std::string(""));
            propertyName = j.value("property", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || propertyName.empty()) {
            return returnError("missing_args", "object and property are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Resolve class via classobject(). Drives schema lookup + return shape.
        std::string className;
        try {
            TreeNode* clsNode = classobject(obj);
            if (clsNode) className = getname(clsNode);
        } catch (...) {}

        const ModelerAi::Schema::PropertyEntry* schemaEntry = nullptr;
        const ModelerAi::Schema::ClassSchema*   classSchema = nullptr;
        if (!className.empty()) {
            classSchema = ModelerAi::Schema::get(className);
            if (classSchema) {
                schemaEntry = ModelerAi::Schema::findProperty(className, propertyName);
            }
        }
        if (classSchema && !schemaEntry) {
            nlohmann::json err;
            err["error"]    = "property_not_on_class";
            err["object"]   = objectName;
            err["class"]    = className;
            err["property"] = propertyName;
            err["note"]     = "Call modelerai_list_properties({class:\"" + className +
                              "\"}) for the full list of readable properties.";
            return returnJson(err);
        }

        // Generate a script that returns the value in a JSON-friendly shape.
        // Vec3/Color unpack to arrays; everything else returns the raw value.
        std::string kind = schemaEntry ? schemaEntry->value_kind : std::string("");
        std::string script;
        script += "Object obj = Model.find(\"" + fsEscape(objectName) + "\");\n";
        if (kind == "vec3") {
            script += "Vec3 v = obj.getProperty(\"" + fsEscape(propertyName) + "\");\n";
            script += "Array a = Array(); a.push(v.x); a.push(v.y); a.push(v.z); return a;\n";
        } else if (kind == "color_rgba") {
            script += "Color c = obj.getProperty(\"" + fsEscape(propertyName) + "\");\n";
            script += "Array a = Array(); a.push(c.r); a.push(c.g); a.push(c.b); a.push(c.a); return a;\n";
        } else {
            script += "return obj.getProperty(\"" + fsEscape(propertyName) + "\");\n";
        }

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("getproperty_failed",
                "getProperty(\"" + propertyName + "\") threw: " + e.what());
        } catch (...) {
            return returnError("getproperty_failed",
                "getProperty(\"" + propertyName + "\") threw a non-std exception.");
        }

        // Variant -> JSON. Mirrors the canonical conversion used elsewhere.
        nlohmann::json valueJson;
        switch (result.type) {
            case VariantType::Null:
                valueJson = nullptr;
                break;
            case VariantType::Number: {
                double v = static_cast<double>(result);
                if (v == static_cast<double>(static_cast<long long>(v))
                    && v >= -9.2e18 && v <= 9.2e18) valueJson = static_cast<long long>(v);
                else valueJson = v;
                break;
            }
            case VariantType::String:
                valueJson = std::string(result);
                break;
            case VariantType::TreeNode: {
                TreeNode* n = static_cast<TreeNode*>(result);
                if (objectexists(n)) {
                    try {
                        const char* p = nodetomodelpath_cstr(n, 1);
                        nlohmann::json obj;
                        obj["name"] = std::string(getname(n));
                        if (p) obj["path"] = std::string(p);
                        valueJson = std::move(obj);
                    } catch (...) {
                        valueJson = std::string(getname(n));
                    }
                } else {
                    valueJson = nullptr;
                }
                break;
            }
            case VariantType::Array: {
                Array a = static_cast<Array>(result);
                nlohmann::json arr = nlohmann::json::array();
                for (int i = 1; i <= a.length; ++i) {
                    Variant e = a[i];
                    if (e.type == VariantType::Number) {
                        double v = static_cast<double>(e);
                        if (v == static_cast<double>(static_cast<long long>(v))
                            && v >= -9.2e18 && v <= 9.2e18) arr.push_back(static_cast<long long>(v));
                        else arr.push_back(v);
                    } else if (e.type == VariantType::String) {
                        arr.push_back(std::string(e));
                    } else if (e.type == VariantType::TreeNode) {
                        TreeNode* n = static_cast<TreeNode*>(e);
                        if (objectexists(n)) arr.push_back(std::string(getname(n)));
                        else arr.push_back(nullptr);
                    } else {
                        arr.push_back(nullptr);
                    }
                }
                valueJson = std::move(arr);
                break;
            }
            default:
                valueJson = nullptr;
                break;
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["object"]   = objectName;
        out["class"]    = className;
        out["property"] = propertyName;
        out["value"]    = std::move(valueJson);
        if (schemaEntry) {
            out["value_kind"] = schemaEntry->value_kind;
            // Enum: resolve the integer value back to its option name.
            if (schemaEntry->value_kind == "enum" &&
                schemaEntry->options_kind == "static" &&
                out["value"].is_number_integer()) {
                long long v = out["value"].get<long long>();
                for (const auto& kv : schemaEntry->options) {
                    if (kv.second == v) { out["option_name"] = kv.first; break; }
                }
            }
        } else {
            out["schema_miss"] = true;
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_property", e.what()); }
      catch (...)                     { return returnException("get_property", "unknown"); }
}

// ============================================================================
// modelerai_list_properties({ class? , object?, category? })
//
// Returns a compact list of settable / readable properties for a class —
// names + value_kinds, with ComboProperty options inlined. Either takes a
// `class` directly OR an `object` (resolved via Model.find, class via
// classobject()). Optional `category` filters by the schema's category
// field (substring match — "FixedResource>Source" matches "Source",
// "FixedResource", etc.).
//
// Designed for AI discovery: small payload, enough to pick a property and
// shape its value before calling set_property. For the full schema (raw
// property paths, source classification, components, etc.) re-run
// modelerai_extract_class_schema with the on-disk file as evidence.
// ============================================================================
modelerai_export Variant ModelerAi_listProperties(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_list_properties expects { class } or { object } JSON.");
        }
        std::string className, objectName, categoryFilter;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            className      = j.value("class",    std::string(""));
            objectName     = j.value("object",   std::string(""));
            categoryFilter = j.value("category", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        // Resolve class from object if needed.
        if (className.empty() && !objectName.empty()) {
            TreeNode* obj = model()->find(objectName.c_str());
            if (!objectexists(obj)) {
                return returnError("not_found",
                    "Object '" + objectName + "' did not resolve via Model.find.");
            }
            try {
                TreeNode* clsNode = classobject(obj);
                if (clsNode) className = getname(clsNode);
            } catch (...) {}
            if (className.empty()) {
                return returnError("class_unresolved",
                    "Could not determine class for object '" + objectName + "'.");
            }
        }
        if (className.empty()) {
            return returnError("missing_args",
                "Pass either { class: \"Source\" } or { object: \"Source1\" }.");
        }

        const ModelerAi::Schema::ClassSchema* schema = ModelerAi::Schema::get(className);
        if (!schema) {
            nlohmann::json err;
            err["error"] = "no_schema";
            err["class"] = className;
            err["note"]  = "No schema on disk at KNOWLEDGE/schemas/" +
                           className + ".json. Either the class name is wrong, "
                           "or schemas need to be regenerated.";
            return returnJson(err);
        }

        nlohmann::json props = nlohmann::json::array();
        for (const auto& name : schema->property_order) {
            auto it = schema->by_name.find(name);
            if (it == schema->by_name.end()) continue;
            const auto& e = it->second;
            if (!categoryFilter.empty()) {
                if (e.category.find(categoryFilter) == std::string::npos) continue;
            }
            nlohmann::json p;
            p["name"]       = e.name;
            p["value_kind"] = e.value_kind;
            if (!e.category.empty())       p["category"]       = e.category;
            if (!e.localized_name.empty()) p["localized_name"] = e.localized_name;
            if (!e.components.empty()) {
                nlohmann::json comps = nlohmann::json::array();
                for (const auto& c : e.components) comps.push_back(c);
                p["components"] = std::move(comps);
            }
            if (e.value_kind == "enum") {
                p["options_kind"] = e.options_kind;
                if (!e.options.empty()) {
                    nlohmann::json opts = nlohmann::json::array();
                    for (const auto& kv : e.options) {
                        nlohmann::json o; o["name"] = kv.first; o["value"] = kv.second;
                        opts.push_back(std::move(o));
                    }
                    p["options"] = std::move(opts);
                }
                if (!e.options_path.empty()) p["options_path"] = e.options_path;
            }
            props.push_back(std::move(p));
        }

        nlohmann::json out;
        out["ok"]             = true;
        out["class"]          = className;
        if (!objectName.empty())     out["object"]   = objectName;
        if (!categoryFilter.empty()) out["category"] = categoryFilter;
        out["property_count"] = static_cast<int>(props.size());
        out["properties"]     = std::move(props);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_properties", e.what()); }
      catch (...)                     { return returnException("list_properties", "unknown"); }
}

// ============================================================================
// Groups
// ============================================================================
// A Group is a FlexSim Tool that holds references to other model objects.
// Backed by `Tools.create("Group")` + `Group("name").addMember(node)`.
//
//   modelerai_create_group        — assert/create a group by name, optional initial members
//   modelerai_group_add_member    — add one or many members; skips already-members
//   modelerai_group_remove_member — remove one or many members; reports not-members
//   modelerai_list_groups         — list every group + member count
//   modelerai_list_group_members  — list one group's members (flat, descends nested groups)
// ============================================================================

// Build a FlexScript snippet that resolves a member name to a treenode.
// Tries Model.find() first (the common "Op1", "Processor3" case); on miss
// falls back to Tools.get("Group", name) so the AI can NEST groups by
// passing a group name as a member. Both addMember and removeMember accept
// a Group treenode just fine — that's how FlexSim represents nesting.
//
// Generates FlexScript that leaves a `treenode obj` variable in scope.
// Caller must supply the name (already-escaped form NOT required — we
// fsEscape inside).
namespace {
std::string resolveMemberSnippet(const std::string& memberName)
{
    std::string s;
    s += "treenode obj = Model.find(\"" + fsEscape(memberName) + "\");\n";
    s += "if (!objectexists(obj)) obj = Tools.get(\"Group\", \"" + fsEscape(memberName) + "\");\n";
    return s;
}
} // namespace

// modelerai_create_group({ name, members? }) — idempotent: returns the
// existing group if one with that name already exists, with `existing: true`.
modelerai_export Variant ModelerAi_createGroup(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_group expects { name, members? } JSON.");
        }
        std::string groupName;
        std::vector<std::string> initialMembers;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            groupName = j.value("name", std::string(""));
            if (j.contains("members") && j["members"].is_array()) {
                for (const auto& m : j["members"]) {
                    if (m.is_string()) initialMembers.push_back(m.get<std::string>());
                }
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (groupName.empty()) {
            return returnError("missing_name", "name is required.");
        }

        // Build the create-or-assert script. Tools.get returns null if the
        // group doesn't exist; only call Tools.create in that case so we're
        // idempotent on the name.
        std::string script;
        script += "treenode existing = Tools.get(\"Group\", \"" + fsEscape(groupName) + "\");\n";
        script += "Map out = Map();\n";
        script += "if (existing) {\n";
        script += "    out[\"existing\"] = 1;\n";
        script += "    out[\"node\"]     = existing;\n";
        script += "} else {\n";
        script += "    treenode g = Tools.create(\"Group\");\n";
        script += "    setname(g, \"" + fsEscape(groupName) + "\");\n";
        script += "    out[\"existing\"] = 0;\n";
        script += "    out[\"node\"]     = g;\n";
        script += "}\n";
        // Optionally add initial members. The Group(name) reference works
        // for both newly-created and pre-existing groups.
        if (!initialMembers.empty()) {
            script += "Group gref = Group(\"" + fsEscape(groupName) + "\");\n";
            script += "double addedCount   = 0;\n";
            script += "double skippedCount = 0;\n";
            script += "Array notFound = Array();\n";
            for (const auto& m : initialMembers) {
                script += "{\n";
                script += resolveMemberSnippet(m);
                script += "    if (!objectexists(obj)) { notFound.push(\"" + fsEscape(m) + "\"); }\n";
                script += "    else if (gref.isMember(obj, 0)) { skippedCount = skippedCount + 1; }\n";
                script += "    else { gref.addMember(obj); addedCount = addedCount + 1; }\n";
                script += "}\n";
            }
            script += "out[\"added\"]      = addedCount;\n";
            script += "out[\"skipped\"]    = skippedCount;\n";
            script += "out[\"not_found\"]  = notFound;\n";
        }
        script += "out[\"member_count\"] = Group(\"" + fsEscape(groupName) + "\").length;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("create_group_failed", e.what());
        } catch (...) {
            return returnError("create_group_failed", "non-std exception.");
        }

        nlohmann::json out;
        out["ok"]   = true;
        out["name"] = groupName;
        if (result.type == VariantType::Map) {
            try {
                Map m = static_cast<Map>(result);
                Variant existingV = m["existing"];
                out["existing"] = (static_cast<double>(existingV) != 0.0);
                Variant nodeV = m["node"];
                if (nodeV.type == VariantType::TreeNode) {
                    TreeNode* n = static_cast<TreeNode*>(nodeV);
                    if (objectexists(n)) {
                        try {
                            const char* p = nodetomodelpath_cstr(n, 1);
                            if (p) out["path"] = std::string(p);
                        } catch (...) {}
                    }
                }
                Variant memCountV = m["member_count"];
                out["member_count"] = (int)static_cast<double>(memCountV);
                if (!initialMembers.empty()) {
                    out["added"]   = (int)static_cast<double>(m["added"]);
                    out["skipped"] = (int)static_cast<double>(m["skipped"]);
                    nlohmann::json nf = nlohmann::json::array();
                    Variant nfV = m["not_found"];
                    if (nfV.type == VariantType::Array) {
                        Array a = static_cast<Array>(nfV);
                        for (int i = 1; i <= a.length; ++i) nf.push_back(std::string(a[i]));
                    }
                    out["not_found"] = std::move(nf);
                }
            } catch (...) {}
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_group", e.what()); }
      catch (...)                     { return returnException("create_group", "unknown"); }
}

// Shared body for add/remove: walks a members list and applies an op to
// each. Members can be passed as a single string OR an array of strings.
namespace {
struct GroupMemberOpArgs {
    std::string group;
    std::vector<std::string> members;
};
GroupMemberOpArgs parseGroupOpArgs(const Variant& arg, std::string& outErr)
{
    GroupMemberOpArgs out;
    if (arg.type != VariantType::String) {
        outErr = "expected { group, member | members } JSON";
        return out;
    }
    try {
        auto j = nlohmann::json::parse(std::string(arg));
        if (!j.is_object()) { outErr = "expected JSON object"; return out; }
        out.group = j.value("group", std::string(""));
        if (j.contains("member") && j["member"].is_string()) {
            out.members.push_back(j["member"].get<std::string>());
        }
        if (j.contains("members") && j["members"].is_array()) {
            for (const auto& m : j["members"]) {
                if (m.is_string()) out.members.push_back(m.get<std::string>());
            }
        }
    } catch (const std::exception& e) {
        outErr = std::string("parse: ") + e.what();
    }
    return out;
}
} // namespace

modelerai_export Variant ModelerAi_groupAddMember(FLEXSIMINTERFACE)
{
    try {
        std::string err;
        auto a = parseGroupOpArgs(param(1), err);
        if (!err.empty())            return returnError("bad_args", err);
        if (a.group.empty())         return returnError("missing_group",   "group is required.");
        if (a.members.empty())       return returnError("missing_member",  "pass `member` or `members`.");

        std::string script;
        script += "treenode g = Tools.get(\"Group\", \"" + fsEscape(a.group) + "\");\n";
        script += "if (!g) return -1;\n";
        script += "Group gref = Group(\"" + fsEscape(a.group) + "\");\n";
        script += "Map out = Map();\n";
        script += "double addedCount   = 0;\n";
        script += "double skippedCount = 0;\n";
        script += "Array notFound = Array();\n";
        for (const auto& m : a.members) {
            script += "{\n";
            script += resolveMemberSnippet(m);
            script += "    if (!objectexists(obj)) { notFound.push(\"" + fsEscape(m) + "\"); }\n";
            script += "    else if (gref.isMember(obj, 0)) { skippedCount = skippedCount + 1; }\n";
            script += "    else { gref.addMember(obj); addedCount = addedCount + 1; }\n";
            script += "}\n";
        }
        script += "out[\"added\"]      = addedCount;\n";
        script += "out[\"skipped\"]    = skippedCount;\n";
        script += "out[\"not_found\"]  = notFound;\n";
        script += "out[\"total_after\"]= gref.length;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("add_member_failed", e.what()); }
          catch (...) { return returnError("add_member_failed", "non-std exception."); }
        if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
            return returnError("group_not_found",
                "Group '" + a.group + "' does not exist. Use modelerai_create_group first.");
        }
        if (result.type != VariantType::Map) {
            return returnError("add_member_failed", "script returned wrong type.");
        }
        Map m = static_cast<Map>(result);
        nlohmann::json out;
        out["ok"]          = true;
        out["group"]       = a.group;
        out["added"]       = (int)static_cast<double>(m["added"]);
        out["skipped"]     = (int)static_cast<double>(m["skipped"]);
        nlohmann::json nf = nlohmann::json::array();
        Variant nfV = m["not_found"];
        if (nfV.type == VariantType::Array) {
            Array arr = static_cast<Array>(nfV);
            for (int i = 1; i <= arr.length; ++i) nf.push_back(std::string(arr[i]));
        }
        out["not_found"]   = std::move(nf);
        out["total_after"] = (int)static_cast<double>(m["total_after"]);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("group_add_member", e.what()); }
      catch (...)                     { return returnException("group_add_member", "unknown"); }
}

modelerai_export Variant ModelerAi_groupRemoveMember(FLEXSIMINTERFACE)
{
    try {
        std::string err;
        auto a = parseGroupOpArgs(param(1), err);
        if (!err.empty())      return returnError("bad_args", err);
        if (a.group.empty())   return returnError("missing_group",  "group is required.");
        if (a.members.empty()) return returnError("missing_member", "pass `member` or `members`.");

        std::string script;
        script += "treenode g = Tools.get(\"Group\", \"" + fsEscape(a.group) + "\");\n";
        script += "if (!g) return -1;\n";
        script += "Group gref = Group(\"" + fsEscape(a.group) + "\");\n";
        script += "Map out = Map();\n";
        script += "double removedCount  = 0;\n";
        script += "double notMemberCount = 0;\n";
        script += "Array notFound = Array();\n";
        for (const auto& m : a.members) {
            script += "{\n";
            script += resolveMemberSnippet(m);
            script += "    if (!objectexists(obj)) { notFound.push(\"" + fsEscape(m) + "\"); }\n";
            script += "    else if (!gref.isMember(obj, 0)) { notMemberCount = notMemberCount + 1; }\n";
            script += "    else { gref.removeMember(obj); removedCount = removedCount + 1; }\n";
            script += "}\n";
        }
        script += "out[\"removed\"]    = removedCount;\n";
        script += "out[\"not_member\"] = notMemberCount;\n";
        script += "out[\"not_found\"]  = notFound;\n";
        script += "out[\"total_after\"]= gref.length;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("remove_member_failed", e.what()); }
          catch (...) { return returnError("remove_member_failed", "non-std exception."); }
        if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
            return returnError("group_not_found",
                "Group '" + a.group + "' does not exist.");
        }
        if (result.type != VariantType::Map) {
            return returnError("remove_member_failed", "script returned wrong type.");
        }
        Map m = static_cast<Map>(result);
        nlohmann::json out;
        out["ok"]          = true;
        out["group"]       = a.group;
        out["removed"]     = (int)static_cast<double>(m["removed"]);
        out["not_member"]  = (int)static_cast<double>(m["not_member"]);
        nlohmann::json nf = nlohmann::json::array();
        Variant nfV = m["not_found"];
        if (nfV.type == VariantType::Array) {
            Array arr = static_cast<Array>(nfV);
            for (int i = 1; i <= arr.length; ++i) nf.push_back(std::string(arr[i]));
        }
        out["not_found"]   = std::move(nf);
        out["total_after"] = (int)static_cast<double>(m["total_after"]);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("group_remove_member", e.what()); }
      catch (...)                     { return returnException("group_remove_member", "unknown"); }
}

// modelerai_list_groups({}) — every Group under Tools, with member counts.
modelerai_export Variant ModelerAi_listGroups(FLEXSIMINTERFACE)
{
    try {
        // Groups live under model().find("Tools/Groups"). The folder may
        // not exist yet on a brand-new model — handle null cleanly.
        std::string script;
        script += "treenode root = model().find(\"Tools/Groups\");\n";
        script += "Array out = Array();\n";
        script += "if (root) {\n";
        script += "    for (int i = 1; i <= root.subnodes.length; i++) {\n";
        script += "        treenode g = root.subnodes[i];\n";
        script += "        Map entry = Map();\n";
        script += "        entry[\"name\"] = string(getname(g));\n";
        script += "        entry[\"member_count\"] = Group(string(getname(g))).length;\n";
        script += "        out.push(entry);\n";
        script += "    }\n";
        script += "}\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("list_groups_failed", e.what()); }
          catch (...) { return returnError("list_groups_failed", "non-std exception."); }

        nlohmann::json groups = nlohmann::json::array();
        if (result.type == VariantType::Array) {
            Array a = static_cast<Array>(result);
            for (int i = 1; i <= a.length; ++i) {
                Variant ev = a[i];
                if (ev.type != VariantType::Map) continue;
                Map m = static_cast<Map>(ev);
                nlohmann::json e;
                e["name"]         = std::string(m["name"]);
                e["member_count"] = (int)static_cast<double>(m["member_count"]);
                groups.push_back(std::move(e));
            }
        }
        nlohmann::json out;
        out["ok"]          = true;
        out["group_count"] = (int)groups.size();
        out["groups"]      = std::move(groups);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_groups", e.what()); }
      catch (...)                     { return returnException("list_groups", "unknown"); }
}

// modelerai_list_group_members({ group, recursive? }) — flat list of every
// member. Default is recursive=true (uses gref.toFlatArray()) since "what
// is in this group?" almost always wants the full leaf set, not just
// directly-attached children. Pass recursive=false for the structural
// view (direct members only — nested groups appear as group entries).
//
// The response always carries `has_nested_groups` so the caller knows
// whether the recursive=true result was flattened from a nested structure
// or was already flat — useful when deciding whether `recursive: false`
// would say something different.
modelerai_export Variant ModelerAi_listGroupMembers(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string groupName;
        bool recursive = true;   // default: full flat membership
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) {
                        groupName = j.value("group",     std::string(""));
                        recursive = j.value("recursive", true);
                    }
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            } else {
                groupName = s;
            }
        }
        if (groupName.empty()) {
            return returnError("missing_group", "Pass { group: \"Name\" } or a bare group name.");
        }

        // We always walk direct children at least once to count nested
        // groups; if recursive is on we ALSO emit the toFlatArray result
        // as the canonical members list.
        std::string script;
        script += "treenode g = Tools.get(\"Group\", \"" + fsEscape(groupName) + "\");\n";
        script += "if (!g) return -1;\n";
        script += "Group gref = Group(\"" + fsEscape(groupName) + "\");\n";
        script += "Map out = Map();\n";
        script += "Array members = Array();\n";
        script += "double nestedCount = 0;\n";
        script += "double directCount = gref.length;\n";
        // First pass — scan direct children to count nested groups. A
        // direct child is "nested" when its parent under Tools is the
        // Groups folder (i.e. its class is Group).
        script += "for (int i = 1; i <= gref.length; i++) {\n";
        script += "    treenode child = gref[i];\n";
        script += "    if (!objectexists(child)) continue;\n";
        script += "    treenode cls = classobject(child);\n";
        script += "    if (cls && string(getname(cls)) == \"Group\") nestedCount = nestedCount + 1;\n";
        script += "}\n";
        if (recursive) {
            script += "Array flat = gref.toFlatArray();\n";
            script += "for (int i = 1; i <= flat.length; i++) {\n";
            script += "    treenode m = flat[i];\n";
            script += "    if (objectexists(m)) members.push(string(getname(m)));\n";
            script += "}\n";
        } else {
            script += "for (int i = 1; i <= gref.length; i++) {\n";
            script += "    treenode m = gref[i];\n";
            script += "    if (objectexists(m)) members.push(string(getname(m)));\n";
            script += "}\n";
        }
        script += "out[\"members\"]      = members;\n";
        script += "out[\"nested\"]       = nestedCount;\n";
        script += "out[\"direct_count\"] = directCount;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("list_members_failed", e.what()); }
          catch (...) { return returnError("list_members_failed", "non-std exception."); }
        if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
            return returnError("group_not_found", "Group '" + groupName + "' does not exist.");
        }
        if (result.type != VariantType::Map) {
            return returnError("list_members_failed", "script returned wrong type.");
        }
        Map m = static_cast<Map>(result);
        nlohmann::json members = nlohmann::json::array();
        Variant memsV = m["members"];
        if (memsV.type == VariantType::Array) {
            Array a = static_cast<Array>(memsV);
            for (int i = 1; i <= a.length; ++i) members.push_back(std::string(a[i]));
        }
        int nestedCount = (int)static_cast<double>(m["nested"]);
        nlohmann::json out;
        out["ok"]                 = true;
        out["group"]              = groupName;
        out["recursive"]          = recursive;
        out["member_count"]       = (int)members.size();
        out["direct_count"]       = (int)static_cast<double>(m["direct_count"]);
        out["has_nested_groups"]  = (nestedCount > 0);
        out["nested_group_count"] = nestedCount;
        out["members"]            = std::move(members);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_group_members", e.what()); }
      catch (...)                     { return returnException("list_group_members", "unknown"); }
}

// ============================================================================
// Global Tables
// ============================================================================
// Backed by Tools.create("GlobalTable") + Table("name")[row][col].
// Cells are 1-indexed (FlexSim convention). Headers are optional row/column
// labels separate from the cell data.
//
//   modelerai_create_global_table       — assert/create; optional dims + headers + initial cells
//   modelerai_resize_global_table       — gt.setSize(rows, cols)
//   modelerai_set_global_table_cell     — set one cell (number or string)
//   modelerai_get_global_table_cell     — read one cell with value_kind
//   modelerai_list_global_tables        — every GlobalTable + dimensions
// ============================================================================

// Helper: serialize a JSON value as a FlexScript literal for a cell write.
// Returns the literal string, or empty if the JSON value isn't a supported
// shape (number / boolean / string).
namespace {
bool cellJsonToLiteral(const nlohmann::json& v, std::string& outLit, std::string& outKind)
{
    if (v.is_boolean()) {
        outLit  = v.get<bool>() ? "1" : "0";
        outKind = "number";
        return true;
    }
    if (v.is_number()) {
        std::ostringstream s; s << v.get<double>();
        outLit  = s.str();
        outKind = "number";
        return true;
    }
    if (v.is_string()) {
        outLit  = "\"" + fsEscape(v.get<std::string>()) + "\"";
        outKind = "string";
        return true;
    }
    return false;
}
} // namespace

// modelerai_create_global_table({ name, rows?, cols?, row_headers?, col_headers?, cells? })
// Idempotent on name — returns existing:true when already present.
modelerai_export Variant ModelerAi_createGlobalTable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_global_table expects { name, rows?, cols?, row_headers?, col_headers?, cells? } JSON.");
        }
        std::string tableName;
        int rows = -1, cols = -1;
        std::vector<std::string> rowHeaders, colHeaders;
        nlohmann::json cells;     // 2D array; empty if not provided
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            tableName = j.value("name", std::string(""));
            if (j.contains("rows") && j["rows"].is_number_integer()) rows = j["rows"].get<int>();
            if (j.contains("cols") && j["cols"].is_number_integer()) cols = j["cols"].get<int>();
            if (j.contains("row_headers") && j["row_headers"].is_array()) {
                for (const auto& h : j["row_headers"]) {
                    if (h.is_string()) rowHeaders.push_back(h.get<std::string>());
                }
            }
            if (j.contains("col_headers") && j["col_headers"].is_array()) {
                for (const auto& h : j["col_headers"]) {
                    if (h.is_string()) colHeaders.push_back(h.get<std::string>());
                }
            }
            if (j.contains("cells") && j["cells"].is_array()) cells = j["cells"];
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (tableName.empty()) return returnError("missing_name", "name is required.");
        if (rows < 0) rows = 0;
        if (cols < 0) cols = 0;

        std::string script;
        script += "treenode existing = Tools.get(\"GlobalTable\", \"" + fsEscape(tableName) + "\");\n";
        script += "Map out = Map();\n";
        script += "if (existing) {\n";
        script += "    out[\"existing\"] = 1;\n";
        script += "    out[\"node\"]     = existing;\n";
        script += "} else {\n";
        script += "    treenode t = Tools.create(\"GlobalTable\");\n";
        script += "    setname(t, \"" + fsEscape(tableName) + "\");\n";
        script += "    out[\"existing\"] = 0;\n";
        script += "    out[\"node\"]     = t;\n";
        script += "}\n";
        // Resize if dims given. setSize is no-op on the same size.
        if (rows > 0 && cols > 0) {
            script += "Table(\"" + fsEscape(tableName) + "\").setSize(" +
                      std::to_string(rows) + ", " + std::to_string(cols) + ");\n";
        }
        // Headers.
        for (size_t r = 0; r < rowHeaders.size(); ++r) {
            script += "Table(\"" + fsEscape(tableName) + "\").setRowHeader(" +
                      std::to_string(r + 1) + ", \"" + fsEscape(rowHeaders[r]) + "\");\n";
        }
        for (size_t c = 0; c < colHeaders.size(); ++c) {
            script += "Table(\"" + fsEscape(tableName) + "\").setColHeader(" +
                      std::to_string(c + 1) + ", \"" + fsEscape(colHeaders[c]) + "\");\n";
        }
        // Initial cell values (1-indexed in FlexScript; cells[r][c] in JSON
        // are 0-indexed in our parser, so add 1 when emitting).
        int cellsWritten = 0, cellsSkipped = 0;
        if (cells.is_array()) {
            for (size_t r = 0; r < cells.size(); ++r) {
                const auto& row = cells[r];
                if (!row.is_array()) continue;
                for (size_t c = 0; c < row.size(); ++c) {
                    std::string lit, kind;
                    if (!cellJsonToLiteral(row[c], lit, kind)) { ++cellsSkipped; continue; }
                    script += "Table(\"" + fsEscape(tableName) + "\")[" +
                              std::to_string(r + 1) + "][" + std::to_string(c + 1) +
                              "] = " + lit + ";\n";
                    ++cellsWritten;
                }
            }
        }
        script += "out[\"num_rows\"] = Table(\"" + fsEscape(tableName) + "\").numRows;\n";
        script += "out[\"num_cols\"] = Table(\"" + fsEscape(tableName) + "\").numCols;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("create_table_failed", e.what()); }
          catch (...) { return returnError("create_table_failed", "non-std exception."); }

        nlohmann::json out;
        out["ok"]            = true;
        out["name"]          = tableName;
        out["cells_written"] = cellsWritten;
        out["cells_skipped"] = cellsSkipped;
        if (result.type == VariantType::Map) {
            try {
                Map m = static_cast<Map>(result);
                out["existing"] = (static_cast<double>(m["existing"]) != 0.0);
                Variant nodeV = m["node"];
                if (nodeV.type == VariantType::TreeNode) {
                    TreeNode* n = static_cast<TreeNode*>(nodeV);
                    if (objectexists(n)) {
                        try {
                            const char* p = nodetomodelpath_cstr(n, 1);
                            if (p) out["path"] = std::string(p);
                        } catch (...) {}
                    }
                }
                out["num_rows"] = (int)static_cast<double>(m["num_rows"]);
                out["num_cols"] = (int)static_cast<double>(m["num_cols"]);
            } catch (...) {}
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_global_table", e.what()); }
      catch (...)                     { return returnException("create_global_table", "unknown"); }
}

// modelerai_resize_global_table({ name, rows, cols, overwrite? })
modelerai_export Variant ModelerAi_resizeGlobalTable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_resize_global_table expects { name, rows, cols, overwrite? } JSON.");
        }
        std::string tableName;
        int rows = -1, cols = -1;
        bool overwrite = false;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            tableName = j.value("name", std::string(""));
            if (j.contains("rows") && j["rows"].is_number_integer()) rows = j["rows"].get<int>();
            if (j.contains("cols") && j["cols"].is_number_integer()) cols = j["cols"].get<int>();
            overwrite = j.value("overwrite", false);
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (tableName.empty()) return returnError("missing_name", "name is required.");
        if (rows < 0 || cols < 0) return returnError("bad_dims", "rows and cols must be >= 0.");

        std::string script;
        script += "treenode t = Tools.get(\"GlobalTable\", \"" + fsEscape(tableName) + "\");\n";
        script += "if (!t) return -1;\n";
        script += "Table(\"" + fsEscape(tableName) + "\").setSize(" +
                  std::to_string(rows) + ", " + std::to_string(cols) + ", 0, " +
                  std::string(overwrite ? "1" : "0") + ");\n";
        script += "Map out = Map();\n";
        script += "out[\"num_rows\"] = Table(\"" + fsEscape(tableName) + "\").numRows;\n";
        script += "out[\"num_cols\"] = Table(\"" + fsEscape(tableName) + "\").numCols;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("resize_table_failed", e.what()); }
          catch (...) { return returnError("resize_table_failed", "non-std exception."); }
        if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
            return returnError("table_not_found",
                "GlobalTable '" + tableName + "' does not exist. Use modelerai_create_global_table first.");
        }
        nlohmann::json out;
        out["ok"]   = true;
        out["name"] = tableName;
        if (result.type == VariantType::Map) {
            Map m = static_cast<Map>(result);
            out["num_rows"] = (int)static_cast<double>(m["num_rows"]);
            out["num_cols"] = (int)static_cast<double>(m["num_cols"]);
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("resize_global_table", e.what()); }
      catch (...)                     { return returnException("resize_global_table", "unknown"); }
}

// modelerai_set_global_table_cell({ table, row, col, value })
modelerai_export Variant ModelerAi_setGlobalTableCell(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_global_table_cell expects { table, row, col, value } JSON.");
        }
        std::string tableName;
        int row = -1, col = -1;
        nlohmann::json valueJson;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            tableName = j.value("table", std::string(""));
            if (j.contains("row") && j["row"].is_number_integer()) row = j["row"].get<int>();
            if (j.contains("col") && j["col"].is_number_integer()) col = j["col"].get<int>();
            if (!j.contains("value")) return returnError("missing_value", "value is required.");
            valueJson = j["value"];
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (tableName.empty()) return returnError("missing_table", "table is required.");
        if (row < 1 || col < 1) {
            return returnError("bad_index", "row and col are 1-indexed and must be >= 1.");
        }
        std::string lit, kind;
        if (!cellJsonToLiteral(valueJson, lit, kind)) {
            return returnError("unsupported_value_type",
                "value must be a number, boolean, or string.");
        }

        std::string script;
        script += "treenode t = Tools.get(\"GlobalTable\", \"" + fsEscape(tableName) + "\");\n";
        script += "if (!t) return -1;\n";
        script += "int nr = Table(\"" + fsEscape(tableName) + "\").numRows;\n";
        script += "int nc = Table(\"" + fsEscape(tableName) + "\").numCols;\n";
        script += "if (" + std::to_string(row) + " > nr || " + std::to_string(col) + " > nc) return -2;\n";
        script += "Table(\"" + fsEscape(tableName) + "\")[" +
                  std::to_string(row) + "][" + std::to_string(col) + "] = " + lit + ";\n";
        script += "return 1;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("set_cell_failed", e.what()); }
          catch (...) { return returnError("set_cell_failed", "non-std exception."); }
        if (result.type == VariantType::Number) {
            double v = static_cast<double>(result);
            if (v == -1) return returnError("table_not_found", "GlobalTable '" + tableName + "' does not exist.");
            if (v == -2) {
                nlohmann::json err;
                err["error"]   = "index_out_of_range";
                err["table"]   = tableName;
                err["row"]     = row;
                err["col"]     = col;
                err["message"] = "row/col exceed table dimensions; call resize first.";
                return returnJson(err);
            }
        }
        nlohmann::json out;
        out["ok"]         = true;
        out["table"]      = tableName;
        out["row"]        = row;
        out["col"]        = col;
        out["value_kind"] = kind;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_global_table_cell", e.what()); }
      catch (...)                     { return returnException("set_global_table_cell", "unknown"); }
}

// modelerai_get_global_table_cell({ table, row, col })
modelerai_export Variant ModelerAi_getGlobalTableCell(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_global_table_cell expects { table, row, col } JSON.");
        }
        std::string tableName;
        int row = -1, col = -1;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            tableName = j.value("table", std::string(""));
            if (j.contains("row") && j["row"].is_number_integer()) row = j["row"].get<int>();
            if (j.contains("col") && j["col"].is_number_integer()) col = j["col"].get<int>();
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (tableName.empty()) return returnError("missing_table", "table is required.");
        if (row < 1 || col < 1) return returnError("bad_index", "row and col are 1-indexed and must be >= 1.");

        // Probe existence + range; on success return the cell Variant directly.
        // Bookkeeping: we use a Map to carry { value, in_range } back so the
        // C++ side can distinguish "cell holds 0" from "cell out of range".
        std::string script;
        script += "treenode t = Tools.get(\"GlobalTable\", \"" + fsEscape(tableName) + "\");\n";
        script += "if (!t) return -1;\n";
        script += "int nr = Table(\"" + fsEscape(tableName) + "\").numRows;\n";
        script += "int nc = Table(\"" + fsEscape(tableName) + "\").numCols;\n";
        script += "if (" + std::to_string(row) + " > nr || " + std::to_string(col) + " > nc) return -2;\n";
        script += "Map out = Map();\n";
        script += "out[\"value\"] = Table(\"" + fsEscape(tableName) + "\")[" +
                  std::to_string(row) + "][" + std::to_string(col) + "];\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("get_cell_failed", e.what()); }
          catch (...) { return returnError("get_cell_failed", "non-std exception."); }
        if (result.type == VariantType::Number) {
            double v = static_cast<double>(result);
            if (v == -1) return returnError("table_not_found", "GlobalTable '" + tableName + "' does not exist.");
            if (v == -2) {
                nlohmann::json err;
                err["error"] = "index_out_of_range";
                err["table"] = tableName;
                err["row"]   = row;
                err["col"]   = col;
                return returnJson(err);
            }
        }
        if (result.type != VariantType::Map) {
            return returnError("get_cell_failed", "script returned wrong type.");
        }
        Map m = static_cast<Map>(result);
        Variant cell = m["value"];
        nlohmann::json valueJson;
        std::string kind;
        switch (cell.type) {
            case VariantType::Number: {
                double v = static_cast<double>(cell);
                if (v == static_cast<double>(static_cast<long long>(v))
                    && v >= -9.2e18 && v <= 9.2e18) valueJson = static_cast<long long>(v);
                else valueJson = v;
                kind = "number";
                break;
            }
            case VariantType::String:
                valueJson = std::string(cell);
                kind = "string";
                break;
            case VariantType::Null:
                valueJson = nullptr;
                kind = "null";
                break;
            default:
                valueJson = nullptr;
                kind = "unknown";
                break;
        }
        nlohmann::json out;
        out["ok"]         = true;
        out["table"]      = tableName;
        out["row"]        = row;
        out["col"]        = col;
        out["value"]      = std::move(valueJson);
        out["value_kind"] = kind;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_global_table_cell", e.what()); }
      catch (...)                     { return returnException("get_global_table_cell", "unknown"); }
}

// ============================================================================
// modelerai_get_parameter({ name }) — read one parameter's current value.
//
// Model.parameters[name] is GLOBAL (cross-table). If the same name lives
// in multiple tables, only the first hit is returned; AI should rename
// duplicates if it cares which one is read.
// ============================================================================
modelerai_export Variant ModelerAi_getParameter(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string name;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) name = j.value("name", std::string(""));
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            } else {
                name = s;
            }
        }
        if (name.empty()) {
            return returnError("missing_name", "Pass { name: \"X\" } or a bare name string.");
        }

        // Probe + read in one go. -1 means parameter doesn't exist.
        std::string script;
        script += "Map out = Map();\n";
        script += "Array allNames = Model.parameters.names();\n";
        script += "int found = 0;\n";
        script += "for (int i = 1; i <= allNames.length; i++) {\n";
        script += "    if (string(allNames[i]) == \"" + fsEscape(name) + "\") { found = 1; break; }\n";
        script += "}\n";
        script += "if (!found) return -1;\n";
        script += "out[\"value\"] = Model.parameters[\"" + fsEscape(name) + "\"].value;\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("get_parameter_failed", e.what()); }
          catch (...) { return returnError("get_parameter_failed", "non-std exception."); }
        if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
            return returnError("parameter_not_found",
                "No parameter named '" + name + "'. Call modelerai_list_parameters to see what's defined.");
        }
        if (result.type != VariantType::Map) {
            return returnError("get_parameter_failed", "script returned wrong type.");
        }
        Map m = static_cast<Map>(result);
        Variant v = m["value"];
        nlohmann::json valueJson;
        std::string kind;
        switch (v.type) {
            case VariantType::Number: {
                double d = static_cast<double>(v);
                if (d == static_cast<double>(static_cast<long long>(d))
                    && d >= -9.2e18 && d <= 9.2e18) valueJson = static_cast<long long>(d);
                else valueJson = d;
                kind = "number";
                break;
            }
            case VariantType::String:
                valueJson = std::string(v);
                kind = "string";
                break;
            case VariantType::Null:
                valueJson = nullptr;
                kind = "null";
                break;
            default:
                valueJson = nullptr;
                kind = "unknown";
                break;
        }
        nlohmann::json out;
        out["ok"]         = true;
        out["name"]       = name;
        out["value"]      = std::move(valueJson);
        out["value_kind"] = kind;
        out["accessor"]   = "Model.parameters[\"" + name + "\"]";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_parameter", e.what()); }
      catch (...)                     { return returnException("get_parameter", "unknown"); }
}

// ============================================================================
// modelerai_list_parameters({ table_name? }) — every parameter (across all
// ParameterTables when no table_name given) with current values.
// ============================================================================
modelerai_export Variant ModelerAi_listParameters(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string tableFilter;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) tableFilter = j.value("table_name", std::string(""));
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            }
        }

        std::string script;
        script += "Array tableNames = Model.parameters.tableNames;\n";
        script += "Array out = Array();\n";
        script += "for (int t = 1; t <= tableNames.length; t++) {\n";
        script += "    string tn = string(tableNames[t]);\n";
        if (!tableFilter.empty()) {
            script += "    if (tn != \"" + fsEscape(tableFilter) + "\") continue;\n";
        }
        script += "    Array names = Model.parameters.names(tn);\n";
        script += "    for (int i = 1; i <= names.length; i++) {\n";
        script += "        string nm = string(names[i]);\n";
        script += "        Map entry = Map();\n";
        script += "        entry[\"name\"]  = nm;\n";
        script += "        entry[\"table\"] = tn;\n";
        script += "        entry[\"value\"] = Model.parameters[nm].value;\n";
        script += "        out.push(entry);\n";
        script += "    }\n";
        script += "}\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("list_parameters_failed", e.what()); }
          catch (...) { return returnError("list_parameters_failed", "non-std exception."); }

        nlohmann::json params = nlohmann::json::array();
        if (result.type == VariantType::Array) {
            Array a = static_cast<Array>(result);
            for (int i = 1; i <= a.length; ++i) {
                Variant ev = a[i];
                if (ev.type != VariantType::Map) continue;
                Map m = static_cast<Map>(ev);
                nlohmann::json p;
                p["name"]  = std::string(m["name"]);
                p["table"] = std::string(m["table"]);
                Variant val = m["value"];
                std::string kind;
                switch (val.type) {
                    case VariantType::Number: {
                        double d = static_cast<double>(val);
                        if (d == static_cast<double>(static_cast<long long>(d))
                            && d >= -9.2e18 && d <= 9.2e18) p["value"] = static_cast<long long>(d);
                        else p["value"] = d;
                        kind = "number";
                        break;
                    }
                    case VariantType::String:
                        p["value"] = std::string(val);
                        kind = "string";
                        break;
                    case VariantType::Null:
                        p["value"] = nullptr;
                        kind = "null";
                        break;
                    default:
                        p["value"] = nullptr;
                        kind = "unknown";
                        break;
                }
                p["value_kind"] = kind;
                params.push_back(std::move(p));
            }
        }
        nlohmann::json out;
        out["ok"]              = true;
        out["parameter_count"] = (int)params.size();
        if (!tableFilter.empty()) out["table_filter"] = tableFilter;
        out["parameters"]      = std::move(params);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_parameters", e.what()); }
      catch (...)                     { return returnException("list_parameters", "unknown"); }
}

// ============================================================================
// modelerai_list_classes({ category? }) — every class with an on-disk
// schema, with property count. Optional category substring filter
// matches against any property's category in the class's schema.
//
// This is a SCHEMA-LAYER tool — it reads KNOWLEDGE/schemas/*.json. So
// the answer is "what classes do I have schemas for", not "what classes
// does FlexSim ship". Use for AI self-discovery before create_object /
// set_property when AI isn't sure a class is in scope.
// ============================================================================
modelerai_export Variant ModelerAi_listClasses(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string categoryFilter;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) categoryFilter = j.value("category", std::string(""));
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            }
        }

        std::string installDir = ModelerAi::paths::moduleInstallDir();
        if (installDir.empty()) {
            return returnError("install_dir_unavailable",
                "Could not resolve module install dir; cannot find KNOWLEDGE/schemas/.");
        }
        std::filesystem::path schemaDir = std::filesystem::path(installDir) / "KNOWLEDGE" / "schemas";
        std::error_code ec;
        if (!std::filesystem::exists(schemaDir, ec)) {
            return returnError("schemas_dir_missing",
                "KNOWLEDGE/schemas/ does not exist. Run modelerai_extract_class_schema "
                "via the FlexScript loop to generate them.");
        }

        nlohmann::json classes = nlohmann::json::array();
        for (const auto& entry : std::filesystem::directory_iterator(schemaDir, ec)) {
            if (ec) break;
            if (!entry.is_regular_file()) continue;
            auto p = entry.path();
            if (p.extension() != ".json") continue;
            std::string className = p.stem().string();
            // Replace the schema's file-safe "__" separator back to "::"
            // for module-namespaced classes like AGV::Path.
            for (size_t pos = 0; (pos = className.find("__", pos)) != std::string::npos;) {
                className.replace(pos, 2, "::");
                pos += 2;
            }
            // Resolve through the schema store so the count comes from the
            // canonical parser, not a re-read.
            const auto* schema = ModelerAi::Schema::get(className);
            if (!schema) continue;

            // Optional category filter — keep if ANY property matches.
            if (!categoryFilter.empty()) {
                bool any = false;
                for (const auto& kv : schema->by_name) {
                    if (kv.second.category.find(categoryFilter) != std::string::npos) {
                        any = true; break;
                    }
                }
                if (!any) continue;
            }

            nlohmann::json c;
            c["name"]           = className;
            c["property_count"] = (int)schema->by_name.size();
            if (!schema->extracted_at.empty()) c["extracted_at"] = schema->extracted_at;
            classes.push_back(std::move(c));
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["class_count"] = (int)classes.size();
        if (!categoryFilter.empty()) out["category_filter"] = categoryFilter;
        out["classes"]     = std::move(classes);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_classes", e.what()); }
      catch (...)                     { return returnException("list_classes", "unknown"); }
}

// ============================================================================
// modelerai_get_object_info({ object }) — one-shot characterization of an
// instance. Combines what would otherwise take 4-5 separate calls:
//   - class (via classobject)
//   - path
//   - location / rotation / size (Vec3 readbacks)
//   - parent name + class
//   - group memberships
//   - label count + label names
// Useful as a "tell me about this thing" probe before deciding what
// to modify.
// ============================================================================
modelerai_export Variant ModelerAi_getObjectInfo(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string objectName;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty() && s.front() == '{') {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) objectName = j.value("object", std::string(""));
                } catch (const std::exception& e) {
                    return returnError("bad_args_json", std::string("parse: ") + e.what());
                }
            } else {
                objectName = s;
            }
        }
        if (objectName.empty()) {
            return returnError("missing_args",
                "Pass { object: \"Name\" } or a bare name string.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        nlohmann::json out;
        out["ok"]   = true;
        out["name"] = objectName;

        // Class.
        try {
            TreeNode* cls = classobject(obj);
            if (cls) out["class"] = std::string(getname(cls));
        } catch (...) {}

        // Path.
        try {
            const char* p = nodetomodelpath_cstr(obj, 1);
            if (p) out["path"] = std::string(p);
        } catch (...) {}

        // Location / Rotation / Size. Use the dedicated XYZ accessors
        // so we don't have to go through setProperty plumbing.
        try {
            nlohmann::json loc;
            loc["x"] = xloc(obj); loc["y"] = yloc(obj); loc["z"] = zloc(obj);
            out["location"] = std::move(loc);
        } catch (...) {}
        try {
            nlohmann::json sz;
            sz["x"] = xsize(obj); sz["y"] = ysize(obj); sz["z"] = zsize(obj);
            out["size"] = std::move(sz);
        } catch (...) {}
        try {
            nlohmann::json rot;
            rot["x"] = xrot(obj); rot["y"] = yrot(obj); rot["z"] = zrot(obj);
            out["rotation"] = std::move(rot);
        } catch (...) {}

        // Parent.
        try {
            TreeNode* parent = obj->up;
            if (parent && objectexists(parent)) {
                nlohmann::json p;
                p["name"] = std::string(getname(parent));
                TreeNode* pcls = classobject(parent);
                if (pcls) p["class"] = std::string(getname(pcls));
                out["parent"] = std::move(p);
            }
        } catch (...) {}

        // Group memberships + label count via a one-shot script.
        std::string script;
        script += "Object o = Model.find(\"" + fsEscape(objectName) + "\");\n";
        script += "Map info = Map();\n";
        script += "Array groups = Array();\n";
        script += "treenode tg = model().find(\"Tools/Groups\");\n";
        script += "if (tg) {\n";
        script += "    for (int i = 1; i <= tg.subnodes.length; i++) {\n";
        script += "        treenode g = tg.subnodes[i];\n";
        script += "        string nm = string(getname(g));\n";
        script += "        if (Group(nm).isMember(o, 0)) groups.push(nm);\n";
        script += "    }\n";
        script += "}\n";
        script += "info[\"groups\"]      = groups;\n";
        script += "info[\"label_count\"] = o.labels.length;\n";
        script += "Array labelNames = Array();\n";
        script += "for (int i = 1; i <= o.labels.length; i++) {\n";
        script += "    labelNames.push(string(getname(o.labels[i])));\n";
        script += "}\n";
        script += "info[\"label_names\"] = labelNames;\n";
        script += "info[\"subnode_count\"] = o.subnodes.length;\n";
        script += "return info;\n";

        try {
            Variant infoV = executestring(script.c_str(), nullptr, nullptr, Variant());
            if (infoV.type == VariantType::Map) {
                Map m = static_cast<Map>(infoV);
                Variant gV = m["groups"];
                if (gV.type == VariantType::Array) {
                    nlohmann::json gjs = nlohmann::json::array();
                    Array ga = static_cast<Array>(gV);
                    for (int i = 1; i <= ga.length; ++i) gjs.push_back(std::string(ga[i]));
                    out["groups"] = std::move(gjs);
                }
                out["label_count"]   = (int)static_cast<double>(m["label_count"]);
                out["subnode_count"] = (int)static_cast<double>(m["subnode_count"]);
                Variant lV = m["label_names"];
                if (lV.type == VariantType::Array) {
                    nlohmann::json ljs = nlohmann::json::array();
                    Array la = static_cast<Array>(lV);
                    for (int i = 1; i <= la.length; ++i) ljs.push_back(std::string(la[i]));
                    out["label_names"] = std::move(ljs);
                }
            }
        } catch (...) {
            // soft fail — caller still has the basic info above
        }

        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_object_info", e.what()); }
      catch (...)                     { return returnException("get_object_info", "unknown"); }
}

// modelerai_list_global_tables({}) — every GlobalTable + its dimensions.
modelerai_export Variant ModelerAi_listGlobalTables(FLEXSIMINTERFACE)
{
    try {
        std::string script;
        script += "treenode root = model().find(\"Tools/GlobalTables\");\n";
        script += "Array out = Array();\n";
        script += "if (root) {\n";
        script += "    for (int i = 1; i <= root.subnodes.length; i++) {\n";
        script += "        treenode t = root.subnodes[i];\n";
        script += "        string nm = string(getname(t));\n";
        script += "        Map entry = Map();\n";
        script += "        entry[\"name\"]     = nm;\n";
        script += "        entry[\"num_rows\"] = Table(nm).numRows;\n";
        script += "        entry[\"num_cols\"] = Table(nm).numCols;\n";
        script += "        out.push(entry);\n";
        script += "    }\n";
        script += "}\n";
        script += "return out;\n";

        Variant result;
        try {
            result = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) { return returnError("list_tables_failed", e.what()); }
          catch (...) { return returnError("list_tables_failed", "non-std exception."); }

        nlohmann::json tables = nlohmann::json::array();
        if (result.type == VariantType::Array) {
            Array a = static_cast<Array>(result);
            for (int i = 1; i <= a.length; ++i) {
                Variant ev = a[i];
                if (ev.type != VariantType::Map) continue;
                Map m = static_cast<Map>(ev);
                nlohmann::json e;
                e["name"]     = std::string(m["name"]);
                e["num_rows"] = (int)static_cast<double>(m["num_rows"]);
                e["num_cols"] = (int)static_cast<double>(m["num_cols"]);
                tables.push_back(std::move(e));
            }
        }
        nlohmann::json out;
        out["ok"]          = true;
        out["table_count"] = (int)tables.size();
        out["tables"]      = std::move(tables);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_global_tables", e.what()); }
      catch (...)                     { return returnException("list_global_tables", "unknown"); }
}

// ============================================================================
// Tools-family delete wrappers — close the asymmetry where parameter and
// performance-measure tools shipped with delete tools but Group and
// GlobalTable didn't, which pushed AI to run_script for those deletions.
//
//   modelerai_delete_group         — destroys a Group node + its membership
//   modelerai_delete_global_table  — destroys a GlobalTable node
//
// Both go through destroyobject() — same path FlexSim's UI delete uses.
// ============================================================================

namespace {
// Shared body. The tool param is `name` (or a bare name string). Returns
// `not_found` if Tools.get returns null. Tool-type label appears in the
// response so the AI can echo "Group 'Operators' deleted" / "GlobalTable
// 'Recipes' deleted" without re-parsing.
Variant deleteToolByName(const Variant& arg,
                          const char* toolType,
                          const char* toolName,
                          const char* notFoundCode,
                          const char* failCode)
{
    std::string name;
    if (arg.type == VariantType::String) {
        std::string s(arg);
        if (!s.empty() && s.front() == '{') {
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_object()) name = j.value("name", std::string(""));
            } catch (const std::exception& e) {
                return returnError("bad_args_json", std::string("parse: ") + e.what());
            }
        } else {
            name = s;
        }
    }
    if (name.empty()) {
        return returnError("missing_name",
            std::string("Pass { name: \"X\" } or a bare name string for ") + toolName + ".");
    }

    std::string script;
    script += "treenode t = Tools.get(\"" + std::string(toolType) + "\", \"" + fsEscape(name) + "\");\n";
    script += "if (!t) return -1;\n";
    script += "destroyobject(t);\n";
    script += "return 1;\n";

    Variant result;
    try {
        result = executestring(script.c_str(), nullptr, nullptr, Variant());
    } catch (const std::exception& e) { return returnError(failCode, e.what()); }
      catch (...) { return returnError(failCode, "non-std exception."); }

    if (result.type == VariantType::Number && static_cast<double>(result) < 0) {
        nlohmann::json err;
        err["ok"]            = false;
        err["error_code"]    = notFoundCode;
        err["error_message"] = std::string(toolName) + " '" + name + "' does not exist.";
        err["name"]          = name;
        return returnJson(err);
    }
    nlohmann::json out;
    out["ok"]      = true;
    out["deleted"] = true;
    out["name"]    = name;
    out["type"]    = toolType;
    return returnJson(out);
}
} // namespace

modelerai_export Variant ModelerAi_deleteGroup(FLEXSIMINTERFACE)
{
    try {
        return deleteToolByName(param(1), "Group", "Group",
                                 "group_not_found", "delete_group_failed");
    } catch (const std::exception& e) { return returnException("delete_group", e.what()); }
      catch (...)                     { return returnException("delete_group", "unknown"); }
}

modelerai_export Variant ModelerAi_deleteGlobalTable(FLEXSIMINTERFACE)
{
    try {
        return deleteToolByName(param(1), "GlobalTable", "GlobalTable",
                                 "table_not_found", "delete_table_failed");
    } catch (const std::exception& e) { return returnException("delete_global_table", e.what()); }
      catch (...)                     { return returnException("delete_global_table", "unknown"); }
}

// ============================================================================
// ============================================================================
//
//   SHELLS — PENDING CONTEXT
//
//   The functions below are registered (in .fsx + here) so the AI can
//   discover them via modelerai_call's introspection, but they all return
//   err:not_implemented today. Each carries a TODO block listing the
//   FlexSim-specific context we need from the user before filling in the
//   body. Going through them one at a time with the user; each one becomes
//   a real implementation in its own commit.
//
//   When you fill one in, replace its stub body with the real logic AND
//   remove the SHELL marker from its KB topic at
//   KNOWLEDGE/topics/modelerai/<name>.md.
//
// ============================================================================
// ============================================================================

// ----------------------------------------------------------------------------
// modelerai_set_trigger({ object, trigger, code })  — v1
//
// Sets a trigger (OnReset / OnEntry / OnExit / OnMessage / OnDraw /
// OnPreDraw / OnStateChange / etc.) on a FlexSim object. Writes the
// canonical `codeHeader` (the per-event FlexScript variable declarations
// like `Object current = ownerobject(c); Object item = param(1);`) and
// then the caller's body so the AI's code sees the expected named
// parameters (`current`, `item`, `port`, etc.).
//
// Implementation:
//   1. Resolve object via Model.find.
//   2. Enumerate events on the object with
//        function_s(obj, "enumerateEvents", tmp, 8)
//      where 8 == EVENT_TYPE_TRIGGER. Walk the temp node, look up the
//      requested trigger name. If not found, error with the list of
//      available trigger names so the AI can correct itself in one round.
//   3. Read the `codeHeader` value off the matching entry.
//   4. Call function_s(obj, "assertEventWithCode", trigger_name)
//      to get-or-create the per-instance storage treenode for this
//      trigger. (Per Josh: this is the canonical "create the event"
//      entry point — idempotent on already-existing events.)
//   5. setnodestr the storage node to `codeHeader + user_code`.
//   6. Return the storage path + the final stored content so the AI can
//      confirm what was written.
//
// ============================================================================
// PICKLISTS — DEFERRED TO V2.  See the design note above
// ModelerAi_setTrigger (and the matching one above ModelerAi_setProperty)
// for the broader picks-as-a-cross-cutting-concern story. The short
// version: for v1, set_trigger takes raw FlexScript only. The AI is
// expected to learn the common pick-option code patterns from the picklist
// VIEW nodes (it can search_kb on them) and reproduce them as raw code.
// V2 will add a `pick_option` arg that resolves a named pick from the
// class-level <Class>>behaviour/eventInfo/<event>/variables/picklist
// coupling, copies its code into the storage node, and round-trips the
// pick name back via get_trigger. Don't add the picks arg in v1 — wait
// until we've inspected the GUI's actual storage shape after picking,
// since that decides whether picks store "resolved code + remembered
// pick name" vs "live coupling to the pick definition".
// ============================================================================
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_setTrigger(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_trigger expects { object, trigger, code } JSON.");
        }
        std::string objectName, triggerName, code;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName  = j.value("object",  std::string(""));
            triggerName = j.value("trigger", std::string(""));
            code        = j.value("code",    std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || triggerName.empty()) {
            return returnError("missing_args",
                "object and trigger are required.");
        }

        // Scan the trigger body for hallucination / freeze-risk patterns
        // BEFORE writing it to the tree. A trigger body is hotter than a
        // one-shot run_script call — it fires on every event, so a
        // `while (1)` inside an OnEntry locks the model the first time
        // an item arrives. Reuse run_script's antipattern catalog so the
        // remediation message is consistent across tools.
        if (!code.empty()) {
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(code, anName, anRem)) {
                nlohmann::json err;
                err["ok"]            = false;
                err["error_code"]    = "antipattern_in_trigger_code";
                err["error_message"] = "Trigger body contains a forbidden pattern (" + anName +
                                       "). Refuse to write it — FlexScript has no try/catch and "
                                       "a bad trigger body freezes FlexSim.";
                err["antipattern"]   = anName;
                err["remediation"]   = anRem;
                err["object"]        = objectName;
                err["trigger"]       = triggerName;
                return returnJson(err);
            }
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Build a FlexScript script that does the whole flow in one
        // shot — enumerate, validate, codeHeader lookup, assertEventWithCode,
        // write. Mirrors the established pattern in set_label / add_parameter.
        std::string script;

        // --- find object + create scratch ---
        // Scratch lives under MAIN:/project/exec/globals (per-process, not
        // saved with the model). Asserted under a clearly-namespaced name so
        // a leftover scratch from a previous failed call doesn't leak data.
        script += "Object obj = Model.find(\"";
        script += fsEscape(objectName);
        script += "\");\n";
        script += "treenode globals = node(\"MAIN:/project/exec/globals\");\n";
        script += "treenode tmp = globals.subnodes.assert(\"__mai_set_trigger_tmp__\");\n";
        // Bounded purge — FlexScript has no try/catch, so an unbounded
        // `while (x.subnodes.length) x.subnodes[1].destroy();` will freeze
        // the engine if destroy() ever fails to decrement. Cap at 4096 —
        // any object with that many events is already pathological.
        script += "for (int __pX = 0; __pX < 4096 && tmp.subnodes.length; __pX++) tmp.subnodes[1].destroy();\n";

        // Enumerate events without a type filter. Earlier we passed `8`
        // assuming EVENT_TYPE_TRIGGER, but that returned 0 entries in
        // practice on Source/Queue/Processor — either the constant differs
        // from 8 in this FlexSim build or `enumerateEvents` interprets the
        // 4th arg differently than `EVENT_TYPE_*`. The 3-arg form returns
        // every declared event on the object's class; we match by name
        // and don't care about its event-type category.
        script += "function_s(obj, \"enumerateEvents\", tmp);\n";

        // Locate the matching entry; collect available names as a fallback
        // payload for the not-found case. Scripts ALWAYS return an Array so
        // executestring never lands in the C++ "expected Array return"
        // branch — first slot is a status string ("ok" / "not_found").
        script += "treenode match = 0;\n";
        script += "string header = \"\";\n";
        script += "string availableNames = \"\";\n";
        script += "for (int iX = 1; iX <= tmp.subnodes.length; iX++) {\n";
        script += "    treenode entry = tmp.subnodes[iX];\n";
        script += "    treenode nameNode = entry.find(\"name\");\n";
        script += "    if (!nameNode) continue;\n";
        script += "    string evName = nameNode.value;\n";
        script += "    if (availableNames.length) availableNames += \", \";\n";
        script += "    availableNames += evName;\n";
        script += "    if (evName == \""; script += fsEscape(triggerName); script += "\") {\n";
        script += "        match = entry;\n";
        script += "        treenode hdrNode = entry.find(\"codeHeader\");\n";
        script += "        if (hdrNode) header = hdrNode.value;\n";
        script += "    }\n";
        script += "}\n";

        script += "Array ret = Array();\n";
        script += "if (!match) {\n";
        script += "    ret.push(\"not_found\");\n";
        script += "    ret.push(availableNames);\n";
        script += "    ret.push(tmp.subnodes.length);\n";
        script += "    tmp.destroy();\n";
        script += "    return ret;\n";
        script += "}\n";

        script += "treenode storage = function_s(obj, \"assertEventWithCode\", \"";
        script += fsEscape(triggerName);
        script += "\");\n";

        // codeHeader strings end with \n; just concatenate.
        script += "string finalCode = header + \"";
        script += fsEscape(code);
        script += "\";\n";
        script += "storage.value = finalCode;\n";

        script += "ret.push(\"ok\");\n";
        script += "ret.push(nodetopath(storage));\n";
        script += "ret.push(finalCode);\n";
        script += "ret.push(header);\n";
        script += "tmp.destroy();\n";
        script += "return ret;\n";

        Variant scriptResult;
        try {
            scriptResult = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("set_trigger_failed",
                "Setting trigger '" + triggerName + "' on '" + objectName +
                "' threw: " + (e.what() ? e.what() : ""));
        } catch (...) {
            return returnError("set_trigger_failed",
                "Setting trigger '" + triggerName + "' threw a non-std exception.");
        }

        if (scriptResult.type != VariantType::Array) {
            return returnError("set_trigger_failed",
                "Internal: script returned " +
                std::to_string(static_cast<int>(scriptResult.type)) +
                " instead of Array. enumerateEvents may have failed on this object.");
        }
        Array ret = (Array)scriptResult;
        std::string status = ret.length >= 1 ? (std::string)(ret[1].toString()) : "";
        if (status == "not_found") {
            std::string availableNames = ret.length >= 2 ? (std::string)(ret[2].toString()) : "";
            int enumCount             = ret.length >= 3 ? static_cast<int>(static_cast<double>(ret[3])) : 0;
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(enumCount) + " entries. "
                "Available: " + availableNames);
        }
        std::string storagePath  = ret.length >= 2 ? (std::string)(ret[2].toString()) : "";
        std::string finalCode    = ret.length >= 3 ? (std::string)(ret[3].toString()) : "";
        std::string headerOnly   = ret.length >= 4 ? (std::string)(ret[4].toString()) : "";

        nlohmann::json out;
        out["ok"]            = true;
        out["object"]        = objectName;
        out["trigger"]       = triggerName;
        out["storage_path"]  = storagePath;
        out["code_header"]   = headerOnly;
        out["stored_code"]   = finalCode;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_trigger", e.what()); }
      catch (...)                     { return returnException("set_trigger", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_get_trigger({ object, trigger })  — v1
//
// Returns the currently-stored FlexScript on a named trigger of a named
// object. Surfaces:
//   - `set`            : false if the trigger has no per-instance storage
//                        (the `reference` coupling on the enumerator entry
//                        is null), true if storage exists.
//   - `code_header`    : the canonical declarations FlexSim prepends.
//   - `stored_code`    : exactly what's in the storage node (header + body).
//   - `user_code`      : `stored_code` with the header prefix stripped off
//                        so the modeler/AI sees just the body they would
//                        have written. If the stored code somehow doesn't
//                        start with the header (e.g. legacy hand-edits),
//                        `user_code` == `stored_code` and `header_match`
//                        is false.
//   - `storage_path`   : path to the storage node (empty string when unset).
//
// Pick-option detection deferred to v2 — see the DESIGN NOTE above
// ModelerAi_setTrigger. For v1 the AI gets raw stored code only.
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_getTrigger(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_trigger expects { object, trigger } JSON.");
        }
        std::string objectName, triggerName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName  = j.value("object",  std::string(""));
            triggerName = j.value("trigger", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || triggerName.empty()) {
            return returnError("missing_args", "object and trigger are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        std::string script;
        script += "Object obj = Model.find(\""; script += fsEscape(objectName); script += "\");\n";
        script += "treenode globals = node(\"MAIN:/project/exec/globals\");\n";
        script += "treenode tmp = globals.subnodes.assert(\"__mai_get_trigger_tmp__\");\n";
        // Bounded purge — FlexScript has no try/catch, so an unbounded
        // `while (x.subnodes.length) x.subnodes[1].destroy();` will freeze
        // the engine if destroy() ever fails to decrement. Cap at 4096 —
        // any object with that many events is already pathological.
        script += "for (int __pX = 0; __pX < 4096 && tmp.subnodes.length; __pX++) tmp.subnodes[1].destroy();\n";
        // No type filter — see comment in set_trigger above.
        script += "function_s(obj, \"enumerateEvents\", tmp);\n";

        script += "treenode match = 0;\n";
        script += "string header = \"\";\n";
        script += "string availableNames = \"\";\n";
        script += "treenode reference = 0;\n";
        script += "for (int iX = 1; iX <= tmp.subnodes.length; iX++) {\n";
        script += "    treenode entry = tmp.subnodes[iX];\n";
        script += "    treenode nameNode = entry.find(\"name\");\n";
        script += "    if (!nameNode) continue;\n";
        script += "    string evName = nameNode.value;\n";
        script += "    if (availableNames.length) availableNames += \", \";\n";
        script += "    availableNames += evName;\n";
        script += "    if (evName == \""; script += fsEscape(triggerName); script += "\") {\n";
        script += "        match = entry;\n";
        script += "        treenode hdrNode = entry.find(\"codeHeader\");\n";
        script += "        if (hdrNode) header = hdrNode.value;\n";
        script += "        reference = entry.find(\"reference\");\n";
        script += "    }\n";
        script += "}\n";

        script += "Array ret = Array();\n";
        script += "if (!match) {\n";
        script += "    ret.push(\"not_found\");\n";
        script += "    ret.push(availableNames);\n";
        script += "    ret.push(tmp.subnodes.length);\n";
        script += "    tmp.destroy();\n";
        script += "    return ret;\n";
        script += "}\n";

        // Coupling: `.value` on a DATA_POINTERCOUPLING node returns the
        // target node, or nullvar when there's no target.
        script += "treenode storage = 0;\n";
        script += "if (reference) storage = reference.value;\n";

        script += "int isSet = 0;\n";
        script += "string storagePath = \"\";\n";
        script += "string storedCode = \"\";\n";
        script += "string userCode = \"\";\n";
        script += "int headerMatch = 0;\n";
        script += "if (storage) {\n";
        script += "    isSet = 1;\n";
        script += "    storagePath = nodetopath(storage);\n";
        script += "    storedCode = storage.value;\n";
        script += "    if (header.length > 0 && storedCode.startsWith(header)) {\n";
        script += "        headerMatch = 1;\n";
        script += "        userCode = storedCode.substr(header.length + 1, storedCode.length - header.length);\n";
        script += "    } else {\n";
        script += "        userCode = storedCode;\n";
        script += "    }\n";
        script += "}\n";

        script += "ret.push(\"ok\");\n";
        script += "ret.push(isSet);\n";
        script += "ret.push(storagePath);\n";
        script += "ret.push(header);\n";
        script += "ret.push(storedCode);\n";
        script += "ret.push(userCode);\n";
        script += "ret.push(headerMatch);\n";
        script += "tmp.destroy();\n";
        script += "return ret;\n";

        Variant scriptResult;
        try {
            scriptResult = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("get_trigger_failed",
                "Getting trigger '" + triggerName + "' on '" + objectName + "' threw: " +
                (e.what() ? e.what() : ""));
        } catch (...) {
            return returnError("get_trigger_failed",
                "Getting trigger '" + triggerName + "' threw a non-std exception.");
        }

        if (scriptResult.type != VariantType::Array) {
            return returnError("get_trigger_failed",
                "Internal: script returned " +
                std::to_string(static_cast<int>(scriptResult.type)) +
                " instead of Array.");
        }
        Array ret = (Array)scriptResult;
        std::string status = ret.length >= 1 ? (std::string)(ret[1].toString()) : "";
        if (status == "not_found") {
            std::string availableNames = ret.length >= 2 ? (std::string)(ret[2].toString()) : "";
            int enumCount             = ret.length >= 3 ? static_cast<int>(static_cast<double>(ret[3])) : 0;
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(enumCount) + " entries. "
                "Available: " + availableNames);
        }
        int isSet              = ret.length >= 2 ? static_cast<int>(static_cast<double>(ret[2])) : 0;
        std::string storagePath = ret.length >= 3 ? (std::string)(ret[3].toString()) : "";
        std::string headerOnly  = ret.length >= 4 ? (std::string)(ret[4].toString()) : "";
        std::string storedCode  = ret.length >= 5 ? (std::string)(ret[5].toString()) : "";
        std::string userCode    = ret.length >= 6 ? (std::string)(ret[6].toString()) : "";
        int headerMatch         = ret.length >= 7 ? static_cast<int>(static_cast<double>(ret[7])) : 0;

        nlohmann::json out;
        out["ok"]            = true;
        out["object"]        = objectName;
        out["trigger"]       = triggerName;
        out["set"]           = isSet != 0;
        out["storage_path"]  = storagePath;
        out["code_header"]   = headerOnly;
        out["stored_code"]   = storedCode;
        out["user_code"]     = userCode;
        out["header_match"]  = headerMatch != 0;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_trigger", e.what()); }
      catch (...)                     { return returnException("get_trigger", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_list_triggers({ object })  — v1
//
// Enumerate every trigger declared on the object's class (via the
// EVENT_TYPE_TRIGGER=8 filter on enumerateEvents). Returns one entry per
// trigger with the metadata the AI needs to write code against it:
//   - name           : trigger name (e.g. "OnExit", "OnEntry")
//   - code_header    : canonical declaration block (params already in scope)
//   - params[]       : array of { title, name, type } so the AI knows the
//                      named parameters available in the body
//   - set            : true if per-instance storage exists (reference
//                      coupling resolves to a node)
//   - storage_path   : storage node path (empty when unset)
//
// The trigger's body is NOT included in the listing — that's what
// `modelerai_get_trigger` is for. List is the cheap index; get is the
// detail fetch.
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_listTriggers(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string objectName;
        if (arg.type == VariantType::String) {
            // Accept either a raw object name OR a JSON { object } payload.
            std::string s = (std::string)arg;
            size_t firstNonSpace = s.find_first_not_of(" \t\r\n");
            bool looksLikeJson = firstNonSpace != std::string::npos && s[firstNonSpace] == '{';
            if (looksLikeJson) {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) objectName = j.value("object", std::string(""));
                } catch (...) {}
            } else {
                objectName = s;
            }
        }
        if (objectName.empty()) {
            return returnError("missing_args",
                "modelerai_list_triggers expects an object name or { object } JSON.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        // Enumerate to a scratch node, then walk it from C++ — we want
        // per-entry param sub-arrays which is awkward to round-trip as a
        // single FlexScript Array. C++ navigation keeps the structure clean.
        std::string script;
        script += "Object obj = Model.find(\""; script += fsEscape(objectName); script += "\");\n";
        script += "treenode globals = node(\"MAIN:/project/exec/globals\");\n";
        script += "treenode tmp = globals.subnodes.assert(\"__mai_list_triggers_tmp__\");\n";
        // Bounded purge — FlexScript has no try/catch, so an unbounded
        // `while (x.subnodes.length) x.subnodes[1].destroy();` will freeze
        // the engine if destroy() ever fails to decrement. Cap at 4096 —
        // any object with that many events is already pathological.
        script += "for (int __pX = 0; __pX < 4096 && tmp.subnodes.length; __pX++) tmp.subnodes[1].destroy();\n";
        // No type filter — earlier `, 8)` (assuming EVENT_TYPE_TRIGGER=8)
        // returned zero entries on every FixedResource. Return everything;
        // the AI uses event names directly.
        script += "function_s(obj, \"enumerateEvents\", tmp);\n";
        script += "return nodetopath(tmp);\n";

        Variant scriptResult;
        try {
            scriptResult = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("list_triggers_failed",
                std::string("enumerateEvents threw: ") + (e.what() ? e.what() : ""));
        } catch (...) {
            return returnError("list_triggers_failed",
                "enumerateEvents threw a non-std exception.");
        }
        std::string tmpPath = (std::string)scriptResult.toString();
        TreeNode* tmp = nullptr;
        try { tmp = model()->find(tmpPath.c_str()); } catch (...) {}
        if (!tmp) {
            try { tmp = node(tmpPath.c_str(), nullptr); } catch (...) {}
        }
        if (!tmp) {
            return returnError("list_triggers_failed",
                "Could not resolve scratch node at '" + tmpPath + "' after enumeration.");
        }

        nlohmann::json triggers = nlohmann::json::array();
        int entryCount = 0;
        try { entryCount = (int)tmp->subnodes.length; } catch (...) {}
        for (int i = 1; i <= entryCount; ++i) {
            TreeNode* entry = nullptr;
            try { entry = tmp->subnodes[i]; } catch (...) {}
            if (!entry) continue;

            std::string name, header;
            TreeNode* nameNode = nullptr;
            try { nameNode = entry->find("name"); } catch (...) {}
            if (nameNode) {
                try { name = (std::string)(nameNode->value.toString()); } catch (...) {}
            }
            if (name.empty()) continue;

            TreeNode* hdrNode = nullptr;
            try { hdrNode = entry->find("codeHeader"); } catch (...) {}
            if (hdrNode) {
                try { header = (std::string)(hdrNode->value.toString()); } catch (...) {}
            }

            // params: each subnode = one param with Title/Name/Type subnodes
            nlohmann::json params = nlohmann::json::array();
            TreeNode* paramsNode = nullptr;
            try { paramsNode = entry->find("params"); } catch (...) {}
            if (paramsNode) {
                int pn = 0;
                try { pn = (int)paramsNode->subnodes.length; } catch (...) {}
                for (int p = 1; p <= pn; ++p) {
                    TreeNode* pEntry = nullptr;
                    try { pEntry = paramsNode->subnodes[p]; } catch (...) {}
                    if (!pEntry) continue;
                    auto readSub = [&](const char* key) -> std::string {
                        TreeNode* sub = nullptr;
                        try { sub = pEntry->find(key); } catch (...) {}
                        if (!sub) return "";
                        try { return (std::string)(sub->value.toString()); } catch (...) { return ""; }
                    };
                    std::string title = readSub("Title");
                    std::string pname = readSub("Name");
                    std::string ptype = readSub("Type");
                    // If lookup by key failed (e.g. unnamed subnode pattern),
                    // fall back to positional reads on subnodes[1..3].
                    if (title.empty() && pname.empty() && ptype.empty()) {
                        int sn = 0;
                        try { sn = (int)pEntry->subnodes.length; } catch (...) {}
                        auto readPos = [&](int idx) -> std::string {
                            if (idx < 1 || idx > sn) return "";
                            TreeNode* s = nullptr;
                            try { s = pEntry->subnodes[idx]; } catch (...) {}
                            if (!s) return "";
                            try { return (std::string)(s->value.toString()); } catch (...) { return ""; }
                        };
                        title = readPos(1);
                        pname = readPos(2);
                        ptype = readPos(3);
                    }
                    nlohmann::json pj;
                    pj["title"] = title;
                    pj["name"]  = pname;
                    pj["type"]  = ptype;
                    params.push_back(pj);
                }
            }

            // reference coupling — does the trigger have per-instance storage?
            TreeNode* refNode = nullptr;
            try { refNode = entry->find("reference"); } catch (...) {}
            TreeNode* storage = couplingTarget(refNode);
            bool isSet = storage != nullptr;
            std::string storagePath;
            if (storage) {
                try {
                    const char* p = nodetomodelpath_cstr(storage, 1);
                    if (p) storagePath = p;
                } catch (...) {}
            }

            nlohmann::json t;
            t["name"]         = name;
            t["code_header"]  = header;
            t["params"]       = params;
            t["set"]          = isSet;
            t["storage_path"] = storagePath;
            triggers.push_back(t);
        }

        // Clean up the scratch so a leftover doesn't leak data on next call.
        try { tmp->destroy(); } catch (...) {}

        nlohmann::json out;
        out["ok"]       = true;
        out["object"]   = objectName;
        out["count"]    = triggers.size();
        out["triggers"] = triggers;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_triggers", e.what()); }
      catch (...)                     { return returnException("list_triggers", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_remove_trigger({ object, trigger })  — v1
//
// Idempotent. Looks up the trigger via enumerateEvents, follows the
// `reference` coupling to the per-instance storage node, and destroys it
// — the inverse of `assertEventWithCode`. After this, a re-enumeration
// shows `reference` as a null coupling again (the "unset" shape from the
// dump).
//
// Returns:
//   { ok: true, object, trigger, was_set, removed }
//     - was_set: whether the trigger had storage before the call
//     - removed: whether this call actually destroyed the storage (false
//                if was_set was already false — idempotent no-op path)
//
// If the trigger name doesn't enumerate, returns err:trigger_not_found
// with the available names (same shape as set_trigger / get_trigger).
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_removeTrigger(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_remove_trigger expects { object, trigger } JSON.");
        }
        std::string objectName, triggerName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            objectName  = j.value("object",  std::string(""));
            triggerName = j.value("trigger", std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (objectName.empty() || triggerName.empty()) {
            return returnError("missing_args", "object and trigger are required.");
        }

        TreeNode* obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        std::string script;
        script += "Object obj = Model.find(\""; script += fsEscape(objectName); script += "\");\n";
        script += "treenode globals = node(\"MAIN:/project/exec/globals\");\n";
        script += "treenode tmp = globals.subnodes.assert(\"__mai_remove_trigger_tmp__\");\n";
        // Bounded purge — FlexScript has no try/catch, so an unbounded
        // `while (x.subnodes.length) x.subnodes[1].destroy();` will freeze
        // the engine if destroy() ever fails to decrement. Cap at 4096 —
        // any object with that many events is already pathological.
        script += "for (int __pX = 0; __pX < 4096 && tmp.subnodes.length; __pX++) tmp.subnodes[1].destroy();\n";
        // No type filter — see set_trigger above.
        script += "function_s(obj, \"enumerateEvents\", tmp);\n";

        script += "treenode match = 0;\n";
        script += "string availableNames = \"\";\n";
        script += "treenode reference = 0;\n";
        script += "for (int iX = 1; iX <= tmp.subnodes.length; iX++) {\n";
        script += "    treenode entry = tmp.subnodes[iX];\n";
        script += "    treenode nameNode = entry.find(\"name\");\n";
        script += "    if (!nameNode) continue;\n";
        script += "    string evName = nameNode.value;\n";
        script += "    if (availableNames.length) availableNames += \", \";\n";
        script += "    availableNames += evName;\n";
        script += "    if (evName == \""; script += fsEscape(triggerName); script += "\") {\n";
        script += "        match = entry;\n";
        script += "        reference = entry.find(\"reference\");\n";
        script += "    }\n";
        script += "}\n";

        script += "Array ret = Array();\n";
        script += "if (!match) {\n";
        script += "    ret.push(\"not_found\");\n";
        script += "    ret.push(availableNames);\n";
        script += "    ret.push(tmp.subnodes.length);\n";
        script += "    tmp.destroy();\n";
        script += "    return ret;\n";
        script += "}\n";

        script += "treenode storage = 0;\n";
        script += "if (reference) storage = reference.value;\n";
        script += "int wasSet = storage ? 1 : 0;\n";
        script += "int removed = 0;\n";
        script += "if (storage) { storage.destroy(); removed = 1; }\n";

        script += "ret.push(\"ok\");\n";
        script += "ret.push(wasSet);\n";
        script += "ret.push(removed);\n";
        script += "tmp.destroy();\n";
        script += "return ret;\n";

        Variant scriptResult;
        try {
            scriptResult = executestring(script.c_str(), nullptr, nullptr, Variant());
        } catch (const std::exception& e) {
            return returnError("remove_trigger_failed",
                "Removing trigger '" + triggerName + "' on '" + objectName + "' threw: " +
                (e.what() ? e.what() : ""));
        } catch (...) {
            return returnError("remove_trigger_failed",
                "Removing trigger '" + triggerName + "' threw a non-std exception.");
        }

        if (scriptResult.type != VariantType::Array) {
            return returnError("remove_trigger_failed",
                "Internal: script returned " +
                std::to_string(static_cast<int>(scriptResult.type)) +
                " instead of Array.");
        }
        Array ret = (Array)scriptResult;
        std::string status = ret.length >= 1 ? (std::string)(ret[1].toString()) : "";
        if (status == "not_found") {
            std::string availableNames = ret.length >= 2 ? (std::string)(ret[2].toString()) : "";
            int enumCount             = ret.length >= 3 ? static_cast<int>(static_cast<double>(ret[3])) : 0;
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(enumCount) + " entries. "
                "Available: " + availableNames);
        }
        int wasSet  = ret.length >= 2 ? static_cast<int>(static_cast<double>(ret[2])) : 0;
        int removed = ret.length >= 3 ? static_cast<int>(static_cast<double>(ret[3])) : 0;

        nlohmann::json out;
        out["ok"]      = true;
        out["object"]  = objectName;
        out["trigger"] = triggerName;
        out["was_set"] = wasSet != 0;
        out["removed"] = removed != 0;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("remove_trigger", e.what()); }
      catch (...)                     { return returnException("remove_trigger", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_find_objects({ name_pattern?, class?, in_group?, parent_path?,
//                          recursive? })
//
// Search the active model for objects matching the criteria. Returns
// `[{ path, name, class }, ...]`. Read-only.
//
// TODO — needs context from user before implementing:
//   1. Name-pattern semantics: glob (`Source*`) or substring or regex or
//      exact? The slash-command UI uses substring. Standard FlexSim
//      Model.find takes exact-path strings — we'd need our own walker.
//   2. Class match: by short class name only ("Source"), or also fully
//      qualified ("ProcessFlow::Source")? Probably both.
//   3. `in_group`: group by Tools name (and possibly nested groups)?
//   4. `parent_path`: a tree path to start the walk from (e.g.
//      "Model/Area1"). If absent, default to Model root.
//   5. `recursive`: depth-first into subobjects? Default true is safer
//      but slower on big models. Cap recursion?
//   6. Cap result size — what's a reasonable upper bound? 200?
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_findObjects(FLEXSIMINTERFACE)
{
    try {
        return returnError("not_implemented",
            "modelerai_find_objects is shelled but not yet implemented. "
            "See the TODO block above ModelerAi_findObjects in commands.cpp.");
    } catch (const std::exception& e) { return returnException("find_objects", e.what()); }
      catch (...)                     { return returnException("find_objects", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_validate_model({ checks?: ["orphans", "undefined_process_times",
//                                       "disconnected_sources", ...] })
//
// Run a set of read-only sanity checks across the active model.
// Returns `[{ check, severity: "warn"|"error", path, message }, ...]`.
//
// TODO — needs context from user before implementing:
//   1. What's the MVP check list? My proposed first set:
//        - "orphans": objects in Tools but never referenced in 3D
//        - "disconnected_sources": Sources with no output port wired
//        - "disconnected_sinks": Sinks with no input port wired
//        - "undefined_process_times": Processors with empty processTime
//        - "no_arrival_schedule": Sources with neither inter-arrival
//          nor schedule
//        - "infinite_capacity_warning": Queues with capacity==MAXINT
//      Do you want any of those dropped, or others added?
//   2. Some checks (like "undefined" or "infinite") are warnings, not
//      errors. Severity levels: error / warn / info — sound right?
//   3. Should we batch-cap output? e.g. "show first 50 issues"?
//   4. Some checks need to know what classes count as "Source", "Sink",
//      "Processor" etc. — that's reasonable to hard-code OR let the
//      caller pass class lists. Hardcode for v1?
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_validateModel(FLEXSIMINTERFACE)
{
    try {
        return returnError("not_implemented",
            "modelerai_validate_model is shelled but not yet implemented. "
            "See the TODO block above ModelerAi_validateModel in commands.cpp.");
    } catch (const std::exception& e) { return returnException("validate_model", e.what()); }
      catch (...)                     { return returnException("validate_model", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_create_statistics_collector({ name, event_source, columns,
//                                          when?, where? })
//
// Create a StatisticsCollector tool. The biggest of the pending tools —
// has the most knobs. Worth a proper scope conversation before shaping.
//
// TODO — needs context from user before implementing:
//   1. What's the MVP arg shape? Hypothesis:
//        { name: "MyCollector",
//          event_source: { object: "Processor1", event: "OnExit" },
//          columns: [ { name: "TimeOut", value: "Model.time" },
//                     { name: "ItemName", value: "item.name" } ],
//          when: "<flexscript>",    // optional "filter" expression
//          where: "<flexscript>" }  // optional row-grouping expression
//      Is this close, or does the GUI's column schema have more required
//      knobs?
//   2. Event source: just one event per collector, or multiple events
//      stacked? GUI allows multiple — should we?
//   3. Column types: number/string/object — autodetect from the value
//      expression, or explicit type in args?
//   4. Auto-attach to a Dashboard chart? Or leave that as a separate
//      command? (Probably separate — keep this tool single-purpose.)
//   5. What FlexSim app command actually creates a StatisticsCollector
//      with the same shape the GUI produces? Need to inspect a
//      GUI-created one's tree shape to match it exactly.
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_createStatisticsCollector(FLEXSIMINTERFACE)
{
    try {
        return returnError("not_implemented",
            "modelerai_create_statistics_collector is shelled but not yet implemented. "
            "See the TODO block above ModelerAi_createStatisticsCollector in commands.cpp.");
    } catch (const std::exception& e) { return returnException("create_statistics_collector", e.what()); }
      catch (...)                     { return returnException("create_statistics_collector", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_create_tracked_variable({ name, initial_value? })
//
// Create a TrackedVariable tool. Simpler than StatisticsCollector but
// still needs an arg-shape and SDK-exposure check.
//
// TODO — needs context from user before implementing:
//   1. Where do TrackedVariables get created in the tree? Under
//      Tools/TrackedVariables, same shape as a label-Tracked-Variable
//      under a node-label? Or distinct?
//   2. Initial value: just a number, or also "tracked over time" + chart
//      attachment from the start? Probably plain number for v1.
//   3. Are there variants — Time-Weighted vs Sample? If yes, expose as
//      an enum in args.
//   4. Does the GUI's "New TrackedVariable" execute a specific
//      app command we should mirror, or is it just createcopy(library
//      template, Tools)?
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_createTrackedVariable(FLEXSIMINTERFACE)
{
    try {
        return returnError("not_implemented",
            "modelerai_create_tracked_variable is shelled but not yet implemented. "
            "See the TODO block above ModelerAi_createTrackedVariable in commands.cpp.");
    } catch (const std::exception& e) { return returnException("create_tracked_variable", e.what()); }
      catch (...)                     { return returnException("create_tracked_variable", "unknown"); }
}
