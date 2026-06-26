/**Custom Code*/
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

// ProcessFlow module headers (linked via ProcessFlow.lib) — for native activity
// creation via the ProcessFlow::createObject class method. Painter.h (pulled in
// by ProcessFlow.h) drags in Windows crypto headers (Softpub.h) that expect the
// legacy SAL macros IN/OUT, which FlexSim's headers leave undefined — restore
// them as no-ops so the include compiles.
#ifndef IN
#define IN
#endif
#ifndef OUT
#define OUT
#endif
#include "ProcessFlow.h"
#include "ProcessFlowObject.h"   // ProcessFlowObject::addConnectorIn/Out + full def for casts
#include "Connector.h"           // Connector::setFromBlock/setToBlock/onCreate

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

// ProcessFlow module export, linked via ProcessFlow.lib (see module.vcxproj).
// Global-scope C++ function — mangled ?getactivity@@YAPEAVTreeNode@FlexSim@@VVariant@2@PEBD@Z.
// Resolves an activity treenode by name within a process flow. First use of a
// cross-module ProcessFlow export from ModelerAI.
FlexSim::TreeNode* getactivity(FlexSim::Variant processFlow, const char* name);

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
        // Unwrap a JSON-quoted bare string (.1000085 — the agent naturally passes
        // args:"\"Queue1\"" for the bare-string form, which arrived here with the
        // literal quotes still attached and failed to resolve).
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_string()) return j.get<std::string>();
            } catch (...) {}
            return s.substr(1, s.size() - 2);
        }
        return s;
    }
    return "";
}

// Resolve a ProcessFlow storage node by name under /Tools/ProcessFlow, honoring
// a trailing ~N to pick the Nth flow of that name. Returns nullptr if absent.
treenode resolvePfNode(const std::string& pfName)
{
    std::string base = pfName;
    int idx = 1;
    auto t = pfName.rfind('~');
    if (t != std::string::npos) {
        try { idx = std::stoi(pfName.substr(t + 1)); base = pfName.substr(0, t); } catch (...) {}
    }
    treenode storage = node("Tools/ProcessFlow", model());
    if (!objectexists(storage)) return nullptr;
    int seen = 0, n = content(storage);
    for (int i = 1; i <= n; ++i) {
        treenode c = rank(storage, i);
        if (c && std::string(getname(c)) == base && ++seen == idx) return c;
    }
    return nullptr;
}

// Resolve a direct child activity of a ProcessFlow by name, honoring ~N.
treenode resolveActivityNode(treenode pf, const std::string& actName)
{
    if (!objectexists(pf)) return nullptr;
    std::string base = actName;
    int idx = 1;
    auto t = actName.rfind('~');
    if (t != std::string::npos) {
        try { idx = std::stoi(actName.substr(t + 1)); base = actName.substr(0, t); } catch (...) {}
    }
    int seen = 0, n = content(pf);
    for (int i = 1; i <= n; ++i) {
        treenode c = rank(pf, i);
        // Require a classobject so we only match real activities — a PF's direct
        // children also include bookkeeping containers (variables, connectors,
        // stats, tools) that can share a name; matching one of those would let
        // delete_activity/set_activity_variable mutate a structural node.
        if (c && std::string(getname(c)) == base
            && objectexists(classobject(c)) && ++seen == idx) return c;
    }
    return nullptr;
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
    // Common aliases the agent reaches for (.1000085): "number"/"float"/"double"
    // → continuous, "int" → integer.
    if      (lower == "continuous" || lower == "number" || lower == "float" || lower == "double")
                                     { out = ParamType::Continuous;  return true; }
    else if (lower == "integer" || lower == "int")
                                     { out = ParamType::Integer;     return true; }
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
// EXECUTION DOMAIN — gutted 2026-06-08 during run-tool clean-slate redesign.
// ============================================================================
//
// All previous run-state event machinery (Win32 events, hook install/
// uninstall helpers, temp-stop helpers, polling-loop helpers, sim-time
// watchdog, exit-reason mapping) was removed. The corresponding
// `modelerai_*` tools below are now shells returning "not_implemented"
// while the redesign is in flight. See KNOWLEDGE/topics/modelerai-quickref.md
// for the FlexScript console patterns we know work; the new tool surface
// will be built up from those.
// ============================================================================

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

        // Create natively. ObjectDataType::create() instantiates the class by
        // name and drops the new object straight into the model — replacing the
        // old Object.create + setProperty FlexScript codegen. Name / spatial /
        // parent / the extra-properties map are all native C++ now, so one bad
        // property gets recorded and skipped instead of aborting the whole
        // create the way the old all-or-nothing executestring did.
        std::vector<std::string> failedProps;
        ObjectDataType* odt = nullptr;
        try { odt = ObjectDataType::create(className.c_str()); } catch (...) { odt = nullptr; }
        treenode created = (odt != nullptr) ? odt->holder : nullptr;

        if (created && objectexists(created)) {
            if (!objName.empty()) setname(created, objName.c_str());
            if (hasLoc)  setloc(created, lx, ly, lz);
            if (hasRot)  setrot(created, rx, ry, rz);
            if (hasSize) setsize(created, sx, sy, sz);
            if (!parentName.empty()) {
                treenode parentObj = model()->find(parentName.c_str());
                if (objectexists(parentObj)) created->up = parentObj;
            }
            // Extra-properties map — native, with per-property recovery.
            // Vec3/Color values pass as a 3-/4-element Variant Array (there is
            // no Variant<-Vec3 conversion); setProperty returns 1 on success.
            if (propertiesMap.is_object() && !propertiesMap.empty()) {
                for (auto it = propertiesMap.begin(); it != propertiesMap.end(); ++it) {
                    const std::string propName = it.key();
                    const auto& pj = it.value();
                    Variant val;
                    bool shapeOk = true;
                    if (pj.is_boolean()) {
                        val = Variant(pj.get<bool>() ? 1.0 : 0.0);
                    } else if (pj.is_number()) {
                        val = Variant(pj.get<double>());
                    } else if (pj.is_string()) {
                        val = Variant(pj.get<std::string>().c_str());
                    } else if (pj.is_array() && (pj.size() == 3 || pj.size() == 4) &&
                               pj[0].is_number() && pj[1].is_number() && pj[2].is_number() &&
                               (pj.size() == 3 || pj[3].is_number())) {
                        Array arr;
                        for (size_t k = 0; k < pj.size(); ++k) arr.push(Variant(pj[k].get<double>()));
                        val = Variant(arr);
                    } else {
                        shapeOk = false;  // unsupported shape — skip + record
                    }
                    if (!shapeOk) { failedProps.push_back(propName); continue; }
                    try {
                        if (odt->setProperty(propName.c_str(), val) != 1)
                            failedProps.push_back(propName);
                    } catch (...) {
                        failedProps.push_back(propName);
                    }
                }
            }
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
        // Build the value as a native Variant and write it with the object's
        // setProperty (the proven create_object path). Vec3/Color go as a
        // 3-/4-element Array Variant — the lowest-common-denominator every
        // Vec3Property/ColorProperty setValue casts correctly.
        Variant val;
        std::string valueKind;
        if (gotShape == "boolean") {
            valueKind = "boolean";
            val = Variant(valueJson.get<bool>() ? 1.0 : 0.0);
        } else if (gotShape == "number") {
            valueKind = "number";
            val = Variant(valueJson.get<double>());
        } else if (gotShape == "string") {
            std::string raw = valueJson.get<std::string>();
            if (wrapAsScript) {
                // Only wrap if the body isn't already complete ("return "
                // anywhere → the AI wrote its own headers; pass through as-is).
                bool alreadyComplete = (raw.find("return ") != std::string::npos);
                if (!alreadyComplete) {
                    raw = "Object current = ownerobject(c);\n"
                          "Object item = param(1);\n"
                          "return " + raw + ";";
                }
                valueKind = "flexscript_body";
                // Scan the FlexScript body for hallucination / freeze-risk
                // patterns BEFORE writing it (expression-valued properties are
                // evaluated every time FlexSim queries the value).
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
            val = Variant(raw.c_str());
        } else if (gotShape == "vec3") {
            valueKind = "Vec3";
            Array v;
            v.push(Variant(valueJson[0].get<double>()));
            v.push(Variant(valueJson[1].get<double>()));
            v.push(Variant(valueJson[2].get<double>()));
            val = Variant(v);
        } else if (gotShape == "color_rgba") {
            valueKind = "Color";
            double r = valueJson[0].get<double>();
            double g = valueJson[1].get<double>();
            double b = valueJson[2].get<double>();
            double a = valueJson[3].get<double>();
            // FlexSim color channels are 0..1. If any of R/G/B exceeds 1 the
            // caller gave 0..255 (a common web-RGB / LLM mistake that otherwise
            // clamps everything to white) — rescale. Alpha is rescaled only if
            // it too is >1, so an opaque 0..255 color like [255,192,203,1] keeps
            // a=1 instead of going near-transparent.
            if (r > 1.0 || g > 1.0 || b > 1.0) { r /= 255.0; g /= 255.0; b /= 255.0; }
            if (a > 1.0) a /= 255.0;
            Array v;
            v.push(Variant(r));
            v.push(Variant(g));
            v.push(Variant(b));
            v.push(Variant(a));
            val = Variant(v);
        } else {
            return returnError("unsupported_value_type",
                "value must be a number, boolean, string, 3-element [x,y,z] (Vec3), "
                "or 4-element [r,g,b,a] (Color). Arbitrary arrays / nested objects "
                "not supported by set_property — use a more specific tool, or "
                "run_script if no curated tool exists.");
        }

        ObjectDataType* odt = obj->object<ObjectDataType>();
        if (!odt) {
            return returnError("setproperty_failed", "could not obtain object data from node.");
        }
        int setRc = 0;
        try {
            setRc = odt->setProperty(propertyName.c_str(), val);
        } catch (const std::exception& e) {
            return returnError("setproperty_failed",
                "setProperty(\"" + propertyName + "\", ...) threw: " + e.what() +
                ". Property name may not be valid for class " +
                (className.empty() ? std::string("<unknown>") : className) + ".");
        } catch (...) {
            return returnError("setproperty_failed",
                "setProperty(\"" + propertyName + "\", ...) threw a non-std exception.");
        }
        if (setRc != 1) {
            return returnError("setproperty_failed",
                "setProperty(\"" + propertyName + "\", ...) returned " + std::to_string(setRc) +
                " — property '" + propertyName + "' may not be valid for class " +
                (className.empty() ? std::string("<unknown>") : className) + ".");
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

        // Convert one JSON element to a native Variant. Used both for the
        // top-level Number/String/Pointer cases and for Array elements.
        // Returns false + sets `err` on failure.
        auto elemToVariant = [&](const nlohmann::json& v, Variant& out, std::string& err) -> bool {
            if (v.is_boolean()) { out = Variant(v.get<bool>() ? 1.0 : 0.0); return true; }
            if (v.is_number())  { out = Variant(v.get<double>()); return true; }
            if (v.is_string())  { out = Variant(v.get<std::string>().c_str()); return true; }
            if (v.is_object()) {
                std::string nodePath  = v.value("node_path", std::string(""));
                std::string objectRef = v.value("object",    std::string(""));
                TreeNode* ref = nullptr;
                if      (!nodePath.empty())  ref = node(nodePath.c_str(), nullptr);
                else if (!objectRef.empty()) ref = model()->find(objectRef.c_str());
                else { err = "object element must have `node_path` or `object`"; return false; }
                if (!objectexists(ref)) { err = "referenced node not found"; return false; }
                out = Variant(ref);
                return true;
            }
            err = "unsupported element type";
            return false;
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

        // Native C++ (.1000079 — was one big executestring script). Assert the
        // label node via assertlabel(); lbl->value is a VariantLValue that
        // writes through to the node (the native equivalent of FlexScript's
        // `lbl.value = ...`). Bundle/TrackedVariable use fsvisible bundle
        // commands + the TrackedVariable::init factory.
        TreeNode* lbl = assertlabel(obj, labelName.c_str());
        if (!objectexists(lbl)) {
            return returnError("assert_label_failed",
                "Could not assert label '" + labelName + "' on '" + objectName + "'.");
        }

        std::string valueKind;
        switch (kind) {
            case Kind::Number: {
                valueKind = "number";
                double n = valueJson.is_boolean()
                    ? (valueJson.get<bool>() ? 1.0 : 0.0)
                    : valueJson.get<double>();
                lbl->value = Variant(n);
                break;
            }
            case Kind::String: {
                valueKind = "string";
                lbl->value = Variant(valueJson.get<std::string>().c_str());
                break;
            }
            case Kind::Pointer: {
                valueKind = "pointer";
                Variant pv; std::string err;
                if (!elemToVariant(valueJson, pv, err))
                    return returnError("bad_value_shape", err);
                lbl->value = pv;
                break;
            }
            case Kind::Array: {
                valueKind = "array";
                Array a;
                for (const auto& el : valueJson) {
                    Variant ev; std::string err;
                    if (!elemToVariant(el, ev, err))
                        return returnError("bad_array_element", "Array element error: " + err);
                    a.push(ev);
                }
                lbl->value = Variant(a);
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
                // Set the body text, flip the node's FlexScript flag on, then
                // build/compile the script tree so it can evaluate.
                // switch_flexscript(node, 1) is the documented "force ON" form.
                lbl->value = Variant(body.c_str());
                switch_flexscript(lbl, 1);
                buildnodeflexscript(lbl);
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
                // Initialize the bundle the way FlexSim's own code does
                // (AStarNavigator::dumpBlockageData): set the node's dataType to
                // DATA_BUNDLE directly, THEN clearbundle, THEN addbundlefield.
                // (.1000082 fix — assertlabel(...,DATA_BUNDLE)/nodeadddata left a
                // stray default field, so the schema came back malformed.)
                lbl->dataType = DATA_BUNDLE;
                clearbundle(lbl);
                std::vector<std::string> fieldNames;  // for cell addressing by name
                std::vector<std::string> fieldTypes;  // for row coercion
                for (const auto& f : spec["fields"]) {
                    if (!f.is_object() || !f.contains("name") || !f.contains("type")) {
                        return returnError("bad_value_shape",
                            "Each bundle field needs `name` and `type`.");
                    }
                    std::string fname = f.value("name", std::string(""));
                    std::string ftype = f.value("type", std::string(""));
                    int typeMacro;
                    if      (ftype == "number" || ftype == "double") typeMacro = BUNDLE_FIELD_TYPE_DOUBLE;
                    else if (ftype == "int")                          typeMacro = BUNDLE_FIELD_TYPE_INT;
                    else if (ftype == "string")                       typeMacro = BUNDLE_FIELD_TYPE_VARCHAR;
                    else if (ftype == "node"   || ftype == "pointer") typeMacro = BUNDLE_FIELD_TYPE_NODEREF;
                    else {
                        return returnError("bad_value_shape",
                            "Bundle field type '" + ftype + "' unsupported. "
                            "Use one of: number, int, string, node.");
                    }
                    addbundlefield(lbl, fname.c_str(), typeMacro);
                    fieldNames.push_back(fname);
                    fieldTypes.push_back(ftype);
                }
                // Optional initial rows. Mirror Main_Node.fsx exactly: use the
                // entry number RETURNED by addbundleentry and a 0-based field
                // index (setbundlevalue(data, e, j-1, val)). The earlier code
                // assumed a 1-based rowIdx, which misaligned every row by one
                // entry and left the real rows empty. setbundlevalue has a
                // numeric and a string overload — pick by field type.
                if (spec.contains("rows") && spec["rows"].is_array()) {
                    for (const auto& row : spec["rows"]) {
                        if (!row.is_array()) continue;
                        int e = addbundleentry(lbl);   // entry number to write to
                        for (size_t c = 0; c < row.size() && c < fieldNames.size(); ++c) {
                            const nlohmann::json& cell = row[c];
                            int fieldIdx = static_cast<int>(c);  // 0-based
                            if (fieldTypes[c] == "string") {
                                std::string sv = cell.is_string() ? cell.get<std::string>()
                                    : (cell.is_number() ? std::to_string(cell.get<double>())
                                                        : std::string(""));
                                setbundlevalue(lbl, e, fieldIdx, sv.c_str());
                            } else if (fieldTypes[c] == "node" || fieldTypes[c] == "pointer") {
                                // Node-ref bundle cells aren't expressible through
                                // setbundlevalue's numeric/string overloads; leave
                                // the default (rare — caller can run_script if needed).
                            } else {
                                double dv = cell.is_boolean() ? (cell.get<bool>() ? 1.0 : 0.0)
                                    : (cell.is_number() ? cell.get<double>() : 0.0);
                                setbundlevalue(lbl, e, fieldIdx, dv);
                            }
                        }
                    }
                }
                break;
            }
            case Kind::TrackedVariable: {
                valueKind = "tracked_variable";
                const auto& spec = valueJson["tracked_variable"];
                // NOTE: the prior FlexScript form referenced STAT_TYPE_LEVEL_HISTORY /
                // _DISCRETE_VALUE / _DISCRETE_CHANGE, which exist neither as C++
                // macros nor as FlexScript constants — those branches never worked.
                // Mapped to the real STAT_TYPE_* macros (allobjects.h).
                int    tvType     = STAT_TYPE_TIME_SERIES;
                double startValue = 0.0;
                int    flags      = 0;   // SDK default
                if (spec.is_object()) {
                    std::string t = spec.value("type", std::string(""));
                    if      (t == "" || t == "time_series")    tvType = STAT_TYPE_TIME_SERIES;
                    else if (t == "categorical")               tvType = STAT_TYPE_CATEGORICAL;
                    else if (t == "categorical_combo")         tvType = STAT_TYPE_CATEGORICAL_COMBO;
                    else if (t == "level")                     tvType = STAT_TYPE_LEVEL;
                    else if (t == "cumulative")                tvType = STAT_TYPE_CUMULATIVE;
                    else if (t == "kinetic_level")             tvType = STAT_TYPE_KINETIC_LEVEL;
                    else if (t == "pointer")                   tvType = STAT_TYPE_POINTER;
                    else {
                        return returnError("bad_value_shape",
                            "TrackedVariable type '" + t + "' unknown. Use one of: "
                            "time_series, categorical, categorical_combo, level, "
                            "cumulative, kinetic_level, pointer.");
                    }
                    if (spec.contains("start_value") && spec["start_value"].is_number()) {
                        startValue = spec["start_value"].get<double>();
                    }
                    if (spec.contains("flags") && spec["flags"].is_number_integer()) {
                        flags = spec["flags"].get<int>();
                    }
                }
                TrackedVariable::init(lbl, tvType, startValue, flags);
                break;
            }
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

        // Find the label by name — native walk of the labels container node's
        // subnodes (replaces the executestring probe). LabelsArray is name-keyed
        // only, so walk the container directly and read dataType / value off the
        // label node; the C++ decode below maps dataType to kind. isScript stays
        // 0 (parity with the old minimal probe — the rare flexscript-valued
        // label is reported by its value type, not an explicit "flexscript" tag).
        treenode lbl = nullptr;
        TreeNode* labelsNode = labels(obj);
        if (objectexists(labelsNode)) {
            int lcount = content(labelsNode);
            for (int i = 1; i <= lcount; ++i) {
                TreeNode* l = rank(labelsNode, i);
                if (l && std::string(getname(l)) == labelName) { lbl = l; break; }
            }
        }
        if (!objectexists(lbl)) {
            nlohmann::json err;
            err["error"]  = "label_not_found";
            err["object"] = objectName;
            err["label"]  = labelName;
            return returnJson(err);
        }
        int     dt         = lbl->dataType;
        int     isScript   = 0;
        Variant labelValue = lbl->value;

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

        nlohmann::json out;
        out["ok"]     = true;
        out["object"] = objectName;
        out["label"]  = labelName;

        if (dt == DATA_BUNDLE) {
            // Native bundle read (.1000080 — was a hardcoded 0/0 stub that
            // punted to run_script). getbundlenrentries/nrfields give the dims;
            // getbundlefieldname/type the schema; getbundlevalue the cells.
            // FlexSim bundle fields AND entries are 0-based (verified against
            // Main_Node.fsx: getbundlefieldname(t,j-1), getbundlevalue(t,i,j-1)
            // with the entry loop starting at i=0). getbundlenr* return counts.
            out["value_kind"] = "bundle";
            int rows   = getbundlenrentries(lbl);
            int fields = getbundlenrfields(lbl);
            nlohmann::json b;
            b["row_count"]   = rows;
            b["field_count"] = fields;

            nlohmann::json fieldArr = nlohmann::json::array();
            for (int f = 0; f < fields; ++f) {
                nlohmann::json fj;
                const char* fn = getbundlefieldname(lbl, f);
                fj["name"] = fn ? std::string(fn) : std::string("");
                switch (getbundlefieldtype(lbl, f) & BUNDLE_FIELD_TYPE_MASK) {
                    case BUNDLE_FIELD_TYPE_DOUBLE:
                    case BUNDLE_FIELD_TYPE_FLOAT:   fj["type"] = "number"; break;
                    case BUNDLE_FIELD_TYPE_INT:     fj["type"] = "int";    break;
                    case BUNDLE_FIELD_TYPE_STR:
                    case BUNDLE_FIELD_TYPE_VARCHAR: fj["type"] = "string"; break;
                    case BUNDLE_FIELD_TYPE_NODEREF: fj["type"] = "node";   break;
                    default:                        fj["type"] = "other";  break;
                }
                fieldArr.push_back(std::move(fj));
            }
            b["fields"] = std::move(fieldArr);

            // Actual rows, capped to keep the payload bounded.
            const int ROW_CAP = 100;
            int rowsToRead = rows < ROW_CAP ? rows : ROW_CAP;
            nlohmann::json rowArr = nlohmann::json::array();
            for (int r = 0; r < rowsToRead; ++r) {
                nlohmann::json rowObj = nlohmann::json::object();
                for (int f = 0; f < fields; ++f) {
                    const char* fn = getbundlefieldname(lbl, f);
                    std::string key = fn ? std::string(fn) : ("field" + std::to_string(f));
                    rowObj[key] = variantToJson(getbundlevalue(lbl, r, f));
                }
                rowArr.push_back(std::move(rowObj));
            }
            b["rows"] = std::move(rowArr);
            if (rows > ROW_CAP) b["rows_truncated"] = true;
            out["value"] = std::move(b);
        } else if (isScript == 1) {
            out["value_kind"] = "flexscript";
            std::string body;  // isScript is always 0 here (dead branch)
            out["value"] = body;
            out["note"]  = "Value is the raw FlexScript body; not evaluated.";
        } else {
            // Inspect the Variant under "value" for shape detection.
            Variant v = labelValue;
            // A genuine TrackedVariable (DATA_SIMPLE node) evaluates to a Number
            // on read. But plain Variant labels ALSO land on DATA_SIMPLE when
            // they hold an Array (verified 2026-06-13: array label dataType==7),
            // so the bare dt==DATA_SIMPLE test mis-tagged arrays as tracked_variable.
            // Only tag tracked_variable when the value reads as a number; else
            // decode by the Variant type.
            if (dt == DATA_SIMPLE && v.type == VariantType::Number) {
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
        nlohmann::json labelList = nlohmann::json::array();
        try {
            // Native walk of the labels container node's subnodes (plain
            // treenodes) — replaces the executestring enumeration. Per label:
            // name / dataType / value read directly; the C++ decode below maps
            // dataType to kind. isScript stays 0 (parity with the old probe).
            TreeNode* labelsNode = labels(obj);
            if (objectexists(labelsNode)) {
                int lcount = content(labelsNode);
                for (int li = 1; li <= lcount; ++li) {
                    TreeNode* l = rank(labelsNode, li);
                    if (!l) continue;
                    nlohmann::json entry;
                    entry["name"]    = std::string(getname(l));
                    int dt           = l->dataType;
                    int isScript     = 0;
                    Variant val      = l->value;

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
                    } else if (dt == DATA_SIMPLE && val.type == VariantType::Number) {
                        // Genuine TrackedVariable — evaluates to a Number. Array
                        // labels also report DATA_SIMPLE, so they fall through to
                        // the type switch below (else they'd mis-tag as TV).
                        kind = "tracked_variable";
                        double v = static_cast<double>(val);
                        if (v == static_cast<double>(static_cast<long long>(v))
                            && v >= -9.2e18 && v <= 9.2e18) valueJson = static_cast<long long>(v);
                        else valueJson = v;
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
                    labelList.push_back(std::move(entry));
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
        out["label_count"] = static_cast<int>(labelList.size());
        out["labels"]      = std::move(labelList);
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

        // Native walk of the labels container node — find by name, destroy.
        bool removed = false;
        TreeNode* labelsNode = labels(obj);
        if (objectexists(labelsNode)) {
            int lcount = content(labelsNode);
            for (int i = 1; i <= lcount; ++i) {
                TreeNode* l = rank(labelsNode, i);
                if (l && std::string(getname(l)) == labelName) {
                    destroyobject(l);
                    removed = true;
                    break;
                }
            }
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
            // Native (.1000100 — was a maintree().find + forobjecttreeunder
            // executestring walk). node("MAIN:/project/library") resolves the
            // library root; nextforobjecttreeunder iterates every node under it;
            // leaf nodes (content==0) are the concrete classes — extract each via
            // applicationcommand (the same recursive call the FlexScript made).
            treenode lib = node("MAIN:/project/library", nullptr);
            if (!objectexists(lib)) {
                return returnError("library_not_found",
                    "Could not resolve MAIN:/project/library. Is a model loaded?");
            }
            int classCount = 0;
            try {
                for (treenode n = nextforobjecttreeunder(lib, lib);
                     objectexists(n);
                     n = nextforobjecttreeunder(n, lib)) {
                    if (content(n) != 0) continue;            // only leaf classes
                    std::string nm = getname(n);
                    std::string callArg = "{\"class_name\":\"" + nm + "\",\"save_to_file\":true}";
                    applicationcommand("modelerai_extract_class_schema", Variant(callArg.c_str()));
                    ++classCount;
                }
            } catch (const std::exception& e) {
                return returnError("all_walk_failed", std::string("class walk threw: ") + e.what());
            } catch (...) {
                return returnError("all_walk_failed", "class walk threw a non-std exception.");
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

        // Resolve the class node under /project/library. AGV::Path → AGV/?Path.
        // Native (.1000100 — was maintree().find via executestring). node() can't
        // do the `?` recursive search, so resolve the library root with node() then
        // TreeNode::find the class subpath (find DOES support `?`).
        std::string treePath = classNameToTreePath(className);   // "project/library/?Class"
        std::string relPath = treePath;
        const std::string libPrefix = "project/library/";
        if (relPath.rfind(libPrefix, 0) == 0) relPath = relPath.substr(libPrefix.size());
        TreeNode* classNode = nullptr;
        try {
            treenode lib = node("MAIN:/project/library", nullptr);
            if (objectexists(lib)) classNode = lib->find(relPath.c_str());
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
// PICKLISTS — live lookup (.1000125)
//
// modelerai_list_picks({ picklist?: "<name>" }):
//   Read VIEW:/picklists/<name> LIVE and return its PickOptions as JSON.
//   Defaults to "parameterpicklist" (the parameter "Set" behaviour picklist).
//   The running engine is the SINGLE SOURCE OF TRUTH — no stored catalog, so
//   results can never go stale or drift from the user's FlexSim version.
//   Confirmed tree shape (2026-06-22 probes):
//     - picklist node's string value  = the shared header (param decls).
//     - each child is a PickOption. Parameter options are object nodes whose
//       code lives in a hidden ">content" subnode; other picklists store the
//       code on the option node itself. We try ">content", then fall back to
//       the option node, so the same tool generalizes across surfaces.
//     - the template wraps: /***popup:NAME*/ marker, a /**Display*/ label,
//       and fillable /***tag:NAME*//**/VALUE/**/ slots.
// ============================================================================
namespace {

struct PickTag {
    std::string name;
    std::string defVal;
    std::vector<std::string> options;   // from a /**list:a~b~c*/ closer, if present
};

// Pull tag slots out of a PickOption template. A slot is:
//     /***tag:NAME*/ /**/ VALUE CLOSER
// where the opener is the empty comment "/**/" and CLOSER is the next comment,
// either "/**/" (plain) or "/**list:a~b~c*/" (a dropdown's allowed values).
// The value is therefore everything between the opener and the next comment
// start "/*" — NOT specifically the next "/**/" (that v1 assumption swallowed
// list-style closers whole). Confirmed against parameterpicklist (plain) and
// performancemeasurepicklist (list) on 2026-06-22.
std::vector<PickTag> parsePickTags(const std::string& tmpl)
{
    std::vector<PickTag> tags;
    const std::string open = "/***tag:";
    size_t pos = 0;
    while ((pos = tmpl.find(open, pos)) != std::string::npos) {
        size_t nameStart = pos + open.size();
        size_t nameEnd   = tmpl.find("*/", nameStart);
        if (nameEnd == std::string::npos) break;
        PickTag t;
        t.name = tmpl.substr(nameStart, nameEnd - nameStart);

        size_t openerPos = nameEnd + 2;             // first char after the tag's "*/"
        while (openerPos < tmpl.size() &&           // some tags put a space before "/**/"
               (tmpl[openerPos] == ' ' || tmpl[openerPos] == '\t' ||
                tmpl[openerPos] == '\r' || tmpl[openerPos] == '\n')) ++openerPos;
        if (openerPos + 4 <= tmpl.size() && tmpl.compare(openerPos, 4, "/**/") == 0) {
            size_t valStart = openerPos + 4;
            size_t valEnd   = tmpl.find("/*", valStart);   // start of the closer comment
            if (valEnd != std::string::npos) {
                t.defVal = tmpl.substr(valStart, valEnd - valStart);
                size_t closerEnd = tmpl.find("*/", valEnd + 2);
                if (closerEnd != std::string::npos) {
                    std::string closer = tmpl.substr(valEnd, closerEnd - valEnd);
                    size_t lp = closer.find("list:");
                    if (lp != std::string::npos) {
                        std::string opts = closer.substr(lp + 5);   // "a~b~c"
                        size_t start = 0;
                        while (true) {
                            size_t tilde = opts.find('~', start);
                            std::string o = (tilde == std::string::npos)
                                ? opts.substr(start) : opts.substr(start, tilde - start);
                            if (!o.empty()) t.options.push_back(o);
                            if (tilde == std::string::npos) break;
                            start = tilde + 1;
                        }
                    }
                    pos = closerEnd + 2;
                } else {
                    pos = valStart;
                }
            } else {
                pos = nameEnd + 2;
            }
        } else {
            pos = nameEnd + 2;
        }
        tags.push_back(std::move(t));
    }
    return tags;
}

// Pull the /***popup:NAME[:params]*/ marker. Params (rare on parameters) are
// kept as the raw colon-delimited tail.
void parsePickPopup(const std::string& tmpl, std::string& name, std::string& params)
{
    name.clear(); params.clear();
    const std::string open = "/***popup:";
    size_t s = tmpl.find(open);
    if (s == std::string::npos) return;
    s += open.size();
    size_t e = tmpl.find("*/", s);
    if (e == std::string::npos) return;
    std::string body = tmpl.substr(s, e - s);
    size_t colon = body.find(':');
    if (colon == std::string::npos) { name = body; return; }
    name   = body.substr(0, colon);
    params = body.substr(colon + 1);
}

// getnodestr() only on text (DATA_BYTEBLOCK) nodes. Calling it on object /
// coupling / empty nodes hard-faults inside FlexSim (NOT a catchable C++
// exception), which crashed list_picks on picklists whose node holds no header
// string (e.g. triggerpicklist). Returns "" for any non-text node.
std::string safePickStr(treenode n)
{
    if (!n) return "";
    try {
        if (n->dataType == DATA_BYTEBLOCK) return std::string(getnodestr(n));
    } catch (...) {}
    return "";
}

// Build the VIEW path to a picklist. With a module it lives under that module's
// own collection (VIEW:/modules/<module>/picklists/<name>); otherwise the general
// collection (VIEW:/picklists/<name>). Module families: ProcessFlow, People, AGV.
std::string picklistViewPath(const std::string& picklist, const std::string& module)
{
    if (!module.empty())
        return "VIEW:/modules/" + module + "/picklists/" + picklist;
    return "VIEW:/picklists/" + picklist;
}

// Recursively collect leaf PickOptions from a picklist into `out`. Picklists may
// nest CATEGORY containers (trigger picklists: Data / Control / Visual / ...).
// A node is a LEAF pick if it has a ">content" subnode (parameters) OR no visible
// children (PMs, trigger leaves — code is the node's own value); otherwise it's a
// category to descend into, prefixing the pick's `category` with the path.
void collectPicks(treenode container, const std::string& category, nlohmann::json& out)
{
    int n = 0; try { n = (int)container->subnodes.length; } catch (...) {}
    for (int i = 1; i <= n; ++i) {
        treenode o = nullptr; try { o = container->subnodes[i]; } catch (...) {}
        if (!o) continue;
        std::string name; try { name = getname(o); } catch (...) {}

        treenode content = nullptr; try { content = o->find(">content"); } catch (...) {}
        int kids = 0; try { kids = (int)o->subnodes.length; } catch (...) {}

        if (!content && kids > 0) {                       // category — recurse
            std::string sub = category.empty() ? name : (category + " / " + name);
            collectPicks(o, sub, out);
            continue;
        }

        std::string tmpl = content ? safePickStr(content) : safePickStr(o);
        std::string popupName, popupParams;
        parsePickPopup(tmpl, popupName, popupParams);

        nlohmann::json j;
        j["pick_name"] = name;
        if (!category.empty()) j["category"] = category;
        j["popup"] = popupName;
        if (!popupParams.empty()) j["popup_params"] = popupParams;
        nlohmann::json tarr = nlohmann::json::array();
        for (const auto& t : parsePickTags(tmpl)) {
            nlohmann::json tj = { {"name", t.name}, {"default", t.defVal} };
            if (!t.options.empty()) tj["options"] = t.options;
            tarr.push_back(std::move(tj));
        }
        j["tags"] = std::move(tarr);
        j["code_template"] = tmpl;
        out.push_back(std::move(j));
    }
}

// Replace one tag's value in-place: /***tag:NAME*//**/OLD/**/ -> .../**/NEW/**/.
// Keeps the tag markers intact so FlexSim's popup editor still recognizes the
// slot. Returns false if the named tag isn't present in the template.
bool fillOneTag(std::string& tmpl, const std::string& name, const std::string& value)
{
    std::string marker = "/***tag:" + name + "*/";
    size_t pos = tmpl.find(marker);
    if (pos == std::string::npos) return false;
    size_t openerPos = pos + marker.size();
    while (openerPos < tmpl.size() &&               // some tags put a space before "/**/"
           (tmpl[openerPos] == ' ' || tmpl[openerPos] == '\t' ||
            tmpl[openerPos] == '\r' || tmpl[openerPos] == '\n')) ++openerPos;
    if (!(openerPos + 4 <= tmpl.size() && tmpl.compare(openerPos, 4, "/**/") == 0)) return false;
    size_t valStart = openerPos + 4;
    size_t valEnd = tmpl.find("/*", valStart);    // start of the closer — value ends here
    if (valEnd == std::string::npos) return false;

    // Positional picks (e.g. statisticaldistribution) store each arg's SEPARATOR
    // inside the value slot — par2's default is ", 8", where the leading ", "
    // belongs to the function arg list, not the value. If the existing default
    // begins with (whitespace +) ',' and the caller's replacement doesn't already
    // start with that comma, preserve the separator so the AI can pass a plain
    // value: par2:"40" -> ", 40" (not "40" -> "exponential(040, ...)").
    std::string filled = value;
    std::string oldVal = tmpl.substr(valStart, valEnd - valStart);
    size_t sp = 0;
    while (sp < oldVal.size() && (oldVal[sp] == ' ' || oldVal[sp] == '\t')) ++sp;
    if (sp < oldVal.size() && oldVal[sp] == ',') {
        ++sp;                                       // past the comma
        while (sp < oldVal.size() && (oldVal[sp] == ' ' || oldVal[sp] == '\t')) ++sp;
        size_t firstNonWs = filled.find_first_not_of(" \t");
        bool callerHasComma = (firstNonWs != std::string::npos && filled[firstNonWs] == ',');
        if (!callerHasComma) filled = oldVal.substr(0, sp) + filled;
    }

    // Replace only the value; preserve the closer (incl. any /**list:...*/ spec).
    tmpl = tmpl.substr(0, valStart) + filled + tmpl.substr(valEnd);
    return true;
}

// Extract the /**Display*/ label that immediately follows the /***popup:*/
// marker — this equals the pick's display name, so it round-trips an applied
// pick back to its name. Returns "" if no popup marker is present (custom code).
std::string parsePickLabel(const std::string& code)
{
    size_t p = code.find("/***popup:");
    if (p == std::string::npos) return "";
    size_t pe = code.find("*/", p);
    if (pe == std::string::npos) return "";
    size_t s = pe + 2;                          // scan after the popup marker
    while (true) {
        size_t lb = code.find("/**", s);
        if (lb == std::string::npos) return "";
        // A comment closes at the first "*/" after its "/*" (lb+2), so the empty
        // "/**/" delimiter closes immediately (close == lb+2, before content start).
        size_t close = code.find("*/", lb + 2);
        if (close == std::string::npos) return "";
        std::string label = (close >= lb + 3) ? code.substr(lb + 3, close - (lb + 3)) : "";
        s = close + 2;
        // Accept only a real /**Label*/ — non-empty, NOT a /***tag:/***popup marker
        // (content starts with '*') and NOT a stray value delimiter (starts with '/').
        if (!label.empty() && label[0] != '*' && label[0] != '/') return label;
    }
}

// Extract a property's code header — the leading declarations (e.g. "Object
// current = ownerobject(c);\nObject item = param(1);") — from its current stored
// value, so an applied pick keeps the SAME header (the picklist itself doesn't
// supply it). The body starts at the first picklist marker / return / block, so
// the header is everything before that. Returns "" if no body marker is found.
std::string extractPropertyHeader(const std::string& code)
{
    size_t best = std::string::npos;
    for (const char* m : { "/***popup:", "return", "{" }) {
        size_t p = code.find(m);
        if (p < best) best = p;   // npos sorts as max, so the earliest marker wins
    }
    if (best == std::string::npos) return "";
    return code.substr(0, best);
}

// Find a parameter row node by name across ALL ParameterTables (parameter
// names are global). Returns the row node (parent of Name + Value) or null,
// and sets tableOut to the owning table node on a hit. Confirmed tree shape:
// Tools/ParameterTables/<Table>>variables/parameters/<rank>/{Name, Value}.
treenode findParamRowByName(const std::string& paramName, treenode& tableOut)
{
    tableOut = nullptr;
    treenode container = model()->find("Tools/ParameterTables");
    if (!objectexists(container)) return nullptr;
    int nt = 0; try { nt = (int)container->subnodes.length; } catch (...) {}
    for (int t = 1; t <= nt; ++t) {
        treenode tbl = nullptr; try { tbl = container->subnodes[t]; } catch (...) {}
        if (!tbl) continue;
        treenode plist = nullptr; try { plist = tbl->find(">variables/parameters"); } catch (...) {}
        if (!plist) continue;
        int np = 0; try { np = (int)plist->subnodes.length; } catch (...) {}
        for (int i = 1; i <= np; ++i) {
            treenode row = nullptr; try { row = plist->subnodes[i]; } catch (...) {}
            if (!row) continue;
            treenode nameNode = nullptr; try { nameNode = row->find("Name"); } catch (...) {}
            if (!nameNode) continue;
            std::string nm; try { nm = getnodestr(nameNode); } catch (...) {}
            if (nm == paramName) { tableOut = tbl; return row; }
        }
    }
    return nullptr;
}

// PM analogue of findParamRowByName. PM rows live in each table's bound
// performanceMeasures var space (getvarnode — find("variables") does not
// traverse it). Names are global across all PerformanceMeasureTables. Returns
// the row node (parent of Name + Value) and sets tableOut on a hit.
treenode findPmRowByName(const std::string& pmName, treenode& tableOut)
{
    tableOut = nullptr;
    treenode allTables = model()->find("Tools/PerformanceMeasureTables");
    if (!objectexists(allTables)) return nullptr;
    int nt = content(allTables);
    for (int iT = 1; iT <= nt; ++iT) {
        treenode t = rank(allTables, iT);
        if (!t) continue;
        treenode pms = getvarnode(t, "performanceMeasures");
        if (!pms) continue;
        int np = content(pms);
        for (int iP = 1; iP <= np; ++iP) {
            treenode pm = rank(pms, iP);
            if (!pm) continue;
            treenode nm = pm->find("Name");
            if (!nm) continue;
            std::string s; try { s = getnodestr(nm); } catch (...) {}
            if (s == pmName) { tableOut = t; return pm; }
        }
    }
    return nullptr;
}

// A pick surface: which picklist supplies the options, which Value subnode on
// the instance holds the applied code, and the resolved instance row. Keeps the
// per-surface knowledge in one place so apply/get share a single code path.
struct PickSurface {
    std::string picklist;     // VIEW:/picklists/<picklist>
    std::string writeNode;    // "onSet" (parameter) | "valueNode" (PM)
    std::string targetName;   // resolved instance name
    treenode    row = nullptr;
    treenode    table = nullptr;
};

// Resolve the surface from the request JSON. surface defaults to "parameter".
// Returns false and fills errKey/errMsg on a bad surface or a missing instance.
bool resolvePickSurface(const nlohmann::json& j, PickSurface& out,
                        std::string& errKey, std::string& errMsg)
{
    std::string surface = j.value("surface", std::string("parameter"));
    if (surface == "parameter") {
        out.picklist   = "parameterpicklist";
        out.writeNode  = "onSet";
        out.targetName = j.value("parameter", j.value("target", std::string("")));
        if (out.targetName.empty()) { errKey = "missing_args"; errMsg = "apply/get_pick requires `parameter`."; return false; }
        out.row = findParamRowByName(out.targetName, out.table);
        if (!objectexists(out.row)) { errKey = "not_found"; errMsg = "Parameter '" + out.targetName + "' not found in any ParameterTable."; return false; }
        return true;
    }
    if (surface == "performance_measure" || surface == "pm") {
        out.picklist   = "performancemeasurepicklist";
        out.writeNode  = "valueNode";
        out.targetName = j.value("performance_measure", j.value("target", std::string("")));
        if (out.targetName.empty()) { errKey = "missing_args"; errMsg = "apply/get_pick requires `performance_measure`."; return false; }
        out.row = findPmRowByName(out.targetName, out.table);
        if (!objectexists(out.row)) { errKey = "not_found"; errMsg = "Performance measure '" + out.targetName + "' not found."; return false; }
        return true;
    }
    errKey = "bad_surface";
    errMsg = "surface must be 'parameter' or 'performance_measure'.";
    return false;
}

// ---- Property picklist resolution from the QuickProperties panel tree --------
// VIEW:/standardviews/modelingutilities/QuickProperties >variables/propertiesPanels
// lists, per class, an Edit control per picklist-backed property. Everything is
// stored as ATTRIBUTES (the control node has 0 regular subnodes), so:
//   >variables/propName   — the property (e.g. "ProcessTime")
//   >variables/picklist   — a picklist VIEW path; its subnodes are ADDITIONAL
//                           picklist paths (the dropdown is their union)
//   >variables/codeheader — the exact header to prepend
//   >objectfocus          — "...>variables/<var>"; the tail is the real variable
// A panel applies to an object when isclasstype(obj, panelName). The same propName
// can appear under multiple matching panels with different variables (e.g.
// processTime vs cycletime) — pick the candidate getvarnode(obj, var) resolves to.
// Confirmed via FlexScript prototype 2026-06-23.

// Read any node's value as a string (plain string / coupling / byteblock) —
// QuickProperties values are paths/strings, not just FlexScript bodies.
std::string nodeValueStr(treenode n)
{
    if (!n) return "";
    try { return std::string(n->value.toString()); } catch (...) {}
    return "";
}

struct PropPanelResult {
    bool                     found = false;
    std::string              codeHeader;
    std::vector<std::string> picklistPaths;   // VIEW:/picklists/... (+ nested union)
    std::string              variable;        // chosen write-target variable
    std::vector<std::string> varCandidates;
};

void walkPropertyControls(treenode container, const std::string& prop,
                          PropPanelResult& out, int depth)
{
    if (!container || depth > 12) return;
    int n = 0; try { n = content(container); } catch (...) {}
    for (int i = 1; i <= n; ++i) {
        treenode c = nullptr; try { c = rank(container, i); } catch (...) {}
        if (!c) continue;

        treenode pn = nullptr; try { pn = c->find(">variables/propName"); } catch (...) {}
        if (pn && nodeValueStr(pn) == prop) {
            out.found = true;
            if (out.codeHeader.empty()) {
                treenode ch = nullptr; try { ch = c->find(">variables/codeheader"); } catch (...) {}
                if (ch) out.codeHeader = nodeValueStr(ch);
            }
            treenode pl = nullptr; try { pl = c->find(">variables/picklist"); } catch (...) {}
            if (pl) {
                std::string v = nodeValueStr(pl);
                if (!v.empty()) out.picklistPaths.push_back(v);
                int sn = 0; try { sn = content(pl); } catch (...) {}
                for (int k = 1; k <= sn; ++k) {
                    treenode s = nullptr; try { s = rank(pl, k); } catch (...) {}
                    if (s) { std::string sv = nodeValueStr(s); if (!sv.empty()) out.picklistPaths.push_back(sv); }
                }
            }
            treenode of = nullptr; try { of = c->find(">objectfocus"); } catch (...) {}
            if (of) {
                std::string ofv = nodeValueStr(of);
                size_t slash = ofv.rfind('/');
                std::string var = (slash == std::string::npos) ? ofv : ofv.substr(slash + 1);
                if (!var.empty()) out.varCandidates.push_back(var);
            }
        }
        walkPropertyControls(c, prop, out, depth + 1);
    }
}

// Resolve (object, property) -> { variable, codeHeader, picklistPaths } via panels.
PropPanelResult resolvePropertyPanel(treenode obj, const std::string& property)
{
    PropPanelResult res;
    treenode qp = node("VIEW:/standardviews/modelingutilities/QuickProperties", nullptr);
    if (!qp) return res;
    treenode panels = getvarnode(qp, "propertiesPanels");
    if (!panels) return res;
    int np = 0; try { np = content(panels); } catch (...) {}
    for (int i = 1; i <= np; ++i) {
        treenode panel = nullptr; try { panel = rank(panels, i); } catch (...) {}
        if (!panel) continue;
        std::string pname; try { pname = getname(panel); } catch (...) {}
        bool match = false;
        try { match = (isclasstype(obj, pname.c_str()) != 0); } catch (...) {}  // non-class names => false
        if (!match) continue;
        walkPropertyControls(panel, property, res, 0);
    }
    // Disambiguate the variable: the one that actually exists on the object.
    for (const auto& v : res.varCandidates) {
        treenode vn = nullptr; try { vn = getvarnode(obj, v.c_str()); } catch (...) {}
        if (objectexists(vn)) { res.variable = v; break; }
    }
    if (res.variable.empty() && !res.varCandidates.empty()) res.variable = res.varCandidates.front();
    return res;
}

// One picklist-backed property the object's panels expose — its display name, the
// write-target variable, and the picklist union (for discovery, not application).
struct PropSurfaceEntry {
    std::string              property;
    std::string              variable;
    std::vector<std::string> picklists;
};

// Like walkPropertyControls but collects EVERY control that carries a picklist
// (not just one propName), so we can map an object's whole picklist surface.
void collectAllPropertyControls(treenode container, std::vector<PropSurfaceEntry>& out, int depth)
{
    if (!container || depth > 12) return;
    int n = 0; try { n = content(container); } catch (...) {}
    for (int i = 1; i <= n; ++i) {
        treenode c = nullptr; try { c = rank(container, i); } catch (...) {}
        if (!c) continue;
        treenode pn = nullptr; try { pn = c->find(">variables/propName"); } catch (...) {}
        treenode pl = nullptr; try { pl = c->find(">variables/picklist"); } catch (...) {}
        std::string prop = pn ? nodeValueStr(pn) : std::string("");
        std::string plv  = pl ? nodeValueStr(pl)  : std::string("");
        if (!prop.empty() && !plv.empty()) {
            PropSurfaceEntry e;
            e.property = prop;
            e.picklists.push_back(plv);
            int sn = 0; try { sn = content(pl); } catch (...) {}
            for (int k = 1; k <= sn; ++k) {
                treenode s = nullptr; try { s = rank(pl, k); } catch (...) {}
                if (s) { std::string sv = nodeValueStr(s); if (!sv.empty()) e.picklists.push_back(sv); }
            }
            treenode of = nullptr; try { of = c->find(">objectfocus"); } catch (...) {}
            if (of) {
                std::string ofv = nodeValueStr(of);
                size_t sl = ofv.rfind('/');
                e.variable = (sl == std::string::npos) ? ofv : ofv.substr(sl + 1);
            }
            out.push_back(std::move(e));
        }
        collectAllPropertyControls(c, out, depth + 1);
    }
}

// Enumerate every picklist-backed property surface on `obj` from its matching
// QuickProperties panels. Deduped by property: for a property seen under several
// panels, keep the variable that actually resolves on the object and union the
// picklist paths.
std::vector<PropSurfaceEntry> resolveObjectPropertyPicklists(treenode obj)
{
    std::vector<PropSurfaceEntry> raw;
    treenode qp = node("VIEW:/standardviews/modelingutilities/QuickProperties", nullptr);
    if (qp) {
        treenode panels = getvarnode(qp, "propertiesPanels");
        if (panels) {
            int np = 0; try { np = content(panels); } catch (...) {}
            for (int i = 1; i <= np; ++i) {
                treenode panel = nullptr; try { panel = rank(panels, i); } catch (...) {}
                if (!panel) continue;
                std::string pname; try { pname = getname(panel); } catch (...) {}
                bool match = false;
                try { match = (isclasstype(obj, pname.c_str()) != 0); } catch (...) {}
                if (match) collectAllPropertyControls(panel, raw, 0);
            }
        }
    }
    std::vector<PropSurfaceEntry> outv;
    for (auto& e : raw) {
        PropSurfaceEntry* dst = nullptr;
        for (auto& o : outv) if (o.property == e.property) { dst = &o; break; }
        if (!dst) { outv.push_back(e); continue; }   // first sighting keeps its variable
        for (auto& p : e.picklists) {                // union picklists
            bool seen = false;
            for (auto& q : dst->picklists) if (q == p) { seen = true; break; }
            if (!seen) dst->picklists.push_back(p);
        }
        treenode vn = nullptr; try { vn = getvarnode(obj, e.variable.c_str()); } catch (...) {}
        if (objectexists(vn)) dst->variable = e.variable;   // prefer a resolvable variable
    }
    return outv;
}

// Collect picks from a picklist VIEW path, FOLLOWING picklist-reference options
// (a pick whose code_template is just "VIEW:/picklists/<other>", e.g.
// timeitempicklist's "Statistical Distribution" -> statisticaldistribution) so
// nested picks are reachable. `seen` guards against cycles / re-walks.
void collectPicksDeep(const std::string& viewPath, nlohmann::json& out,
                      std::vector<std::string>& seen, int depth)
{
    if (depth > 5) return;
    for (const auto& s : seen) if (s == viewPath) return;
    seen.push_back(viewPath);

    treenode pl = node(viewPath.c_str(), nullptr);
    if (!pl) return;

    nlohmann::json local = nlohmann::json::array();
    collectPicks(pl, "", local);

    const std::string linkPrefix = "VIEW:/picklists/";
    for (auto& p : local) {
        std::string tmpl = p.value("code_template", std::string(""));
        // Trim leading/trailing whitespace + a trailing ';'.
        size_t a = tmpl.find_first_not_of(" \t\r\n");
        std::string trimmed = (a == std::string::npos) ? "" : tmpl.substr(a);
        size_t b = trimmed.find_last_not_of(" \t\r\n;");
        trimmed = (b == std::string::npos) ? "" : trimmed.substr(0, b + 1);

        bool isLink = trimmed.rfind(linkPrefix, 0) == 0
                      && trimmed.find('\n') == std::string::npos
                      && trimmed.find(' ')  == std::string::npos;
        if (isLink)            collectPicksDeep(trimmed, out, seen, depth + 1);
        else if (!tmpl.empty()) out.push_back(p);
    }
}

} // namespace

modelerai_export Variant ModelerAi_listPicks(FLEXSIMINTERFACE)
{
    try {
        // Optional arg: { "picklist": "<name>" }. Defaults to the parameter
        // onSet picklist. A bare/empty/missing arg keeps the default.
        std::string picklistName = "parameterpicklist";
        std::string moduleName;
        nlohmann::json j;
        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            try {
                j = nlohmann::json::parse(std::string(arg));
                if (j.is_object()) {
                    picklistName = j.value("picklist", picklistName);
                    moduleName   = j.value("module", std::string(""));
                }
            } catch (...) { j = nlohmann::json(); /* tolerate non-JSON arg — keep default */ }
        }

        // Property surface — DISCOVER the picks for an (object, property) pair the
        // same way apply/get_pick RESOLVE them: walk the QuickProperties panel for
        // the picklist union, then flatten it WITH link-following so distribution
        // picks behind timeitempicklist -> statisticaldistribution are listed too.
        // { surface:"property", object, property(DISPLAY name) [, picklist to override] }.
        if (j.is_object() && j.value("surface", std::string("")) == "property") {
            std::string objectName   = j.value("object",   std::string(""));
            std::string propertyName = j.value("property", std::string(""));
            if (objectName.empty() || propertyName.empty()) {
                return returnError("missing_args",
                    "list_picks {surface:\"property\"} requires object and property.");
            }
            treenode obj = model()->find(objectName.c_str());
            if (!objectexists(obj)) {
                return returnError("not_found",
                    "Object '" + objectName + "' did not resolve via Model.find.");
            }
            PropPanelResult panel = resolvePropertyPanel(obj, propertyName);
            std::vector<std::string> paths;
            if (j.contains("picklist") && !picklistName.empty()) {
                paths.push_back(picklistViewPath(picklistName, moduleName));   // explicit override
            } else if (panel.found) {
                paths = panel.picklistPaths;
            }
            if (paths.empty()) {
                return returnError("picklist_unresolved",
                    "Could not auto-resolve a picklist for '" + propertyName + "' on "
                    + objectName + " — use the property's DISPLAY name (e.g. \"ProcessTime\"), "
                    "or pass an explicit `picklist`.");
            }
            nlohmann::json picks = nlohmann::json::array();
            std::vector<std::string> seen;
            for (const auto& p : paths) collectPicksDeep(p, picks, seen, 0);

            nlohmann::json out;
            out["ok"]         = true;
            out["surface"]    = "property";
            out["object"]     = objectName;
            out["property"]   = propertyName;
            if (panel.found) out["variable"] = panel.variable;
            out["picklists"]  = paths;
            out["pick_count"] = static_cast<int>(picks.size());
            out["picks"]      = std::move(picks);
            return returnJson(out);
        }

        if (picklistName.empty()) picklistName = "parameterpicklist";

        std::string viewPath = picklistViewPath(picklistName, moduleName);
        treenode pl = node(viewPath.c_str(), nullptr);
        if (!pl) {
            return returnError("not_found",
                viewPath + " not found — is a model open, and is the picklist name correct?");
        }

        // Header is "" for picklists that carry none on the node (e.g. trigger
        // picklists — header comes from the event). safePickStr never faults.
        std::string header = safePickStr(pl);

        // Recursive walk: flattens category nesting and reads each leaf's code
        // via the type-guarded reader, so no picklist shape can hard-fault.
        nlohmann::json picks = nlohmann::json::array();
        collectPicks(pl, "", picks);

        nlohmann::json out;
        out["ok"]         = true;
        out["picklist"]   = picklistName;
        if (!moduleName.empty()) out["module"] = moduleName;
        out["header"]     = header;
        out["pick_count"] = static_cast<int>(picks.size());
        out["picks"]      = std::move(picks);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_picks", e.what()); }
      catch (...)                     { return returnException("list_picks", "unknown"); }
}

// modelerai_list_object_picklists({ object }): map EVERY picklist-backed property
// surface the object exposes — one entry per property with its write-target
// variable and the picklist(s) its dropdown draws from. Discovery companion to
// list_picks {surface:"property"} (which drills into ONE property): this answers
// "what on this object can I drive with a preset?" Read-only. Accepts a bare
// object-name string too (for Console smoke tests).
modelerai_export Variant ModelerAi_listObjectPicklists(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        std::string objectName;
        if (arg.type == VariantType::String) {
            std::string s(arg);
            try {
                auto j = nlohmann::json::parse(s);
                objectName = j.is_object() ? j.value("object", std::string("")) : s;
            } catch (...) { objectName = s; }   // tolerate a bare object name
        }
        if (objectName.empty()) {
            return returnError("missing_args", "list_object_picklists requires { object }.");
        }
        treenode obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found",
                "Object '" + objectName + "' did not resolve via Model.find.");
        }

        std::string className;
        try { TreeNode* cn = classobject(obj); if (cn) className = getname(cn); } catch (...) {}

        std::vector<PropSurfaceEntry> entries = resolveObjectPropertyPicklists(obj);
        nlohmann::json props = nlohmann::json::array();
        for (const auto& e : entries) {
            nlohmann::json pj;
            pj["property"]  = e.property;
            pj["variable"]  = e.variable;
            pj["picklists"] = e.picklists;
            props.push_back(std::move(pj));
        }

        nlohmann::json out;
        out["ok"]             = true;
        out["object"]         = objectName;
        out["class"]          = className;
        out["property_count"] = static_cast<int>(entries.size());
        out["properties"]     = std::move(props);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_object_picklists", e.what()); }
      catch (...)                     { return returnException("list_object_picklists", "unknown"); }
}

// Trigger-surface impls — defined further below (after matchTrigger) so they can
// reuse its event enumeration; declared here so apply_pick/get_pick can dispatch
// to them. Internal linkage; same translation unit.
static nlohmann::json applyTriggerPickImpl(const nlohmann::json& j);
static nlohmann::json getTriggerPickImpl(const nlohmann::json& j);
static nlohmann::json applyPropertyPickImpl(const nlohmann::json& j);
static nlohmann::json getPropertyPickImpl(const nlohmann::json& j);

// ============================================================================
// modelerai_apply_pick({ surface?, parameter|performance_measure, pick_name,
//                        tags?: { name: expr }, reference?: "<object|group>" }):
//   Apply a pick BY NAME to a parameter (surface "parameter", default →
//   Value/onSet) or performance measure (surface "performance_measure" →
//   Value/valueNode). Resolves the pick LIVE from VIEW:/picklists/<picklist> (so
//   it can't apply one that doesn't exist), fills the tags, optionally binds
//   Value/reference (the object/group the behaviour acts on), then writes header
//   + filled template and compiles it (switch_flexscript + buildnodeflexscript)
//   — the same path add_parameter / create_performance_measure use. The embedded
//   /***popup:Name*/ marker makes FlexSim's UI show the pick as selected.
//
//   Self-validating: a bad pick_name returns { error:"pick_not_found",
//   available:[...] }; an unknown tag returns { error:"unknown_tag",
//   valid_tags:[...] }; an unresolvable reference returns
//   { error:"reference_not_found" }. Tag VALUES are raw FlexScript expressions —
//   quote string literals yourself (e.g. tags:{ property: "\"ItemPlacement\"" }).
// ============================================================================
modelerai_export Variant ModelerAi_applyPick(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "apply_pick expects { surface?, parameter|performance_measure, pick_name, tags? } as a JSON string.");
        }
        nlohmann::json j;
        try { j = nlohmann::json::parse(std::string(arg)); }
        catch (const std::exception& e) { return returnError("bad_args_json", e.what()); }
        if (!j.is_object()) {
            return returnError("bad_args_shape", "apply_pick expects a JSON object.");
        }

        // Trigger / property surfaces have a different shape — dispatch out.
        {
            std::string sfc = j.value("surface", std::string("parameter"));
            if (sfc == "trigger")  return returnJson(applyTriggerPickImpl(j));
            if (sfc == "property") return returnJson(applyPropertyPickImpl(j));
        }

        std::string pickName = j.value("pick_name", std::string(""));
        if (pickName.empty()) {
            return returnError("missing_args", "apply_pick requires `pick_name`.");
        }

        // --- Resolve the surface (parameter | performance_measure) + instance. ---
        PickSurface surf;
        std::string errKey, errMsg;
        if (!resolvePickSurface(j, surf, errKey, errMsg)) {
            return returnError(errKey.c_str(), errMsg);
        }

        // --- Resolve the live picklist + the requested pick. ---
        std::string viewPath = "VIEW:/picklists/" + surf.picklist;
        treenode pl = node(viewPath.c_str(), nullptr);
        if (!pl) {
            return returnError("not_found", viewPath + " not found.");
        }
        std::string header;
        try { header = getnodestr(pl); } catch (...) {}

        treenode opt = nullptr;
        nlohmann::json available = nlohmann::json::array();
        int n = 0; try { n = (int)pl->subnodes.length; } catch (...) {}
        for (int i = 1; i <= n; ++i) {
            treenode o = nullptr; try { o = pl->subnodes[i]; } catch (...) {}
            if (!o) continue;
            std::string nm; try { nm = getname(o); } catch (...) {}
            available.push_back(nm);
            if (nm == pickName) opt = o;
        }
        if (!opt) {
            nlohmann::json e;
            e["ok"]        = false;
            e["error"]     = "pick_not_found";
            e["message"]   = "No pick named '" + pickName + "' in " + surf.picklist + ". See `available`.";
            e["available"] = std::move(available);
            return returnJson(e);
        }

        treenode content = nullptr;
        try { content = opt->find(">content"); } catch (...) {}
        std::string tmpl;
        try { tmpl = content ? getnodestr(content) : getnodestr(opt); } catch (...) {}

        // --- Fill tags (raw FlexScript expressions). Unknown tag => error. ---
        nlohmann::json validTags = nlohmann::json::array();
        for (const auto& t : parsePickTags(tmpl)) validTags.push_back(t.name);
        nlohmann::json unknownTags = nlohmann::json::array();
        if (j.contains("tags") && j["tags"].is_object()) {
            for (auto it = j["tags"].begin(); it != j["tags"].end(); ++it) {
                if (!it.value().is_string()) continue;
                if (!fillOneTag(tmpl, it.key(), it.value().get<std::string>())) {
                    unknownTags.push_back(it.key());
                }
            }
        }
        if (!unknownTags.empty()) {
            nlohmann::json e;
            e["ok"]           = false;
            e["error"]        = "unknown_tag";
            e["message"]      = "Tag name(s) not present in this pick's template.";
            e["unknown_tags"] = std::move(unknownTags);
            e["valid_tags"]   = std::move(validTags);
            return returnJson(e);
        }

        treenode vNode = assertsubnode(surf.row, "Value", 0);

        // --- Optional: bind the instance's reference (the object/group the pick
        // acts on via `reference.as(...)`). Both surfaces store it at
        // Value/reference as a node coupling (pointTo) — same path
        // add_parameter / create_performance_measure use. Validate FIRST so a
        // bad reference can't leave a half-applied pick. ---
        std::string boundReference;
        if (j.contains("reference") && j["reference"].is_string()
            && !j["reference"].get<std::string>().empty()) {
            std::string refName = j["reference"].get<std::string>();
            treenode refNode = model()->find(refName.c_str());
            if (!objectexists(refNode))
                refNode = model()->find(("Tools/Groups/" + refName).c_str());
            if (!objectexists(refNode)) {
                return returnError("reference_not_found",
                    "reference '" + refName + "' did not resolve as an object "
                    "(Model.find) or a group (Tools/Groups/" + refName + ").");
            }
            assertsubnode(vNode, "reference", 0)->pointTo(refNode);
            boundReference = refName;
        }

        // --- Write header + filled template to the surface's code node + compile.
        // Same path add_parameter / create_performance_measure use. ---
        treenode target = assertsubnode(vNode, surf.writeNode.c_str(), DATATYPE_STRING);

        std::string finalCode = header + tmpl;
        target->value = Variant(finalCode.c_str());
        switch_flexscript(target, 1);
        buildnodeflexscript(target);

        nlohmann::json out;
        out["ok"]        = true;
        out["surface"]   = j.value("surface", std::string("parameter"));
        out["target"]    = surf.targetName;
        out["table"]     = std::string(getname(surf.table));
        out["pick_name"] = pickName;
        if (!boundReference.empty()) out["reference"] = boundReference;
        out["written"]   = finalCode;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("apply_pick", e.what()); }
      catch (...)                     { return returnException("apply_pick", "unknown"); }
}

// ============================================================================
// modelerai_get_pick({ parameter }):
//   Read back which onSet pick a parameter currently uses, plus the current
//   tag values. Parses the live Value/onSet text: the /**Display*/ label after
//   the /***popup:*/ marker gives current_pick; parsePickTags gives the filled
//   tag values. current_pick is null when the onSet holds no picklist marker
//   (hand-written / custom code). Read-side mirror of apply_pick.
// ============================================================================
modelerai_export Variant ModelerAi_getPick(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "get_pick requires a name (string or { surface?, parameter|performance_measure }).");
        }
        // Bare string => parameter name (backward compatible). JSON object =>
        // { surface?, parameter|performance_measure }.
        std::string s(arg);
        nlohmann::json j;
        try {
            j = nlohmann::json::parse(s);
            if (!j.is_object()) j = nlohmann::json{ {"parameter", s} };
        } catch (...) {
            j = nlohmann::json{ {"parameter", s} };
        }

        {
            std::string sfc = j.value("surface", std::string("parameter"));
            if (sfc == "trigger")  return returnJson(getTriggerPickImpl(j));
            if (sfc == "property") return returnJson(getPropertyPickImpl(j));
        }

        PickSurface surf;
        std::string errKey, errMsg;
        if (!resolvePickSurface(j, surf, errKey, errMsg)) {
            return returnError(errKey.c_str(), errMsg);
        }

        treenode vNode    = assertsubnode(surf.row, "Value", 0);
        treenode codeNode = assertsubnode(vNode, surf.writeNode.c_str(), DATATYPE_STRING);
        std::string code;
        try { code = getnodestr(codeNode); } catch (...) {}

        std::string label = parsePickLabel(code);
        std::string popupName, popupParams;
        parsePickPopup(code, popupName, popupParams);

        nlohmann::json out;
        out["ok"]        = true;
        out["surface"]   = j.value("surface", std::string("parameter"));
        out["target"]    = surf.targetName;
        out["table"]     = std::string(getname(surf.table));
        if (label.empty()) {
            out["current_pick"] = nullptr;       // custom / hand-written code
        } else {
            out["current_pick"] = label;
            out["popup"]        = popupName;
            nlohmann::json tarr = nlohmann::json::array();
            for (const auto& t : parsePickTags(code)) {
                nlohmann::json tj = { {"name", t.name}, {"value", t.defVal} };
                if (!t.options.empty()) tj["options"] = t.options;
                tarr.push_back(std::move(tj));
            }
            out["tags"] = std::move(tarr);
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_pick", e.what()); }
      catch (...)                     { return returnException("get_pick", "unknown"); }
}

// ============================================================================
// modelerai_resolve_property_picklist({ object, property }):
//   Read-only. Resolves a FlexScript-valued property's picklist surface from the
//   QuickProperties panel tree — returns { variable, codeheader, picklists }
//   where `variable` is the real write-target var (e.g. ProcessTime -> cycletime),
//   `codeheader` is the panel's exact header, and `picklists` is the VIEW-path
//   union the dropdown offers. The basis for apply_pick property auto-resolution.
// ============================================================================
modelerai_export Variant ModelerAi_resolvePropertyPicklist(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "resolve_property_picklist expects { object, property } as a JSON string.");
        }
        nlohmann::json j;
        try { j = nlohmann::json::parse(std::string(arg)); }
        catch (const std::exception& e) { return returnError("bad_args_json", e.what()); }
        std::string objectName = j.value("object",   std::string(""));
        std::string property   = j.value("property", std::string(""));
        if (objectName.empty() || property.empty()) {
            return returnError("missing_args",
                "resolve_property_picklist requires `object` and `property` (the display name, e.g. \"ProcessTime\").");
        }

        treenode obj = model()->find(objectName.c_str());
        if (!objectexists(obj)) {
            return returnError("not_found", "Object '" + objectName + "' did not resolve via Model.find.");
        }

        PropPanelResult r = resolvePropertyPanel(obj, property);
        std::string cls; try { cls = std::string(getname(classobject(obj))); } catch (...) {}
        if (!r.found) {
            return returnError("property_not_found",
                "No picklist-backed control for property '" + property + "' on " + objectName
                + " (class " + cls + "). Check the display name.");
        }

        nlohmann::json out;
        out["ok"]             = true;
        out["object"]         = objectName;
        out["class"]          = cls;
        out["property"]       = property;
        out["variable"]       = r.variable;
        out["codeheader"]     = r.codeHeader;
        out["picklists"]      = r.picklistPaths;
        out["var_candidates"] = r.varCandidates;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("resolve_property_picklist", e.what()); }
      catch (...)                     { return returnException("resolve_property_picklist", "unknown"); }
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
        // The auto-created AGV network's class short-name is "AGVNavigator"
        // (classobject().name returns the SHORT name, no module prefix); the
        // qualified "AGV::AGVNetwork" string does NOT match via isclasstype.
        // Keep the isclasstype call as a fallback in case the type registers.
        if (className == "AGVNavigator" || isclasstype(obj, "AGV::AGVNetwork")) {
            // Native (.1000094 — was a forobjecttreeunder executestring walk).
            // nextforobjecttreeunder iterates the object tree directly; collect
            // any AGV-typed object (excluding the network being deleted), capped
            // at 10, deduped by name. Detect AGV objects via isclasstype against
            // the qualified AGV leaf types — classobject().name returns SHORT
            // names with no "AGV::" prefix, so a name-prefix test never matches.
            std::vector<std::string> dependents;
            std::set<std::string> seen;
            treenode top = model();
            for (treenode n = nextforobjecttreeunder(top, top);
                 objectexists(n) && dependents.size() < 10;
                 n = nextforobjecttreeunder(n, top)) {
                if (n == obj) continue;
                bool isAgv = isclasstype(n, "AGV::StraightPath")
                          || isclasstype(n, "AGV::CurvedPath")
                          || isclasstype(n, "AGV::ControlPoint")
                          || isclasstype(n, "AGV::ControlArea");
                if (!isAgv) continue;
                treenode cls = classobject(n);
                std::string clsName = objectexists(cls) ? std::string(getname(cls)) : "AGV";
                std::string nm = std::string(getname(n));
                if (!seen.insert(nm).second) continue;
                dependents.push_back(nm + " (" + clsName + ")");
            }
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

        // Resolve parent container once (existence already validated above).
        TreeNode* parentObj = parentName.empty() ? nullptr
                                                  : model()->find(parentName.c_str());

        nlohmann::json created = nlohmann::json::array();
        for (int n = 1; n <= count; ++n) {
            // Native instance copy (.1000079 — was executestring src.copy()).
            // TreeNode::copy(destination, flags) is the engine_export method
            // that backs FlexScript's src.copy(dest); a null destination copies
            // into the source's own container (parity with bare src.copy()).
            treenode newObj = src->copy(parentObj, 0);
            if (!objectexists(newObj)) {
                return returnError("clone_failed",
                    "src.copy() returned null on iteration " + std::to_string(n));
            }
            if (count == 1 && !newName.empty()) {
                setname(newObj, newName.c_str());
            }
            if (hasLoc) {
                setloc(newObj, lx, ly, lz);
            } else if (hasOffset) {
                setloc(newObj, srcX + dx * n, srcY + dy * n, srcZ + dz * n);
            }
            if (copyConnections) {
                applicationcommand("recreateObjectConnections", Variant(newObj), Variant(src));
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

    // Native (.1000100 — was an executestring walker). Walk stored(fr) and the
    // bound networknodes space with nextforobjecttreeunder; each entry's value is
    // a coupling whose ownerobject() is the navigator anchor (a NetworkNode lives
    // one level under its Navigator, so fall back to owner->up for that case).
    treenode fr = model()->find(frName.c_str());
    if (!objectexists(fr)) {
        result.error_detail = "fixed resource '" + frName + "' not found";
        return result;
    }

    std::vector<TreeNode*> anchors;
    try {
        // FULLY guarded walk: ANY step (stored(), the value read, ownerobject(),
        // even the iterator advance) can throw a non-std FlexSim exception on some
        // node in the >stored / networknodes space (e.g. item-flow port couplings
        // live in >stored too). The FlexScript a.value walk tolerated those; here we
        // wrap every step so one bad node is skipped, never aborting the whole walk.
        // The nav-anchor couplings read fine, so real memberships are still found.
        auto couplingTarget = [](treenode n) -> treenode {
            try {
                Variant tv = n->value;
                if (tv.type == VariantType::TreeNode) return static_cast<treenode>(tv);
            } catch (...) {}
            return nullptr;
        };
        auto advance = [](treenode cur, treenode top) -> treenode {
            try { return nextforobjecttreeunder(cur, top); } catch (...) { return nullptr; }
        };

        treenode storedNode = 0;
        try { storedNode = stored(fr); } catch (...) {}
        if (objectexists(storedNode)) {
            for (treenode n = advance(storedNode, storedNode); objectexists(n); n = advance(n, storedNode)) {
                try {
                    treenode target = couplingTarget(n);
                    if (!objectexists(target)) continue;
                    treenode owner = ownerobject(target);
                    if (!objectexists(owner)) continue;
                    if (isclasstype(owner, "Navigator")
                     || isclasstype(owner, "AGV::ControlPoint")
                     || isclasstype(owner, "GIS::Point")) {
                        anchors.push_back(owner);
                    } else {
                        // Some systems (A*) store the coupling target a level or
                        // two beneath the navigator; walk up a few parents looking
                        // for a Navigator ancestor instead of only owner->up.
                        treenode anc = owner->up;
                        for (int up = 0; up < 3 && objectexists(anc); ++up, anc = anc->up) {
                            if (isclasstype(anc, "Navigator")) { anchors.push_back(anc); break; }
                        }
                    }
                } catch (...) { /* skip this node */ }
            }
        }

        treenode nnsNode = 0;
        try { nnsNode = getvarnode(fr, "networknodes"); } catch (...) {}
        if (objectexists(nnsNode)) {
            for (treenode n = advance(nnsNode, nnsNode); objectexists(n); n = advance(n, nnsNode)) {
                try {
                    treenode target = couplingTarget(n);
                    if (!objectexists(target)) continue;
                    treenode owner = ownerobject(target);
                    if (objectexists(owner)) anchors.push_back(owner);
                } catch (...) { /* skip this node */ }
            }
        }
    } catch (const std::exception& e) {
        result.error_detail = std::string("walker threw: ") + e.what();
        return result;
    } catch (...) {
        result.error_detail = "walker threw a non-std exception";
        return result;
    }

    // Walk succeeded — empty is a legitimate "not in any navigator system".
    result.ok = true;

    // Classify each collected anchor. CRITICAL: only keep recognized navigator
    // anchor classes. The walked coupling space (especially >stored) holds many
    // non-navigator couplings (flow ports, display blocks, library templates);
    // those classify as "Unknown" and are pure noise — they can never match a
    // requested system and previously flooded inspect_connections with hundreds
    // of bogus entries. Drop them, and dedupe by anchor name (the same anchor can
    // be reached through several couplings).
    std::set<std::string> seenAnchors;
    for (TreeNode* anchor : anchors) {
        if (!objectexists(anchor)) continue;

        std::string sys;
        if      (isclasstype(anchor, "AGV::ControlPoint"))     sys = "AGV";
        else if (isclasstype(anchor, "NetworkNode"))           sys = "Network";
        else if (isclasstype(anchor, "GIS::Point"))            sys = "GIS";
        else if (isclasstype(anchor, "AStar::AStarNavigator")) sys = "AStar";
        else if (isclasstype(anchor, "Navigator"))             sys = "Other";
        else continue;   // not a navigator anchor — skip the noise

        NavMembership m;
        m.system = sys;
        try { m.anchor_name = getname(anchor); } catch (...) {}
        if (!m.anchor_name.empty() && !seenAnchors.insert(m.anchor_name).second) continue;  // dedupe
        TreeNode* cls = nullptr;
        try { cls = classobject(anchor); } catch (...) {}
        if (cls) { try { m.anchor_class = getname(cls); } catch (...) {} }
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
        TreeNode* formerNavNode = nullptr;
        {
            ObjectDataType* teObj = te->object<ObjectDataType>();
            if (teObj) {
                Variant cur = teObj->getProperty("Navigator");
                if (cur.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(cur);
                    if (objectexists(nv)) { formerNavNode = nv; formerNavName = std::string(getname(nv)); }
                }
            }
        }

        // Already on the requested navigator? Compare by NODE IDENTITY, not name
        // (object names aren't globally unique, so a name compare could misfire).
        // For non-GIS, that's a true no-op → reject. For GIS, contextdragconnection
        // would auto-append a duplicate port, so short-circuit BEFORE the wire add.
        if (formerNavNode && formerNavNode == nav) {
            if (isGIS) {
                // Check if THIS specific point is already wired to the TE (either
                // port direction — the wire can land TE-in/point or TE-out/point).
                TreeNode* pt = model()->find(pointName.c_str());
                if (objectexists(pt) && (findInPortFor(te, pt) != 0 || findOutPortFor(te, pt) != 0)) {
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

        // Mutation — native setProperty("Navigator") + (GIS) native
        // contextdragconnection. The post-mutation read below verifies the bind.
        // (`nav` is already resolved earlier in this function.)
        {
            ObjectDataType* teObj = te->object<ObjectDataType>();
            if (!teObj) {
                return returnError("setprop_failed", "could not obtain TaskExecuter object from node.");
            }
            try {
                teObj->setProperty("Navigator", Variant(nav));
            } catch (const std::exception& e) {
                return returnError("setprop_failed",
                    std::string("setProperty(\"Navigator\") threw: ") + e.what());
            } catch (...) {
                return returnError("setprop_failed",
                    "setProperty(\"Navigator\") threw a non-std exception.");
            }
            if (isGIS) {
                treenode pt = model()->find(pointName.c_str());
                if (objectexists(pt)) contextdragconnection(te, pt, 'A');
            }
        }

        // POST-MUTATION VERIFICATION
        // 1. Confirm the Navigator property now points at nav (by NODE IDENTITY,
        //    not name — names aren't globally unique).
        std::string nowNavName;
        TreeNode* nowNavNode = nullptr;
        {
            ObjectDataType* teObj = te->object<ObjectDataType>();
            if (!teObj) {
                return returnError("connect_unverified",
                    "setProperty completed but could not obtain object for verification.");
            }
            Variant after = teObj->getProperty("Navigator");
            if (after.type == VariantType::TreeNode) {
                TreeNode* nv = static_cast<TreeNode*>(after);
                if (objectexists(nv)) { nowNavNode = nv; nowNavName = std::string(getname(nv)); }
            }
        }
        if (nowNavNode != nav) {
            return returnError("connect_unverified",
                "setProperty(Navigator, " + navName + ") returned but task executer's "
                "Navigator is now '" + nowNavName + "'. FlexSim may have "
                "rejected the assignment.");
        }
        // 2. For GIS, also verify the wire actually landed (either port direction).
        if (isGIS) {
            TreeNode* pt = model()->find(pointName.c_str());
            if (objectexists(pt) && findInPortFor(te, pt) == 0 && findOutPortFor(te, pt) == 0) {
                return returnError("connect_unverified",
                    "Navigator property was set but contextdragconnection "
                    "to GIS point '" + pointName + "' didn't materialize "
                    "on the task executer's ports.");
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

        // Connect via the A-drag gesture (same as the UI), then verify by
        // RE-WALKING the FR's navigator memberships. A FR→navigator link is stored
        // as a coupling, NOT an item-flow port wire, so doAConnect's port-array
        // check reports a false "connect_unverified" here (the connection actually
        // succeeds). doAConnect stays unchanged for the genuine A-port tools
        // (FR↔FR flow, dispatcher↔TE); this tool verifies via the coupling instead.
        try {
            contextdragconnection(fr, anchor, 'A');
        } catch (const std::exception& e) {
            return returnError("connect_failed",
                std::string("connect_fixed_resource_to_navigator: contextdragconnection threw: ") + e.what());
        } catch (...) {
            return returnError("connect_failed",
                "connect_fixed_resource_to_navigator: contextdragconnection threw a non-std exception.");
        }

        NavWalkResult after = fixedResourceNavigatorMemberships(frName);
        bool verified = false;
        std::string anchorClass;
        if (after.ok) {
            for (const auto& m : after.memberships) {
                if (m.system == system) { verified = true; anchorClass = m.anchor_class; break; }
            }
        }
        if (!verified) {
            return returnError("connect_unverified",
                "contextdragconnection from '" + frName + "' to '" + anchorName +
                "' completed but the FR did not appear in navigator system '" + system +
                "' on re-inspection. The class pair may not support this connection.");
        }

        nlohmann::json out;
        out["ok"]                   = true;
        out["fr"]                   = frName;
        out["anchor"]               = anchorName;
        out["system"]               = system;
        if (!anchorClass.empty()) out["anchor_class"] = anchorClass;
        out["memberships_verified"] = true;
        return returnJson(out);
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
            // Name the indices by what they actually are (the old
            // removed_from/to_index names had their values swapped):
            // existingFrom = the FROM node's output port; existingTo = the TO
            // node's input port.
            out["removed_from_outport"] = existingFrom;
            out["removed_to_inport"]    = existingTo;
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
            {
                ObjectDataType* teObj = fromNode->object<ObjectDataType>();
                if (!teObj) {
                    return returnError("read_navigator_failed",
                        "Could not obtain task executer object to read its Navigator.");
                }
                Variant cur = teObj->getProperty("Navigator");
                if (cur.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(cur);
                    if (objectexists(nv)) {
                        actualFormerName = std::string(getname(nv));
                        TreeNode* nvCls = classobject(nv);
                        if (nvCls) actualFormerClass = std::string(getname(nvCls));
                    }
                }
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

            // Mutation — native setProperty back to DefaultNavigator.
            {
                ObjectDataType* teObj = fromNode->object<ObjectDataType>();
                treenode defNav = model()->find("DefaultNavigator");
                if (!teObj || !objectexists(defNav)) {
                    return returnError("disconnect_failed",
                        "could not obtain task executer object or DefaultNavigator node.");
                }
                try {
                    teObj->setProperty("Navigator", Variant(defNav));
                } catch (const std::exception& e) {
                    return returnError("disconnect_failed",
                        std::string("setProperty(Navigator, DefaultNavigator) threw: ") + e.what());
                } catch (...) {
                    return returnError("disconnect_failed",
                        "setProperty(Navigator, DefaultNavigator) threw a non-std exception.");
                }
            }

            // Verify the disconnect actually took.
            std::string nowNavName;
            {
                ObjectDataType* teObj = fromNode->object<ObjectDataType>();
                if (!teObj) {
                    return returnError("disconnect_unverified",
                        "setProperty completed but could not obtain object for verification.");
                }
                Variant after = teObj->getProperty("Navigator");
                if (after.type == VariantType::TreeNode) {
                    TreeNode* nv = static_cast<TreeNode*>(after);
                    if (objectexists(nv)) nowNavName = std::string(getname(nv));
                }
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
            // Re-verify removal: a 'Q' drag can complete without throwing yet
            // leave the membership in place (wrong gesture for some anchor types).
            // Re-walk and demote any anchor that's still present to a failure so
            // we never report a dangling membership as removed.
            NavWalkResult afterWalk = fixedResourceNavigatorMemberships(fromName);
            if (afterWalk.ok) {
                std::set<std::string> stillAnchors;
                for (const auto& m : afterWalk.memberships) stillAnchors.insert(m.anchor_name);
                nlohmann::json reallyRemoved = nlohmann::json::array();
                for (auto& e : removed) {
                    std::string a = e.value("anchor", std::string("?"));
                    std::string s = e.value("system", std::string("?"));
                    if (stillAnchors.count(a)) {
                        failures.push_back(a + " (still in system '" + s + "' after Q-disconnect)");
                    } else {
                        reallyRemoved.push_back(e);
                    }
                }
                removed = std::move(reallyRemoved);
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
                ObjectDataType* teObj = obj->object<ObjectDataType>();
                Variant v = teObj ? teObj->getProperty("Navigator") : Variant();
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

        // Groups — which groups is this object a member of? Native (.1000093 —
        // was a forobjecttreeunder walk of the >Groups attribute). Iterate every
        // Group under Tools/Groups and test direct membership via the engine_export
        // Group::isMember — the canonical inverse of the >Groups coupling, same
        // result without traversing the coupling subtree.
        nlohmann::json groupsArr = nlohmann::json::array();
        if (TreeNode* gRoot = model()->find("Tools/Groups")) {
            int gn = content(gRoot);
            for (int i = 1; i <= gn; ++i) {
                TreeNode* gNode = rank(gRoot, i);
                if (!gNode) continue;
                Group* g = gNode->object<Group>();
                if (g && g->isMember(obj, 0)) {
                    groupsArr.push_back(std::string(getname(gNode)));
                }
            }
        }

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

// Parameter tree-walk helpers (defined alongside the PM helpers below;
// forward-declared so add_parameter / set_parameter can share them).
static TreeNode* findParamNodeByName(const std::string& name);
static TreeNode* owningParamTable(TreeNode* p);

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
            // name + type required; table_name defaults to "Parameters" (the
            // table every model ships with) when omitted (.1000085 — was a hard
            // requirement that cost the agent retries).
            if (!j.contains("name") || !j["name"].is_string()
                || !j.contains("type") || !j["type"].is_string())
            {
                return returnError("missing_field",
                    "add_parameter requires name and type (table_name optional, defaults to \"Parameters\").");
            }
            tableName    = j.value("table_name", std::string("Parameters"));
            if (tableName.empty()) tableName = "Parameters";
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

        // Native (.1000099 — was a ~200-line executestring builder). Validate the
        // FlexScript hook bodies FIRST so a bad body can't leave a half-built row,
        // then build the row with SDK calls (Tools::create, function_s("addParameter"),
        // assertsubnode, pointTo, switch_flexscript/buildnodeflexscript). The hook +
        // expression bodies stay FlexScript (authored user code in the tree); only
        // the generated-string eval (executestring) is removed.

        // on_set: auto-prepend the mandatory 4-param signature when the body uses
        // reference/newValue/oldValue/isReset without declaring them, then scan.
        std::string finalOnSet;
        if (!onSetBody.empty()) {
            bool usesRef   = onSetBody.find("reference") != std::string::npos;
            bool declRef   = onSetBody.find("treenode reference") != std::string::npos;
            bool usesNew   = onSetBody.find("newValue") != std::string::npos;
            bool declNew   = onSetBody.find("Variant newValue") != std::string::npos
                          || onSetBody.find("Variant  newValue") != std::string::npos;
            bool usesOld   = onSetBody.find("oldValue") != std::string::npos;
            bool declOld   = onSetBody.find("Variant oldValue") != std::string::npos
                          || onSetBody.find("Variant  oldValue") != std::string::npos;
            bool usesReset = onSetBody.find("isReset") != std::string::npos;
            bool declReset = onSetBody.find("int isReset") != std::string::npos
                          || onSetBody.find("int  isReset") != std::string::npos;
            std::string preamble;
            if (usesRef   && !declRef)   preamble += "treenode reference = param(1);\n";
            if (usesNew   && !declNew)   preamble += "Variant newValue = param(2);\n";
            if (usesOld   && !declOld)   preamble += "Variant oldValue = param(3);\n";
            if (usesReset && !declReset) preamble += "int isReset = param(4);\n";
            finalOnSet = preamble + onSetBody;
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(finalOnSet, anName, anRem)) {
                return returnError("deprecated_api_in_on_set",
                    "on_set body uses a deprecated/hallucinated API: " + anRem);
            }
        }
        if (!onConstrainBody.empty()) {
            std::string anName, anRem;
            if (ModelerAi::scanAntiPatterns(onConstrainBody, anName, anRem)) {
                return returnError("deprecated_api_in_on_constrain",
                    "on_constrain body uses a deprecated/hallucinated API: " + anRem);
            }
        }

        // Table lookup: exact name match, else first existing ParameterTable
        // (every model ships with one), else create a new one.
        treenode tbl = 0;
        TreeNode* container = model()->find("Tools/ParameterTables");
        if (!tableName.empty() && container) {
            int nc = content(container);
            for (int i = 1; i <= nc; ++i) {
                TreeNode* t = rank(container, i);
                if (t && std::string(getname(t)) == tableName) { tbl = t; break; }
            }
        }
        if (!tbl && container && content(container) > 0) tbl = rank(container, 1);
        if (!tbl) {
            tbl = Tools::create("ParameterTable");
            if (!objectexists(tbl)) {
                return returnError("registration_failed", "Tools::create(\"ParameterTable\") returned null.");
            }
            setname(tbl, tableName.empty() ? "Parameters" : tableName.c_str());
        }

        // Uniqueness: parameter names are global (Model.parameters[name] takes no
        // table arg). Same-table hit is replace-or-reject; other-table is rejected.
        if (TreeNode* dup = findParamNodeByName(name)) {
            TreeNode* dupTbl = owningParamTable(dup);
            if (dupTbl == tbl) {
                if (replace) destroyobject(dup);
                else return returnError("duplicate_name",
                    "A parameter named '" + name + "' already exists in this table (use replace=true).");
            } else {
                return returnError("duplicate_name_other_table",
                    "A parameter named '" + name + "' already exists in table '"
                    + (dupTbl ? std::string(getname(dupTbl)) : std::string("?"))
                    + "'. Parameter names must be globally unique.");
            }
        }

        // Allocate the row via the table's addParameter node-function.
        treenode np = 0;
        {
            Variant npv = function_s(tbl, "addParameter");
            if (npv.type == VariantType::TreeNode) np = static_cast<treenode>(npv);
        }
        if (!objectexists(np)) {
            return returnError("registration_failed", "addParameter did not return a valid row node.");
        }

        // Name + Value.type.
        assertsubnode(np, "Name", DATATYPE_STRING)->value = Variant(name.c_str());
        treenode vNode = assertsubnode(np, "Value", 0);
        assertsubnode(vNode, "type", 0)->value = Variant((double)(int)type);

        auto setNum = [&](const char* sub, double n) {
            assertsubnode(vNode, sub, 0)->value = Variant(n);
        };

        if (type == ParamType::Continuous || type == ParamType::Integer || type == ParamType::Discrete) {
            // ALWAYS emit bounds for numeric types. FlexSim's default upperBound on
            // a fresh row is 10, and onConstrain clamps anything above it. Permissive
            // defaults (-1e9 / 1e9) when the caller doesn't specify.
            setNum("lowerBound", hasLower ? lowerBound : -1e9);
            setNum("upperBound", hasUpper ? upperBound :  1e9);
        }
        if (type == ParamType::Discrete && hasStep) setNum("stepSize", stepSize);

        if (type == ParamType::Option) {
            treenode opts = assertsubnode(vNode, "options", 0);
            // Bounded purge of any default options (cap guards against a runaway).
            for (int guard = 0; guard < 4096 && content(opts) > 0; ++guard) {
                treenode first = rank(opts, 1);
                if (!first) break;
                destroyobject(first);
            }
            for (size_t i = 0; i < optionNames.size(); ++i) {
                assertsubnode(opts, optionNames[i].c_str(), 0)->value = Variant((double)(int)(i + 1));
            }
        }

        if (type == ParamType::Sequence) {
            setNum("sequenceLength", static_cast<double>(sequenceLength));
            if (!sequenceValues.empty()) {
                Array seq((int)sequenceValues.size());          // 1-based
                for (size_t i = 0; i < sequenceValues.size(); ++i)
                    seq[(int)i + 1] = Variant((double)(int)sequenceValues[i]);
                vNode->value = Variant(seq);
            }
        }

        if (type == ParamType::Expression) {
            treenode exprNode = assertsubnode(vNode, "expression", DATATYPE_STRING);
            exprNode->value = Variant(expressionCode.c_str());
            switch_flexscript(exprNode, 1);
            buildnodeflexscript(exprNode);
            vNode->value = Variant(expressionCode.c_str());
        }

        if (type == ParamType::Passthrough || type == ParamType::Custom) {
            if (!referencePath.empty()) {
                treenode refn = model()->find(referencePath.c_str());
                if (!objectexists(refn)) {
                    return returnError("reference_not_found",
                        "reference_path not found: " + referencePath);
                }
                assertsubnode(vNode, "reference", 0)->pointTo(refn);
            }
        }

        if (hasValue) {
            if (type == ParamType::Sequence || type == ParamType::Passthrough || type == ParamType::Expression) {
                // handled above
            } else if (valueJson.is_number()) {
                vNode->value = Variant(valueJson.get<double>());
            } else if (valueJson.is_string()) {
                vNode->value = Variant(valueJson.get<std::string>().c_str());
            }
        }

        if (!description.empty()) {
            assertsubnode(np, "Description", DATATYPE_STRING)->value = Variant(description.c_str());
        }
        if (!displayUnits.empty()) {
            assertsubnode(np, "Display Units", DATATYPE_STRING)->value = Variant(displayUnits.c_str());
        }

        // on_set / on_constrain hooks — bodies validated above; write + compile
        // in-tree (switch_flexscript force-ON + buildnodeflexscript).
        if (!finalOnSet.empty()) {
            treenode onSetHook = assertsubnode(np, "onSet", DATATYPE_STRING);
            onSetHook->value = Variant(finalOnSet.c_str());
            switch_flexscript(onSetHook, 1);
            buildnodeflexscript(onSetHook);
        }
        if (!onConstrainBody.empty()) {
            treenode onConstrainHook = assertsubnode(np, "onConstrain", DATATYPE_STRING);
            onConstrainHook->value = Variant(onConstrainBody.c_str());
            switch_flexscript(onConstrainHook, 1);
            buildnodeflexscript(onConstrainHook);
        }

        std::string newPath;
        if (const char* p = nodetomodelpath_cstr(np, 1)) newPath = std::string(p);

        nlohmann::json out;
        out["ok"]       = true;
        out["table"]    = std::string(getname(tbl));
        out["name"]     = name;
        out["type"]     = typeStr;
        out["path"]     = newPath;
        out["accessor"] = "Model.parameters[\"" + name + "\"]";
        if (!onSetBody.empty())       out["on_set_applied"]       = true;
        if (!onConstrainBody.empty()) out["on_constrain_applied"] = true;

        // For numeric-valued types, read the value back and surface any clamping
        // FlexSim's onConstrain performed. Native read: Model::getParameters()
        // .findNode(name)->value (parameters store values; no eval like PMs).
        if (hasValue && valueJson.is_number()
            && (type == ParamType::Continuous || type == ParamType::Integer
             || type == ParamType::Discrete   || type == ParamType::Binary))
        {
            try {
                Parameters params = Model::getParameters();
                treenode pNode = params.findNode(name.c_str());
                if (objectexists(pNode)) {
                    Variant av = pNode->value;
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

        // Native (.1000099 — was an executestring builder). Parameter names are
        // GLOBAL: find the row across all ParameterTables via the shared walk
        // (table_name is informational). Update bounds/structural fields FIRST,
        // then value — setting value before bounds lets onConstrain clamp against
        // the OLD upperBound and the new value is silently lost.
        treenode found = findParamNodeByName(name);
        if (!objectexists(found)) {
            return returnError("not_found",
                "set_parameter: parameter not found in any ParameterTable: " + name);
        }
        treenode owningTable = owningParamTable(found);
        treenode v = assertsubnode(found, "Value", 0);

        auto setNum = [&](const char* sub, double n) {
            assertsubnode(v, sub, 0)->value = Variant(n);
        };
        if (hasLower)  setNum("lowerBound", lowerBound);
        if (hasUpper)  setNum("upperBound", upperBound);
        if (hasStep)   setNum("stepSize",   stepSize);
        if (hasSeqLen) setNum("sequenceLength", static_cast<double>(sequenceLength));

        if (hasValue) {
            if (valueJson.is_number()) {
                v->value = Variant(valueJson.get<double>());
            } else if (valueJson.is_string()) {
                v->value = Variant(valueJson.get<std::string>().c_str());
            } else if (valueJson.is_array()) {
                Array seq((int)valueJson.size());                       // 1-based
                for (size_t i = 0; i < valueJson.size(); ++i)
                    seq[(int)i + 1] = Variant(valueJson[i].is_number() ? valueJson[i].get<double>() : 0.0);
                v->value = Variant(seq);
            }
        }
        if (!sequenceValues.empty()) {
            Array seq2((int)sequenceValues.size());
            for (size_t i = 0; i < sequenceValues.size(); ++i)
                seq2[(int)i + 1] = Variant((double)(int)sequenceValues[i]);
            v->value = Variant(seq2);
        }
        if (hasDesc) {
            assertsubnode(found, "Description", DATATYPE_STRING)->value = Variant(description.c_str());
        }
        if (hasUnits) {
            assertsubnode(found, "Display Units", DATATYPE_STRING)->value = Variant(displayUnits.c_str());
        }

        std::string newPath;
        if (const char* p = nodetomodelpath_cstr(found, 1)) newPath = std::string(p);

        nlohmann::json out;
        out["ok"]       = true;
        out["table"]    = owningTable ? std::string(getname(owningTable)) : tableName;
        out["name"]     = name;
        out["path"]     = newPath;
        out["accessor"] = "Model.parameters[\"" + name + "\"]";

        // Surface any onConstrain clamping on the value update (native read:
        // Model::getParameters().findNode(name)->value).
        if (hasValue && valueJson.is_number()) {
            try {
                Parameters params = Model::getParameters();
                treenode pNode = params.findNode(name.c_str());
                if (objectexists(pNode)) {
                    Variant av = pNode->value;
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

        // Native (.1000084 — was executestring). Walk Tools/ParameterTables; for
        // each table (optionally name-filtered) read its bound `parameters` space
        // via getvarnode and match each parameter's Name node; destroyobject the
        // hit. getvarnode + destroyobject are fsvisible; TreeNode::find links.
        TreeNode* allTables = model()->find("Tools/ParameterTables");
        TreeNode* found = nullptr;
        std::string owner;
        if (allTables) {
            int nt = content(allTables);
            for (int iT = 1; iT <= nt && !found; ++iT) {
                TreeNode* t = rank(allTables, iT);
                if (!t) continue;
                if (!tableName.empty() && std::string(getname(t)) != tableName) continue;
                TreeNode* ps = getvarnode(t, "parameters");
                if (!ps) continue;
                int np = content(ps);
                for (int iX = 1; iX <= np; ++iX) {
                    TreeNode* p = rank(ps, iX);
                    if (!p) continue;
                    TreeNode* nm = p->find("Name");
                    if (nm && std::string(nm->value) == paramName) {
                        found = p;
                        owner = std::string(getname(t));
                        break;
                    }
                }
            }
        }
        if (!found) {
            nlohmann::json err;
            err["error"] = "not_found";
            err["name"]  = paramName;
            if (!tableName.empty()) err["table_name"] = tableName;
            err["note"]  = tableName.empty()
                ? "Parameter not found in any ParameterTable."
                : "Parameter not found in the specified table.";
            return returnJson(err);
        }
        destroyobject(found);

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
// RUN-MODEL TOOL SHELLS — clean-slate redesign in progress (2026-06-08).
// ============================================================================
//
// The 15 functions below are all temporary shells. The original
// implementations had FlexScript API mismatches (function_s used where
// setsdtvalue belongs), wrong reset/install ordering for temp stop times,
// and overcomplicated hook-based synchronization that the modeler is
// rebuilding from scratch. Until the redesign lands, every tool below
// returns "not_implemented" so the AI gets a clear signal rather than a
// silent freeze.
//
// Working FlexScript console patterns (verified) live in
// KNOWLEDGE/topics/modelerai-quickref.md under "Run-model tools (redesign
// in progress)".
//
// EXCEPTION: ModelerAi_notifyRunState stays a true no-op (not an error
// return). User model files may still carry our previously-installed
// trigger hook code that calls `applicationcommand("modelerai_notify_run_state", ...)`.
// Returning an error from those callers would log noise on every reset/run;
// a no-op is invisible.
// ============================================================================
modelerai_export Variant ModelerAi_notifyRunState(FLEXSIMINTERFACE)
{
    return Variant(0.0);
}



// modelerai_reset_model() — resetmodel(1) + clear toolbar Running indicator.
// Returns { ok, sim_time }. sim_time is always 0 after a successful reset.
modelerai_export Variant ModelerAi_resetModel(FLEXSIMINTERFACE)
{
    try {
        // Native (.1000084 — was executestring). resetmodel/applicationcommand/
        // time are all fsvisible/engine_export.
        resetmodel(1);
        applicationcommand("switchRunning", Variant(0.0));
        double finalTime = time();
        nlohmann::json out;
        out["ok"]       = true;
        out["sim_time"] = finalTime;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("reset_model", e.what()); }
      catch (...)                     { return returnException("reset_model", "unknown"); }
}

// modelerai_run_to_time({target_sim_time})
//
// Stub. The real work lives in the modelerai_call dispatcher
// (tools/write/modelerai_call.cpp) under a special case for this command
// name. The dispatcher runs the wait loop on the AGENT WORKER thread and
// drives each small main-thread interaction (install, polled state read,
// cleanup) through mainthread::runAndWait. Between polls the main thread
// is FlexSim's, so sim advances normally.
//
// This stub exists so the FlexScript binding in ModelerAI.fsx still
// resolves. If anyone invokes applicationcommand("modelerai_run_to_time",
// ...) directly (bypassing modelerai_call), they get this error.
modelerai_export Variant ModelerAi_runToTime(FLEXSIMINTERFACE)
{
    return returnError("invoke_via_modelerai_call",
        "modelerai_run_to_time must be invoked through the modelerai_call "
        "dispatcher, not through applicationcommand directly. The "
        "dispatcher runs the wait on the worker thread so FlexSim's main "
        "thread is free to advance sim between polls; a direct call from "
        "FlexScript would block the main thread and the run would never "
        "progress past t=0.");
}

// Internal helper: shared step-loop body for run_until + run_to_end.
//
// Builds a FlexScript snippet:
//
//    resetmodel(1);
//    applicationcommand("switchRunning", 0);
//    Map out = Map();
//    int safetyTime = <safety_sec>;
//    int steps = 0;
//    int reason = 0;   // 0=events_drained, 1=condition_met, 2=safety_capped
//    while (eventqty() > 0 && time() <= safetyTime) {
//        step();
//        steps++;
//        if (<condition>) { reason = 1; break; }   // omitted if no condition
//    }
//    if (time() > safetyTime && reason == 0) reason = 2;
//    out["reason"]         = reason;
//    out["final_sim_time"] = time();
//    out["steps_taken"]    = steps;
//    out["condition_value"]= (<condition>);   // only for run_until
//    return out;
//
// `condition` is the verbatim FlexScript expression from the caller. If
// empty, no condition check is emitted (run_to_end semantics).
namespace {
nlohmann::json runStepLoopShared(const char* commandName,
                                 double safetySec,
                                 const std::string& condition)
{
    // No condition → run_to_end. Native step loop (.1000084 — eventqty/step/time
    // all fsvisible). run_until keeps the executestring path below because its
    // condition is an arbitrary FlexScript expression that must be evaluated by
    // the FlexScript engine, not C++.
    if (condition.empty()) {
        nlohmann::json out;
        try {
            resetmodel(1);
            applicationcommand("switchRunning", Variant(0.0));
            long long steps = 0;
            while (eventqty() > 0 && time() <= safetySec) { step(); ++steps; }
            int reason = (time() > safetySec) ? 2 : 0;
            out["ok"]                 = true;
            out["reason"]             = (reason == 2) ? "safety_capped" : "events_drained";
            out["final_sim_time"]     = time();
            out["steps_taken"]        = steps;
            out["safety_sim_seconds"] = safetySec;
        } catch (const std::exception& e) {
            out["ok"]            = false;
            out["error_code"]    = std::string(commandName) + "_failed";
            out["error_message"] = std::string("step-loop threw: ") + e.what();
        }
        return out;
    }

    // Native (.1000100 — was executestring). The condition is arbitrary
    // FlexScript: compile it ONCE into a scratch node ("return (<cond>);"), then
    // evaluate() it natively each step. evaluate() runs the already-compiled node
    // — no per-call string compile. The C++ step loop mirrors the run_to_end path.
    nlohmann::json out;
    treenode condNode = 0;
    try {
        resetmodel(1);
        applicationcommand("switchRunning", Variant(0.0));

        // Build + compile the condition node after reset so it survives the run.
        condNode = assertsubnode(model(), "__modelerai_runUntilCond", DATATYPE_STRING);
        std::string body = "return (" + condition + ");";
        condNode->value = Variant(body.c_str());
        switch_flexscript(condNode, 1);
        buildnodeflexscript(condNode);

        // A condition is "met" only when it yields a finite, non-zero NUMBER.
        // A non-Number result (e.g. a condition that returns a treenode/string) or
        // NaN must NOT count as true — otherwise the run stops on step 1 with a
        // bogus condition_met. (d != d) is the header-free NaN test.
        auto truthy = [](const Variant& cv) {
            if (cv.type != VariantType::Number) return false;
            double d = (double)cv;
            return !(d != d) && d != 0.0;
        };

        long long steps = 0;
        int reason = 0;   // 0=events_drained 1=condition_met 2=safety_capped
        bool condVal = false;   // last in-loop evaluation — reported as condition_value
        while (eventqty() > 0 && time() <= safetySec) {
            step();
            ++steps;
            condVal = truthy(condNode->evaluate());
            if (condVal) { reason = 1; break; }
        }
        if (reason == 0 && time() > safetySec) reason = 2;
        // NOTE: no post-loop re-evaluate — that would double-fire any side effects
        // in the condition and could turn a clean run into a reported failure.

        std::string reasonStr = (reason == 1) ? "condition_met"
                              : (reason == 2) ? "safety_capped"
                                              : "events_drained";
        out["ok"]                 = true;
        out["reason"]             = reasonStr;
        out["final_sim_time"]     = time();
        out["steps_taken"]        = steps;
        out["safety_sim_seconds"] = safetySec;
        out["condition_value"]    = condVal;
    } catch (const std::exception& e) {
        out["ok"]            = false;
        out["error_code"]    = std::string(commandName) + "_failed";
        out["error_message"] = std::string("step-loop threw: ") + e.what()
                             + " — most likely a condition expression error. Expression: " + condition;
    } catch (...) {
        out["ok"]            = false;
        out["error_code"]    = std::string(commandName) + "_failed";
        out["error_message"] = "step-loop threw a non-std exception. Expression: " + condition;
    }
    if (objectexists(condNode)) { try { destroyobject(condNode); } catch (...) {} }
    return out;
}

// Parse args common to both run-loop tools.
// Supported shapes (all need safety_sim_seconds):
//   JSON   : {"safety_sim_seconds": 259200, "condition": "..."}
// For run_to_end, `condition` is unused (and rejected if supplied).
bool parseRunLoopArgs(const std::string& argStr,
                      bool wantCondition,
                      double& outSafetySec,
                      std::string& outCondition,
                      std::string& errCode,
                      std::string& errMsg)
{
    try {
        auto j = nlohmann::json::parse(argStr);
        if (!j.is_object()) {
            errCode = "bad_args_shape";
            errMsg  = "expected JSON object";
            return false;
        }
        bool haveSafety = false;
        for (const char* k : { "safety_sim_seconds", "safety", "safety_seconds" }) {
            if (j.contains(k) && j[k].is_number()) {
                outSafetySec = j[k].get<double>();
                haveSafety = true;
                break;
            }
        }
        if (!haveSafety) {
            errCode = "missing_safety_cap";
            errMsg  = "safety_sim_seconds is required. Pick a sim-time budget "
                      "the run shouldn't exceed (e.g. days(N), hours(N) — "
                      "expressed as raw seconds in JSON since it can't call "
                      "FlexScript helpers).";
            return false;
        }
        if (outSafetySec <= 0.0) {
            errCode = "bad_safety_cap";
            errMsg  = "safety_sim_seconds must be > 0.";
            return false;
        }

        if (wantCondition) {
            if (!j.contains("condition") || !j["condition"].is_string()
                || j["condition"].get<std::string>().empty())
            {
                errCode = "missing_condition";
                errMsg  = "condition (FlexScript expression string) is "
                          "required. Example: \"Model.find(\\\"Sink1\\\")"
                          ".stats.input.value >= 100\"";
                return false;
            }
            outCondition = j["condition"].get<std::string>();
        }
        return true;
    } catch (const std::exception& e) {
        errCode = "bad_args_json";
        errMsg  = std::string("parse: ") + e.what();
        return false;
    }
}
} // namespace

// modelerai_run_to_end({safety_sim_seconds})
//
// Resets, then steps until eventqty() == 0 OR sim time exceeds the safety
// cap. NO condition. step() ignores stop times — if you have stop-time
// entries you want respected, use modelerai_run_to_time.
modelerai_export Variant ModelerAi_runToEnd(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_run_to_end expects { safety_sim_seconds } JSON.");
        }
        double safetySec = 0.0;
        std::string condition;  // unused
        std::string errCode, errMsg;
        if (!parseRunLoopArgs(std::string(arg), /*wantCondition=*/false,
                              safetySec, condition, errCode, errMsg)) {
            return returnError(errCode.c_str(), errMsg);
        }
        nlohmann::json out = runStepLoopShared("run_to_end", safetySec, "");
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run_to_end", e.what()); }
      catch (...)                     { return returnException("run_to_end", "unknown"); }
}

// modelerai_run_until({condition, safety_sim_seconds})
//
// Resets, then steps until the supplied FlexScript condition evaluates true
// OR eventqty() == 0 OR sim time exceeds the safety cap. `condition` is a
// raw FlexScript expression (will be wrapped in parens at the compare
// site). step() ignores stop times.
modelerai_export Variant ModelerAi_runUntil(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_run_until expects { condition, safety_sim_seconds } JSON.");
        }
        double safetySec = 0.0;
        std::string condition;
        std::string errCode, errMsg;
        if (!parseRunLoopArgs(std::string(arg), /*wantCondition=*/true,
                              safetySec, condition, errCode, errMsg)) {
            return returnError(errCode.c_str(), errMsg);
        }
        nlohmann::json out = runStepLoopShared("run_until", safetySec, condition);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run_until", e.what()); }
      catch (...)                     { return returnException("run_until", "unknown"); }
}

// modelerai_run() — fire-and-forget go(). No reset, no wait. Use
// modelerai_get_run_state to poll status. Use modelerai_stop_model to halt.
modelerai_export Variant ModelerAi_run(FLEXSIMINTERFACE)
{
    try {
        // Reset before go() so freshly-added chart statistics-collectors bind and
        // stats start clean — charts created since the last reset don't collect
        // until a reset. Pass {reset:false} to resume without resetting.
        bool doReset = true;
        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty()) {
                try { auto j = nlohmann::json::parse(s); if (j.is_object()) doReset = j.value("reset", true); }
                catch (...) {}
            }
        }
        if (doReset) resetmodel(1);
        go();   // fsvisible
        double t = time();
        nlohmann::json out;
        out["ok"]        = true;
        out["run_state"] = "Running";
        out["sim_time"]  = t;
        out["reset"]     = doReset;
        out["note"]      = "Async — use modelerai_get_run_state to poll, "
                           "modelerai_stop_model to halt.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("run", e.what()); }
      catch (...)                     { return returnException("run", "unknown"); }
}

// modelerai_stop_model() — stop(1) + repaint. Returns sim_time at stop.
modelerai_export Variant ModelerAi_stopModel(FLEXSIMINTERFACE)
{
    try {
        // Native (.1000084 — was executestring). updatestates (allobjects.h),
        // stop/repaintall (declaration.h), applicationcommand all link.
        updatestates();
        stop(1);
        repaintall();
        applicationcommand("switchRunning", Variant(0.0));
        double t = time();
        nlohmann::json out;
        out["ok"]        = true;
        out["run_state"] = "Stopped";
        out["sim_time"]  = t;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("stop_model", e.what()); }
      catch (...)                     { return returnException("stop_model", "unknown"); }
}

// modelerai_step_model() — single step(). Ignores stop times. Returns
// sim_time after the step.
modelerai_export Variant ModelerAi_stepModel(FLEXSIMINTERFACE)
{
    try {
        // Native (.1000100 — was executestring). step()/repaintall()/time() all
        // link. (clearundohistory has no native decl in the SDK headers; dropped —
        // a single step accrues negligible undo history.)
        double t = 0.0;
        try {
            step();
            repaintall();
            t = time();
        } catch (const std::exception& e) {
            return returnError("step_failed", e.what());
        }
        nlohmann::json out;
        out["ok"]       = true;
        out["sim_time"] = t;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("step_model", e.what()); }
      catch (...)                     { return returnException("step_model", "unknown"); }
}

// modelerai_get_run_state() — non-blocking status read. Native getrunstate()
// + time(). getrunstate() is boolean (1=running, 0=not) per the FlexSim command
// reference — the old "2=Paused" branch was dead code.
modelerai_export Variant ModelerAi_getRunState(FLEXSIMINTERFACE)
{
    try {
        std::string state = (getrunstate() != 0) ? "Running" : "Stopped";
        double t = time();
        nlohmann::json out;
        out["ok"]        = true;
        out["run_state"] = state;
        out["sim_time"]  = t;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_run_state", e.what()); }
      catch (...)                     { return returnException("get_run_state", "unknown"); }
}

// modelerai_add_stop_time(seconds[, enabled, label])
//
// Adds a stop time SDT to the model's stop-times table by cloning the last
// existing entry (the toolbox widget prevents deletion of the last entry,
// so stopTimes.last is always a valid template). Clears the model-level
// stoptime slot 0 afterward so it doesn't override our entry.
//
// Does NOT auto-reset — stop times only take effect on the next reset, but
// the modeler often wants to batch-install several before running. The
// run-control tools call resetmodel(1) themselves when needed.
//
// Accepts:
//   bare number        : 600
//   array              : [600] / [600, 1] / [600, 1, "label"]
//   JSON object        : {"seconds": 600, "enabled": 1, "label": "..."}
//                        (alias: "model_time" for "seconds")
modelerai_export Variant ModelerAi_addStopTime(FLEXSIMINTERFACE)
{
    try {
        Variant arg1 = param(1);
        Variant arg2 = param(2);
        Variant arg3 = param(3);
        double seconds = 0.0;
        double enabled = 1.0;
        std::string label;
        bool haveSeconds = false;

        if (arg1.type == VariantType::Number) {
            seconds = double(arg1); haveSeconds = true;
            if (arg2.type == VariantType::Number) enabled = double(arg2);
            if (arg3.type == VariantType::String) label = std::string(arg3);
        } else if (arg1.type == VariantType::String) {
            // Via applicationcommand the arg always arrives as a string. Parse
            // it as JSON so bare number "600", array "[600,1,\"lbl\"]", and
            // object forms all work (.1000058 — previously only "{...}" did).
            std::string s = std::string(arg1);
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_number()) {
                    seconds = j.get<double>(); haveSeconds = true;
                } else if (j.is_array()) {
                    if (j.size() >= 1 && j[0].is_number()) { seconds = j[0].get<double>(); haveSeconds = true; }
                    if (j.size() >= 2 && j[1].is_number()) enabled = j[1].get<double>();
                    if (j.size() >= 3 && j[2].is_string()) label = j[2].get<std::string>();
                } else if (j.is_object()) {
                    for (const char* k : { "seconds", "model_time" }) {
                        if (j.contains(k) && j[k].is_number()) {
                            seconds = j[k].get<double>();
                            haveSeconds = true;
                            break;
                        }
                    }
                    if (j.contains("enabled")) {
                        if (j["enabled"].is_boolean())
                            enabled = j["enabled"].get<bool>() ? 1.0 : 0.0;
                        else if (j["enabled"].is_number())
                            enabled = j["enabled"].get<double>();
                    }
                    if (j.contains("label") && j["label"].is_string()) {
                        label = j["label"].get<std::string>();
                    }
                }
            } catch (...) {}
        }

        if (!haveSeconds) {
            return returnError("missing_seconds",
                "modelerai_add_stop_time expects a numeric sim time. Shapes: "
                "bare number 600, array [600, enabled?, label?], or JSON "
                "{\"seconds\": 600, \"enabled\": 1, \"label\": \"...\"}.");
        }
        if (seconds <= 0.0) {
            return returnError("bad_seconds",
                "seconds must be > 0.");
        }

        // Native (.1000084 — was executestring). getmodelunit→Variant(TreeNode);
        // createcopy clones the last entry (always-valid template); setsdtvalue +
        // function_s("setModelTime") set the fields; stoptime(0,0) clears slot 0.
        treenode stopTimes = getmodelunit(STOP_TIME_NODE);
        if (!objectexists(stopTimes)) {
            return returnError("add_stop_time_failed", "STOP_TIME_NODE not found.");
        }
        treenode newStop = createcopy(stopTimes->last, stopTimes, 1);
        if (!objectexists(newStop)) {
            return returnError("add_stop_time_failed", "createcopy returned null.");
        }
        setsdtvalue(newStop, "enabled", Variant(enabled != 0.0 ? 1.0 : 0.0));
        function_s(newStop, "setModelTime", Variant(seconds));
        if (!label.empty()) setsdtvalue(newStop, "dateString", Variant(label.c_str()));
        stoptime(0, 0);

        std::string path;
        if (const char* p = nodetomodelpath_cstr(newStop, 1)) path = p;

        if (path.empty()) {
            return returnError("add_stop_time_failed",
                "Could not add stop time — see FlexSim System Console for cause.");
        }

        nlohmann::json out;
        out["ok"]         = true;
        out["model_time"] = seconds;
        out["enabled"]    = (enabled != 0.0);
        out["path"]       = path;
        if (!label.empty()) out["label"] = label;
        out["note"]       = "Stop time installed. Call modelerai_reset_model "
                            "(or a run tool that resets first) for it to "
                            "take effect.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_stop_time", e.what()); }
      catch (...)                     { return returnException("add_stop_time", "unknown"); }
}

// modelerai_remove_stop_time(model_time)
//
// Finds the first stop-time entry whose modelTime equals the given value
// and destroys it. Refuses to destroy the last remaining stop time (the
// FlexSim toolbox widget enforces this rule and our add-stop pattern
// depends on stopTimes.last being a valid template). Clears the model-
// level stoptime slot 0 afterward, same as the add path.
//
// Returns:
//   { ok: true,  removed: true,  model_time, path }            — destroyed
//   { ok: true,  removed: false, reason: "not_found" }         — no entry matched
//   { ok: false, error_code: "last_stop_protected", ... }      — only one left
//
// Accepts:
//   bare number  : 600
//   array        : [600]
//   JSON object  : {"model_time": 600}  (alias: "seconds")
modelerai_export Variant ModelerAi_removeStopTime(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        double modelTime = 0.0;
        bool haveModelTime = false;

        if (arg.type == VariantType::Number) {
            modelTime = double(arg); haveModelTime = true;
        } else if (arg.type == VariantType::String) {
            // Parse the string arg as JSON: bare number "600", array "[600]",
            // or {"model_time":600} all work (.1000058).
            std::string s = std::string(arg);
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_number()) {
                    modelTime = j.get<double>(); haveModelTime = true;
                } else if (j.is_array()) {
                    if (j.size() >= 1 && j[0].is_number()) { modelTime = j[0].get<double>(); haveModelTime = true; }
                } else if (j.is_object()) {
                    for (const char* k : { "model_time", "seconds" }) {
                        if (j.contains(k) && j[k].is_number()) {
                            modelTime = j[k].get<double>();
                            haveModelTime = true;
                            break;
                        }
                    }
                }
            } catch (...) {}
        }

        if (!haveModelTime) {
            return returnError("missing_model_time",
                "modelerai_remove_stop_time expects the modelTime of the stop "
                "to remove. Shapes: bare number 600, array [600], or JSON "
                "{\"model_time\": 600}.");
        }

        // Native (.1000084 — was executestring). Last-stop protection BEFORE any
        // removal; match each entry's modelTime via getsdtvalue and destroyobject
        // the hit; stoptime(0,0) clears slot 0.
        treenode stopTimes = getmodelunit(STOP_TIME_NODE);
        if (!objectexists(stopTimes)) {
            return returnError("remove_stop_time_failed", "STOP_TIME_NODE not found.");
        }
        int count = content(stopTimes);
        if (count == 1) {
            return returnError("last_stop_protected",
                "Only one stop time remains; refusing to delete it. FlexSim's "
                "toolbox widget enforces the same rule, and our add-stop "
                "template depends on stopTimes.last being valid. Add another "
                "stop time first, then retry.");
        }
        int removed = 0;
        std::string path;
        for (int i = 1; i <= count; ++i) {
            treenode n = rank(stopTimes, i);
            if (!n) continue;
            Variant mt = getsdtvalue(n, "modelTime");
            if (mt.type == VariantType::Number && static_cast<double>(mt) == modelTime) {
                if (const char* p = nodetomodelpath_cstr(n, 1)) path = p;
                destroyobject(n);
                removed = 1;
                break;
            }
        }
        stoptime(0, 0);

        nlohmann::json out;
        out["ok"]         = true;
        out["removed"]    = (removed != 0);
        out["model_time"] = modelTime;
        if (removed) {
            out["path"] = path;
            out["note"] = "Stop time removed. Call modelerai_reset_model "
                          "(or a run tool that resets first) for the change "
                          "to take effect.";
        } else {
            out["reason"] = "not_found";
            out["note"]   = "No stop time matched the given modelTime "
                            "(exact equality). List existing stops to "
                            "find the right value.";
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("remove_stop_time", e.what()); }
      catch (...)                     { return returnException("remove_stop_time", "unknown"); }
}

// modelerai_set_warmup_time(seconds[, enabled])
//
// Sets the single warmup-time SDT under getmodelunit(WARMUP_TIME_NODE).
// Same function_s-write / getsdtvalue-read asymmetry as stop times.
// Does NOT auto-reset.
//
// Accepts:
//   bare number  : 100
//   array        : [100] / [100, 1]
//   JSON object  : {"seconds": 100, "enabled": 1}
//                  (alias: "model_time" for "seconds")
modelerai_export Variant ModelerAi_setWarmupTime(FLEXSIMINTERFACE)
{
    try {
        Variant arg1 = param(1);
        Variant arg2 = param(2);
        double seconds = 0.0;
        double enabled = 1.0;
        bool haveSeconds = false;

        if (arg1.type == VariantType::Number) {
            seconds = double(arg1); haveSeconds = true;
            if (arg2.type == VariantType::Number) enabled = double(arg2);
        } else if (arg1.type == VariantType::String) {
            // Parse the string arg as JSON: bare "100", "[100,1]", or
            // {"seconds":100,"enabled":1} all work (.1000058).
            std::string s = std::string(arg1);
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_number()) {
                    seconds = j.get<double>(); haveSeconds = true;
                } else if (j.is_array()) {
                    if (j.size() >= 1 && j[0].is_number()) { seconds = j[0].get<double>(); haveSeconds = true; }
                    if (j.size() >= 2 && j[1].is_number()) enabled = j[1].get<double>();
                } else if (j.is_object()) {
                    for (const char* k : { "seconds", "model_time" }) {
                        if (j.contains(k) && j[k].is_number()) {
                            seconds = j[k].get<double>();
                            haveSeconds = true;
                            break;
                        }
                    }
                    if (j.contains("enabled")) {
                        if (j["enabled"].is_boolean())
                            enabled = j["enabled"].get<bool>() ? 1.0 : 0.0;
                        else if (j["enabled"].is_number())
                            enabled = j["enabled"].get<double>();
                    }
                }
            } catch (...) {}
        }

        if (!haveSeconds) {
            return returnError("missing_seconds",
                "modelerai_set_warmup_time expects a numeric warmup duration "
                "in sim time. Shapes: bare number 100, array [100, enabled?], "
                "or JSON {\"seconds\": 100, \"enabled\": 1}.");
        }
        if (seconds < 0.0) {
            return returnError("bad_seconds",
                "seconds must be >= 0.");
        }

        // Native (.1000084 — was executestring). Same getmodelunit + setsdtvalue +
        // function_s("setModelTime") pattern as add_stop_time.
        treenode wu = getmodelunit(WARMUP_TIME_NODE);
        if (!objectexists(wu)) {
            return returnError("set_warmup_failed", "WARMUP_TIME_NODE not found.");
        }
        setsdtvalue(wu, "enabled", Variant(enabled != 0.0 ? 1.0 : 0.0));
        function_s(wu, "setModelTime", Variant(seconds));

        nlohmann::json out;
        out["ok"]         = true;
        out["model_time"] = seconds;
        out["enabled"]    = (enabled != 0.0);
        out["note"]       = "Warmup time set. Call modelerai_reset_model "
                            "(or a run tool that resets first) for it to "
                            "take effect.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_warmup_time", e.what()); }
      catch (...)                     { return returnException("set_warmup_time", "unknown"); }
}

// modelerai_set_run_speed(speed) — wraps runspeed(N). 1.0 = real-time.
// Higher numbers = faster. INT_MAX = max. Accepted shapes:
//   bare number  : 16
//   array        : [16]
//   JSON         : {"speed": 16}
modelerai_export Variant ModelerAi_setRunSpeed(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        double speed = -1.0;
        bool haveSpeed = false;

        if (arg.type == VariantType::Number) {
            speed = double(arg); haveSpeed = true;
        } else if (arg.type == VariantType::String) {
            // Parse the string arg as JSON: bare "16", "[16]", {"speed":16}.
            std::string s = std::string(arg);
            try {
                auto j = nlohmann::json::parse(s);
                if (j.is_number()) {
                    speed = j.get<double>(); haveSpeed = true;
                } else if (j.is_array()) {
                    if (j.size() >= 1 && j[0].is_number()) { speed = j[0].get<double>(); haveSpeed = true; }
                } else if (j.is_object() && j.contains("speed") && j["speed"].is_number()) {
                    speed = j["speed"].get<double>();
                    haveSpeed = true;
                }
            } catch (...) {}
        }

        if (!haveSpeed) {
            return returnError("missing_speed",
                "modelerai_set_run_speed expects a numeric speed. "
                "Shapes: bare number 16, array [16], or JSON "
                "{\"speed\": 16}. 1.0 = real-time, higher = faster.");
        }
        if (speed <= 0.0) {
            return returnError("bad_speed",
                "speed must be > 0.");
        }

        // Native (.1000084 — was executestring). runspeed(n) sets and returns
        // the new speed (fsvisible: runspeed(double n1, double n2=-1)).
        double actual = runspeed(speed);

        nlohmann::json out;
        out["ok"]        = true;
        out["run_speed"] = actual;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_run_speed", e.what()); }
      catch (...)                     { return returnException("set_run_speed", "unknown"); }
}

// (legacy run-tool bodies removed; the shells above are the only
// implementations of those functions now.)

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

// PM tree-walk helpers (defined after the PM read tools below; forward-declared
// here so the create/delete tools can share them).
static TreeNode* findPmNodeByName(const std::string& name);
static TreeNode* owningPmTable(TreeNode* pm);

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
            // Accept `body` as an alias for `expression` (.1000085 — the agent
            // naturally reaches for "body", matching set_label/cell flexscript).
            bool hasExpr = (j.contains("expression") && j["expression"].is_string())
                        || (j.contains("body")       && j["body"].is_string());
            if (!j.contains("name") || !j["name"].is_string() || !hasExpr) {
                return returnError("missing_field",
                    "create_performance_measure requires `name` and `expression` (alias: `body`).");
            }
            name          = j["name"].get<std::string>();
            expression    = (j.contains("expression") && j["expression"].is_string())
                          ? j["expression"].get<std::string>()
                          : j["body"].get<std::string>();
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

        // Native (.1000095 — was an ~80-line executestring builder). Locate or
        // create the PerformanceMeasureTable, allocate a row via the table's own
        // addPfm node-function (function_s — the same call the FlexScript made,
        // now invoked natively), then write Name / Value.valueNode / reference /
        // Description / Display Units directly. The valueNode body stays
        // FlexScript (user code in the tree), compiled via switch_flexscript +
        // buildnodeflexscript — never executestring.

        // Table lookup: exact name match, else first existing table, else create.
        treenode tbl = 0;
        TreeNode* container = model()->find("Tools/PerformanceMeasureTables");
        if (!tableName.empty() && container) {
            int nc = content(container);
            for (int i = 1; i <= nc; ++i) {
                TreeNode* t = rank(container, i);
                if (t && std::string(getname(t)) == tableName) { tbl = t; break; }
            }
        }
        if (!tbl && container && content(container) > 0) tbl = rank(container, 1);
        if (!tbl) {
            tbl = Tools::create("PerformanceMeasureTable");
            if (!objectexists(tbl)) {
                return returnError("create_pm_failed", "Tools::create(\"PerformanceMeasureTable\") returned null.");
            }
            setname(tbl, tableName.empty() ? "PerformanceMeasures" : tableName.c_str());
        }

        // Uniqueness: PM names are global (Model.performanceMeasures[name] takes no
        // table arg). Reuse the shared walk; a hit in THIS table is replace-or-reject,
        // a hit in another table is always rejected.
        if (TreeNode* dup = findPmNodeByName(name)) {
            TreeNode* dupTbl = owningPmTable(dup);
            if (dupTbl == tbl) {
                if (replace) destroyobject(dup);
                else return returnError("duplicate_name",
                    "A performance measure named '" + name + "' already exists in this table (use replace=true).");
            } else {
                return returnError("duplicate_name_other_table",
                    "A performance measure named '" + name + "' already exists in table '"
                    + (dupTbl ? std::string(getname(dupTbl)) : std::string("?"))
                    + "'. Performance-measure names must be globally unique.");
            }
        }

        // Allocate the row via the table's addPfm method (gives a properly-formed
        // row with Name / Value.valueNode / Description / Display Units subnodes).
        treenode pm = 0;
        {
            Variant pmv = function_s(tbl, "addPfm");
            if (pmv.type == VariantType::TreeNode) pm = static_cast<treenode>(pmv);
        }
        if (!objectexists(pm)) {
            return returnError("create_pm_failed", "addPfm did not return a valid row node.");
        }

        // Name.
        assertsubnode(pm, "Name", DATATYPE_STRING)->value = Variant(name.c_str());

        // Value.valueNode = FlexScript body (S-flagged, compiled in-tree).
        treenode v  = assertsubnode(pm, "Value", 0);
        treenode vn = assertsubnode(v, "valueNode", DATATYPE_STRING);
        vn->value = Variant(finalExpression.c_str());
        switch_flexscript(vn, 1);
        buildnodeflexscript(vn);

        // Optional reference coupling + metadata. The reference must be a real
        // one-way node-COUPLING (what FlexScript's `node.value = treenode` creates)
        // so the PM eval can pass the object as param(1). pointTo() is the
        // documented coupling primitive; evaluatePmNode passes Value/reference back
        // in as param(1) at read time.
        if (!referencePath.empty()) {
            treenode refn = model()->find(referencePath.c_str());
            if (objectexists(refn)) {
                treenode refNode = assertsubnode(v, "reference", 0);
                refNode->pointTo(refn);
            }
        }
        if (!description.empty()) {
            assertsubnode(pm, "Description", DATATYPE_STRING)->value = Variant(description.c_str());
        }
        if (!units.empty()) {
            assertsubnode(pm, "Display Units", DATATYPE_STRING)->value = Variant(units.c_str());
        }

        std::string newPath;
        if (const char* p = nodetomodelpath_cstr(pm, 1)) newPath = std::string(p);
        std::string owningTable = std::string(getname(tbl));

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
        // resolveNameArg accepts a bare string OR {name:"..."} — strParam did
        // not parse the JSON object form, so {name:"X"} arrived as the literal
        // string and was used wholesale as the PM name (.1000058 fix).
        std::string name = resolveNameArg(param(1), "name");
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_delete_performance_measure(name) requires a name.");
        }

        // Native (.1000084 — was executestring). Walk Tools/PerformanceMeasureTables;
        // performanceMeasures lives in each table's bound >variables space, reached
        // via getvarnode (NOT t.find("variables") — the .1000058 traversal bug).
        // Match each PM's Name node and destroyobject the hit.
        TreeNode* allTables = model()->find("Tools/PerformanceMeasureTables");
        bool removed = false;
        if (allTables) {
            int nt = content(allTables);
            for (int iT = 1; iT <= nt && !removed; ++iT) {
                TreeNode* t = rank(allTables, iT);
                if (!t) continue;
                TreeNode* pms = getvarnode(t, "performanceMeasures");
                if (!pms) continue;
                int np = content(pms);
                for (int iP = 1; iP <= np; ++iP) {
                    TreeNode* pm = rank(pms, iP);
                    if (!pm) continue;
                    TreeNode* nm = pm->find("Name");
                    if (nm && std::string(nm->value) == name) {
                        destroyobject(pm);
                        removed = true;
                        break;
                    }
                }
            }
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
        // Use the engine accessor API (Model.performanceMeasures.tableNames /
        // .names(table)) — the same pattern list_parameters uses. The previous
        // hand-rolled tree-walk via Model.find(...)/find("variables")/
        // find("performanceMeasures") did NOT traverse the table's bound
        // `>variables` attribute space, so it returned zero rows even when PMs
        // existed (.1000058 fix).
        // Native (.1000084 — was executestring). Pure enumeration via the engine
        // accessor Model::getPerformanceMeasures().tableNames / .names(table) —
        // both engine_export (no PM body evaluation here; metadata only).
        nlohmann::json results = nlohmann::json::array();
        PerformanceMeasures pms = Model::getPerformanceMeasures();
        Array tableNames = pms.tableNames;
        for (int t = 1; t <= tableNames.length; ++t) {
            std::string tn = std::string(tableNames[t]);
            Array names = pms.names(tn.c_str());
            for (int i = 1; i <= names.length; ++i) {
                nlohmann::json row;
                row["table"] = tn;
                row["name"]  = std::string(names[i]);
                results.push_back(std::move(row));
            }
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

// Native PM lookup: walk every PerformanceMeasureTable's bound performanceMeasures
// var space and return the PM row node whose Name matches (or nullptr). Mirrors the
// delete_performance_measure walk — getvarnode reaches the bound >variables space
// that t.find("variables") can't traverse (the .1000058 traversal bug).
static TreeNode* findPmNodeByName(const std::string& name)
{
    TreeNode* allTables = model()->find("Tools/PerformanceMeasureTables");
    if (!allTables) return nullptr;
    int nt = content(allTables);
    for (int iT = 1; iT <= nt; ++iT) {
        TreeNode* t = rank(allTables, iT);
        if (!t) continue;
        TreeNode* pms = getvarnode(t, "performanceMeasures");
        if (!pms) continue;
        int np = content(pms);
        for (int iP = 1; iP <= np; ++iP) {
            TreeNode* pm = rank(pms, iP);
            if (!pm) continue;
            TreeNode* nm = pm->find("Name");
            if (nm && std::string(nm->value) == name) return pm;
        }
    }
    return nullptr;
}

// Walk up from a PM row node to its owning PerformanceMeasureTable. Layout is
// <table>/>variables/performanceMeasures/<row>, so the table is three parents up.
static TreeNode* owningPmTable(TreeNode* pm)
{
    if (!pm || !pm->up || !pm->up->up) return nullptr;
    return pm->up->up->up;   // row -> performanceMeasures -> >variables -> table
}

// Parameter analogues of the PM helpers. Parameters live in each table's bound
// `parameters` var space (getvarnode — never find("variables")); names are global
// across all ParameterTables, same as PMs. Mirrors the remove_parameter walk.
static TreeNode* findParamNodeByName(const std::string& name)
{
    TreeNode* allTables = model()->find("Tools/ParameterTables");
    if (!allTables) return nullptr;
    int nt = content(allTables);
    for (int iT = 1; iT <= nt; ++iT) {
        TreeNode* t = rank(allTables, iT);
        if (!t) continue;
        TreeNode* ps = getvarnode(t, "parameters");
        if (!ps) continue;
        int np = content(ps);
        for (int iP = 1; iP <= np; ++iP) {
            TreeNode* p = rank(ps, iP);
            if (!p) continue;
            TreeNode* nm = p->find("Name");
            if (nm && std::string(nm->value) == name) return p;
        }
    }
    return nullptr;
}

// row -> parameters -> >variables -> table (three parents up).
static TreeNode* owningParamTable(TreeNode* p)
{
    if (!p || !p->up || !p->up->up) return nullptr;
    return p->up->up->up;
}

// Evaluate a PM row node natively. Its Value/valueNode child holds the FlexScript
// body; TreeNode::evaluate() runs the already-compiled node (engine_export instance
// method — no executestring). FlexSim's PM framework passes the row's reference
// coupling as param(1) (the body's `treenode reference = param(1)`); a bare
// evaluate() leaves reference null, so getstat(reference,...) returns maxnum. Pass
// the Value/reference node when the row has one so reference-based PMs resolve.
static Variant evaluatePmNode(TreeNode* pm)
{
    if (!pm) return Variant();
    TreeNode* valNode = pm->find("Value");
    if (!valNode) return Variant();
    TreeNode* vn      = valNode->find("valueNode");
    TreeNode* refNode = valNode->find("reference");
    TreeNode* target  = vn ? vn : valNode;
    if (refNode) {
        // reference is a one-way coupling; the PM body expects the DEREFERENCED
        // object as param(1), not the coupling node (passing the node itself made
        // `reference` resolve to the node named "reference", so getstat failed).
        // CouplingDataType::evaluate() returns partner().get(), so evaluating the
        // reference node yields the referenced object.
        Variant refTarget = refNode->evaluate();
        return target->evaluate(refTarget);
    }
    return target->evaluate();
}

// ============================================================================
// modelerai_get_performance_measure(name)
//   Evaluates a single PM and returns the value. Marshalled through
//   pmValueToJson — supports number, string, treenode (as path), array, map.
// ============================================================================
modelerai_export Variant ModelerAi_getPerformanceMeasure(FLEXSIMINTERFACE)
{
    try {
        // Accept bare string OR {name:"..."} (.1000058 fix — was strParam,
        // which left the JSON object form unparsed).
        std::string name = resolveNameArg(param(1), "name");
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_get_performance_measure(name) requires a name.");
        }
        // Native (.1000094 — was executestring). Locate the PM row by name across
        // every table, then run its valueNode via TreeNode::evaluate(). The PM body
        // stays FlexScript (user code in the tree) but we invoke it with a native
        // engine call instead of compiling a generated string. (.1000092 — `.value`
        // raised "StatusVariable does not support property value"; the node has to
        // be evaluated, which evaluatePmNode does.)
        TreeNode* pm = findPmNodeByName(name);
        if (!pm) {
            return returnError("pm_not_found",
                "No performance measure named '" + name + "' in any table.");
        }
        nlohmann::json value;
        try {
            value = pmValueToJson(evaluatePmNode(pm));
        } catch (const std::exception& e) {
            return returnError("eval_failed", std::string("Evaluating PM `") + name + "` threw: " + e.what());
        } catch (const char* msg) {
            return returnError("eval_failed", std::string("Evaluating PM `") + name + "` threw: " + (msg ? msg : "FlexScript error"));
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
        // Native (.1000094 — was two executestring calls: one to enumerate names,
        // one per PM to evaluate). Single tree-walk over every table's bound
        // performanceMeasures space; evaluate each row's valueNode via
        // TreeNode::evaluate(). One bad PM body is caught per-row so it doesn't
        // kill the whole snapshot. Same walk as findPmNodeByName /
        // delete_performance_measure (getvarnode reaches the >variables space).
        nlohmann::json values = nlohmann::json::object();
        nlohmann::json errors = nlohmann::json::object();
        int count = 0;
        TreeNode* allTables = model()->find("Tools/PerformanceMeasureTables");
        if (allTables) {
            int nt = content(allTables);
            for (int iT = 1; iT <= nt; ++iT) {
                TreeNode* t = rank(allTables, iT);
                if (!t) continue;
                TreeNode* pms = getvarnode(t, "performanceMeasures");
                if (!pms) continue;
                int np = content(pms);
                for (int iP = 1; iP <= np; ++iP) {
                    TreeNode* pm = rank(pms, iP);
                    if (!pm) continue;
                    TreeNode* nm = pm->find("Name");
                    if (!nm) continue;
                    std::string pmName = std::string(nm->value);
                    if (pmName.empty()) continue;
                    ++count;
                    try {
                        values[pmName] = pmValueToJson(evaluatePmNode(pm));
                    } catch (const std::exception& e) {
                        errors[pmName] = e.what();
                    } catch (const char* msg) {
                        errors[pmName] = msg ? msg : "FlexScript error";
                    } catch (...) {
                        errors[pmName] = "unknown exception";
                    }
                }
            }
        }

        nlohmann::json out;
        out["ok"]                   = true;
        out["count"]                = count;
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
        // Accept bare string OR {object:"..."} (.1000058 fix — was strParam,
        // which left the JSON object form unparsed and used the whole arg
        // string as the object name).
        std::string objName = resolveNameArg(param(1), "object");
        double verbose      = numParam(param(2), 0.0);
        (void)verbose;  // reserved for future expansion
        if (objName.empty()) {
            return returnError("missing_object_name",
                "modelerai_get_object_stats(object_name) requires a name.");
        }

        // Native C++ (.1000079 — was executestring building a FlexScript Map).
        // Uses the same accessors as FlexSim's PerformanceMeasure picklists:
        // getinput/getoutput for counts, getstat(obj,"Content"/"Staytime",
        // STAT_*) for content + staytime, getvarnum(obj,"totaltraveldist")
        // for movers, defaultstatelist walking for state times. All of these
        // are fsvisible free functions that link from the module DLL.
        //
        // Percentage calcs use statisticaltime() (excludes warmup) NOT time();
        // throughput rate uses time() since that's the wall sim time.
        TreeNode* o = model()->find(objName.c_str());
        if (!o || !objectexists(o)) {
            return returnError("object_not_found",
                "Object '" + objName + "' did not resolve.");
        }

        nlohmann::json stats;
        stats["object_name"] = objName;
        if (TreeNode* cls = classobject(o)) {
            if (objectexists(cls)) stats["class_name"] = std::string(getname(cls));
        }

        // Throughput counts. Sinks RECEIVE items (read input_count); Source/
        // Processor/Queue read output_count. We return both.
        stats["input_count"]  = getinput(o);
        stats["output_count"] = getoutput(o);

        // Content stats via getstat (matches GUI picklist).
        stats["content_current"] = getstat(o, "Content", STAT_CURRENT);
        stats["content_min"]     = getstat(o, "Content", STAT_MIN);
        stats["content_max"]     = getstat(o, "Content", STAT_MAX);
        stats["content_avg"]     = getstat(o, "Content", STAT_AVERAGE);

        // Throughput per hour from output count.
        double simTime  = time();
        double simHours = simTime / 3600.0;
        if (simHours > 0.0) stats["throughput_per_hour"] = getoutput(o) / simHours;

        // Staytime via getstat — Min/Max/Average.
        stats["staytime_min"]  = getstat(o, "Staytime", STAT_MIN);
        stats["staytime_max"]  = getstat(o, "Staytime", STAT_MAX);
        stats["staytime_mean"] = getstat(o, "Staytime", STAT_AVERAGE);

        // Total travel distance — only TaskExecuters/movers have the var.
        if (TreeNode* ttd = getvarnode(o, "totaltraveldist")) {
            if (objectexists(ttd)) stats["total_travel_distance"] = getvarnum(o, "totaltraveldist");
        }

        // State times: walk defaultstatelist, percent of statisticaltime().
        double statTime = statisticaltime();
        if (statTime > 0.0) {
            if (TreeNode* dsl = node("MAIN:/project/exec/globals/defaultstatelist")) {
                nlohmann::json sm = nlohmann::json::object();
                int sc = content(dsl);
                for (int i = 1; i <= sc; ++i) {
                    TreeNode* sn = rank(dsl, i);
                    if (!sn) continue;
                    std::string stateName = std::string(getname(sn));
                    if (stateName.empty()) continue;
                    double t = getstat(o, "State", STAT_TIME_AT_VALUE,
                                       Variant(0.0), Variant(stateName.c_str()));
                    if (t > 0.0) sm[stateName] = 100.0 * t / statTime;
                }
                if (!sm.empty()) stats["state_percent"] = std::move(sm);
            }
        }

        stats["sim_time"]         = simTime;
        stats["statistical_time"] = statTime;

        nlohmann::json out;
        out["ok"]    = true;
        out["stats"] = std::move(stats);
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
        // Native C++ (.1000079 — was executestring building a FlexScript Map).
        // Walk the model's direct children, group by class, accumulate
        // Source output / Sink input, and (when stopped) flag any FR holding
        // content as "stuck". All accessors are fsvisible free functions.
        nlohmann::json summary;
        double simTime = time();
        summary["sim_time"] = simTime;

        int rs = getrunstate();   // boolean 0/1 — no "Paused" state
        summary["run_state"] = rs == 0 ? "Stopped" : "Running";

        nlohmann::json classCounts = nlohmann::json::object();
        double totalCreated = 0.0;
        double totalSunk    = 0.0;
        nlohmann::json stuckObjects = nlohmann::json::array();

        TreeNode* root = model();
        int n = content(root);
        for (int i = 1; i <= n; ++i) {
            TreeNode* o = rank(root, i);
            if (!o) continue;
            TreeNode* cls = classobject(o);
            if (!cls) continue;
            std::string cn = std::string(getname(cls));
            if (cn.empty()) continue;

            // Per-class count.
            if (classCounts.contains(cn))
                classCounts[cn] = classCounts[cn].get<double>() + 1.0;
            else
                classCounts[cn] = 1.0;

            // Totals + stuck-objects check, gated on class name so we only
            // read stats on classes that actually expose them.
            if (cn == "Source") totalCreated += getoutput(o);
            if (cn == "Sink")   totalSunk    += getinput(o);

            if (rs == 0 && (cn == "Queue" || cn == "Processor" ||
                            cn == "Combiner" || cn == "Separator" || cn == "BasicFR")) {
                double cnt = getstat(o, "Content", STAT_CURRENT);
                if (cnt > 0.0) {
                    nlohmann::json s;
                    s["name"]    = std::string(getname(o));
                    s["class"]   = cn;
                    s["content"] = cnt;
                    stuckObjects.push_back(std::move(s));
                }
            }
        }

        summary["class_counts"]  = std::move(classCounts);
        summary["total_created"] = totalCreated;
        summary["total_sunk"]    = totalSunk;
        summary["flow_balance"]  = totalCreated - totalSunk;
        summary["stuck_objects"] = std::move(stuckObjects);

        nlohmann::json out;
        out["ok"]      = true;
        out["summary"] = std::move(summary);
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

        // Native read via ObjectDataType::getProperty. The Variant→JSON switch
        // below handles every shape, including Array (Vec3/Color come back as a
        // numeric Array Variant). schemaEntry still drives value_kind reporting.
        ObjectDataType* odt = obj->object<ObjectDataType>();
        if (!odt) {
            return returnError("getproperty_failed", "could not obtain object data from node.");
        }
        Variant result;
        try {
            result = odt->getProperty(propertyName.c_str());
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

// Native equivalents of the group resolvers above (Group::global doesn't link,
// so resolve the group node by name under Tools/Groups). .1000084+ migration.
TreeNode* resolveGroupNode(const std::string& groupName)
{
    if (TreeNode* groups = model()->find("Tools/Groups")) {
        int n = content(groups);
        for (int i = 1; i <= n; ++i) {
            TreeNode* c = rank(groups, i);
            if (c && std::string(getname(c)) == groupName) return c;
        }
    }
    return nullptr;
}

// Member resolve: Model.find first (the common "Op1" case), then fall back to a
// Group node so the AI can nest groups by passing a group name as a member.
TreeNode* resolveMemberToNode(const std::string& memberName)
{
    TreeNode* obj = model()->find(memberName.c_str());
    if (objectexists(obj)) return obj;
    return resolveGroupNode(memberName);
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

        // Native (.1000084 — was executestring). Idempotent on name: resolveGroupNode
        // checks existence (no Tools::get needed); Tools::create("Group") makes a
        // new one when absent. Members use object<Group>() + addMember (the proven
        // group_add_member path). Tools::create is the one static factory here —
        // build-test confirms linkage (like ObjectDataType::create).
        treenode existing = resolveGroupNode(groupName);
        bool wasExisting  = objectexists(existing);
        treenode gNode    = existing;
        if (!wasExisting) {
            gNode = Tools::create("Group");
            if (!objectexists(gNode)) {
                return returnError("create_group_failed", "Tools::create(\"Group\") returned null.");
            }
            setname(gNode, groupName.c_str());
        }
        Group* gref = gNode->object<Group>();
        if (!gref) return returnError("create_group_failed", "node is not a Group.");

        int added = 0, skipped = 0;
        nlohmann::json nf = nlohmann::json::array();
        for (const auto& m : initialMembers) {
            TreeNode* obj = resolveMemberToNode(m);
            if (!objectexists(obj))          nf.push_back(m);
            else if (gref->isMember(obj, 0)) ++skipped;
            else { gref->addMember(obj);     ++added; }
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["name"]     = groupName;
        out["existing"] = wasExisting;
        if (const char* p = nodetomodelpath_cstr(gNode, 1)) out["path"] = std::string(p);
        out["member_count"] = gref->length();
        if (!initialMembers.empty()) {
            out["added"]     = added;
            out["skipped"]   = skipped;
            out["not_found"] = std::move(nf);
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_group", e.what()); }
      catch (...)                     { return returnException("create_group", "unknown"); }
}

// ============================================================================
// modelerai_create_user_command({ name, code, description?, parameters?,
//                                 example?, return_type?, short_description? })
//
// Creates (or replaces) a persistent FlexSim User Command under
// Tools/UserCommands/<name>, then registers it live so it's callable from
// FlexScript immediately (and re-registers on model reload via the node
// structure). Mirrors the tree layout FlexSim's own Tools|User Commands
// builds: a <name> container whose children are
//   code             (FlexScript body — flagged as a FlexScript node)
//   description       (string)
//   parameters        (signature/param doc string, e.g. "(num x, str label)")
//   example           (string, e.g. "myCommand(1, \"hi\")")
//   returntype        (string: "num" | "string" | "variant")
//   shortdescription  (string)
// Registration uses addcommand(codeNode, name, desc, params, example, "",
// RETURN_TYPE_*) + refreshcommandlist().
//
// Idempotent on name: an existing command of the same name is destroyed and
// rebuilt (same as FlexSim's drop handler).
// ============================================================================
modelerai_export Variant ModelerAi_createUserCommand(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_user_command expects { name, code, description?, "
                "parameters?, example?, return_type?, short_description? } JSON.");
        }
        std::string name, code, description, parameters, example, shortDescription;
        std::string returnType = "num";
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            name             = j.value("name",              std::string(""));
            code             = j.value("code",              std::string(""));
            description      = j.value("description",       std::string(""));
            parameters       = j.value("parameters",        std::string(""));
            example          = j.value("example",           std::string(""));
            shortDescription = j.value("short_description",  std::string(""));
            returnType       = j.value("return_type",       std::string("num"));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (name.empty()) return returnError("missing_name", "name is required.");
        if (code.empty()) return returnError("missing_code", "code (the FlexScript body) is required.");

        // Map the return-type string to FlexSim's RETURN_TYPE_* int + the node
        // label FlexSim stores ("num"/"string"/"variant").
        int returnTypeInt = RETURN_TYPE_NUMBER;
        std::string returnTypeLabel = "num";
        {
            std::string rt;
            for (char c : returnType) rt.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
            if      (rt == "" || rt == "num" || rt == "number" || rt == "double") { returnTypeInt = RETURN_TYPE_NUMBER;  returnTypeLabel = "num"; }
            else if (rt == "string" || rt == "str" || rt == "text")               { returnTypeInt = RETURN_TYPE_STRING;  returnTypeLabel = "string"; }
            else if (rt == "variant")                                             { returnTypeInt = RETURN_TYPE_VARIANT; returnTypeLabel = "variant"; }
            else {
                return returnError("bad_return_type",
                    "return_type must be one of: num, string, variant. Got: '" + returnType + "'.");
            }
        }

        // Assert Tools/UserCommands and the command container (replace existing).
        treenode tools = model()->find("Tools");
        if (!objectexists(tools)) return returnError("tools_not_found", "model Tools node not found.");
        treenode folder = assertsubnode(tools, "UserCommands", 0);
        if (!objectexists(folder)) return returnError("usercommands_failed", "could not assert Tools/UserCommands.");

        bool replaced = false;
        if (treenode existing = folder->find(name.c_str())) {
            if (objectexists(existing)) { destroyobject(existing); replaced = true; }
        }
        treenode cmd = assertsubnode(folder, name.c_str(), 0);
        if (!objectexists(cmd)) return returnError("create_failed", "could not create the command node.");

        // Build the child nodes (string-typed). The code node is flagged as a
        // FlexScript node and compiled.
        treenode codeNode = assertsubnode(cmd, "code", DATATYPE_STRING);
        codeNode->value = Variant(code.c_str());
        switch_flexscript(codeNode, 1);
        buildnodeflexscript(codeNode);

        assertsubnode(cmd, "description",      DATATYPE_STRING)->value = Variant(description.c_str());
        assertsubnode(cmd, "parameters",       DATATYPE_STRING)->value = Variant(parameters.c_str());
        assertsubnode(cmd, "example",          DATATYPE_STRING)->value = Variant(example.c_str());
        assertsubnode(cmd, "returntype",       DATATYPE_STRING)->value = Variant(returnTypeLabel.c_str());
        assertsubnode(cmd, "shortdescription", DATATYPE_STRING)->value = Variant(shortDescription.c_str());

        // Register live so it's callable now (and validates the body compiles).
        addcommand(codeNode, name.c_str(), description.c_str(), parameters.c_str(),
                   example.c_str(), "", returnTypeInt);
        refreshcommandlist();

        nlohmann::json out;
        out["ok"]          = true;
        out["name"]        = name;
        out["replaced"]    = replaced;
        out["return_type"] = returnTypeLabel;
        if (const char* p = nodetomodelpath_cstr(cmd, 1)) out["path"] = std::string(p);
        out["note"]        = "User command registered and callable from FlexScript now. "
                             "It persists under Tools/UserCommands and re-registers on model reload.";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_user_command", e.what()); }
      catch (...)                     { return returnException("create_user_command", "unknown"); }
}

// ============================================================================
// modelerai_create_dashboard(json | "name")
//   json: { name?, parameter_table?, open? }
//
// Creates a Dashboard via the engine command (the same one the GUI uses):
//   general      → applicationcommand("adddashboard")
//   from a table → applicationcommand("adddashboard", <ParameterTable node>)
// Dashboards live under Tools/Dashboards. Optionally rename + open it
// (applicationcommand("dashboard", node)). NOTE on the dashboard lifecycle:
// a dashboard round-trips its contents tree<->view on open/close, so WIDGETS
// must be added through the open dashboard (a separate add-chart tool), not by
// editing the tree while it's open. This tool only creates/names/opens.
// ============================================================================
modelerai_export Variant ModelerAi_createDashboard(FLEXSIMINTERFACE)
{
    try {
        std::string name, paramTable;
        bool openIt = false;
        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty()) {
                bool parsed = false;
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) {
                        name       = j.value("name", std::string(""));
                        paramTable = j.value("parameter_table", std::string(""));
                        openIt     = j.value("open", false);
                        parsed = true;
                    } else if (j.is_string()) {
                        name = j.get<std::string>(); parsed = true;
                    }
                } catch (...) {}
                if (!parsed) name = s;   // not JSON → bare dashboard name
            }
        }

        // Optional source parameter table.
        treenode ptNode = 0;
        if (!paramTable.empty()) {
            std::string path = "Tools/ParameterTables/" + paramTable;
            ptNode = node(path.c_str(), model());
            if (!objectexists(ptNode)) ptNode = model()->find(path.c_str());
            if (!objectexists(ptNode)) {
                return returnError("parameter_table_not_found",
                    "parameter_table not found under Tools/ParameterTables: " + paramTable);
            }
        }

        // Create via the engine command; it returns the new dashboard node.
        treenode dash = 0;
        Variant r = objectexists(ptNode)
            ? applicationcommand("adddashboard", Variant(ptNode))
            : applicationcommand("adddashboard");
        if (r.type == VariantType::TreeNode) dash = static_cast<treenode>(r);
        // Fallback: grab the newest child of Tools/Dashboards if no node returned.
        if (!objectexists(dash)) {
            treenode container = model()->find("Tools/Dashboards");
            if (objectexists(container) && content(container) > 0)
                dash = rank(container, content(container));
        }
        if (!objectexists(dash)) {
            return returnError("create_failed", "adddashboard did not produce a dashboard node.");
        }

        if (!name.empty()) setname(dash, name.c_str());

        // adddashboard creates AND opens the dashboard — `dash` is its live VIEW
        // node — and it STAYS open so the modeler can see it (like a placed 3D
        // object). The persistent /Tools/Dashboards/<name> node only materializes
        // once it's closed; add_dashboard_chart finds the open one in the view.
        (void)openIt;   // always open after adddashboard; kept for arg compatibility
        std::string effName = std::string(getname(dash));
        nlohmann::json out;
        out["ok"]       = true;
        out["name"]     = effName;
        out["opened"]   = true;
        if (!paramTable.empty()) out["from_parameter_table"] = paramTable;
        if (const char* p = nodetomodelpath_cstr(dash, 1)) out["view_path"] = std::string(p);
        out["accessor"] = "Model.find(\"Tools/Dashboards/" + effName + "\")  (resolves once the tab is closed)";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_dashboard", e.what()); }
      catch (...)                     { return returnException("create_dashboard", "unknown"); }
}

// ============================================================================
// modelerai_list_chart_templates(json | "category")
//   json: { category? }   (filter to one category folder, e.g. "Output")
//
// Read-only catalog of every dashboard chart template under
// MAIN:/project/exec/globals/ChartTemplateDefinitions/<category>/<name>.
// Returns [{ category, name, template_path }].
//
// NOTE: the live definition nodes are empty leaves (content==0) — their
// menuname/variables/replacements aren't readable here. That's fine:
// createGraphWindow consumes a template BY PATH and materializes the real,
// traversable structure into /Tools/Statistics + /Tools/ChartTemplates (which
// add_dashboard_chart then edits). So this tool is purely the discovery catalog;
// template_path is exactly what add_dashboard_chart needs. (See
// docs/dashboards/chart-subsystem-map.html.)
// ============================================================================
modelerai_export Variant ModelerAi_listChartTemplates(FLEXSIMINTERFACE)
{
    try {
        std::string catFilter;
        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty()) {
                bool parsed = false;
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) { catFilter = j.value("category", std::string("")); parsed = true; }
                    else if (j.is_string()) { catFilter = j.get<std::string>(); parsed = true; }
                } catch (...) {}
                if (!parsed) catFilter = s;
            }
        }

        treenode defs = node("MAIN:/project/exec/globals/ChartTemplateDefinitions", nullptr);
        if (!objectexists(defs)) {
            return returnError("definitions_not_found",
                "ChartTemplateDefinitions not found at "
                "MAIN:/project/exec/globals/ChartTemplateDefinitions.");
        }

        nlohmann::json templates = nlohmann::json::array();
        int nCat = content(defs);
        for (int ci = 1; ci <= nCat; ++ci) {
            treenode cat = rank(defs, ci);
            if (!cat) continue;
            std::string catName = getname(cat);
            if (catName.empty()) continue;
            if (!catFilter.empty() && catName != catFilter) continue;

            int nT = content(cat);
            for (int ti = 1; ti <= nT; ++ti) {
                treenode T = rank(cat, ti);
                if (!T) continue;
                std::string tname = getname(T);
                if (tname.empty()) continue;

                nlohmann::json e;
                e["category"]      = catName;
                e["name"]          = tname;
                e["template_path"] = "MAIN:/project/exec/globals/ChartTemplateDefinitions/"
                                     + catName + "/" + tname;
                templates.push_back(std::move(e));
            }
        }

        nlohmann::json out;
        out["ok"]        = true;
        out["count"]     = (int)templates.size();
        out["templates"] = std::move(templates);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_chart_templates", e.what()); }
      catch (...)                     { return returnException("list_chart_templates", "unknown"); }
}

// ============================================================================
// modelerai_add_dashboard_chart(json)
//   json: { dashboard, template (a template_path from list_chart_templates)
//           | category + name, objects?: [names] }
//
// v1 — basic object-tracking charts. Flow (from the verified recipe):
//   1. resolve the dashboard (Tools/Dashboards/<name>) and OPEN it →
//      applicationcommand("dashboard", dash) returns the VIEW node
//   2. function_s(view, "createGraphWindow", templateNode, 1) → the chart
//      (a ChartTemplate instance under /Tools/ChartTemplates)
//   3. resolve its primary tracked-objects Group at chart >stats/primaryGroup
//      (.first), addMember each requested object
//   4. function_s(chart, "onApply") + refreshview(view)
//
// Returns rich diagnostics (chart_path, group_path, group_resolved, view_path)
// so we can verify and iterate per chart type. See
// docs/dashboards/chart-subsystem-map.html.
// ============================================================================
modelerai_export Variant ModelerAi_addDashboardChart(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_add_dashboard_chart expects { dashboard, template | category+name, objects? } JSON.");
        }
        std::string dashboardName, templatePath, category, name;
        std::vector<std::string> objectNames;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            dashboardName = j.value("dashboard", std::string(""));
            templatePath  = j.value("template",  std::string(""));
            category      = j.value("category",  std::string(""));
            name          = j.value("name",      std::string(""));
            if (j.contains("objects")) {
                const auto& o = j["objects"];
                if (o.is_array()) { for (const auto& it : o) if (it.is_string()) objectNames.push_back(it.get<std::string>()); }
                else if (o.is_string()) objectNames.push_back(o.get<std::string>());
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (dashboardName.empty()) return returnError("missing_dashboard", "dashboard (name) is required.");
        if (templatePath.empty()) {
            if (category.empty() || name.empty())
                return returnError("missing_template",
                    "Provide template (a template_path from list_chart_templates) or category + name.");
            templatePath = "MAIN:/project/exec/globals/ChartTemplateDefinitions/" + category + "/" + name;
        }

        treenode tmpl = node(templatePath.c_str(), nullptr);
        if (!objectexists(tmpl)) {
            return returnError("template_not_found", "chart template not found: " + templatePath);
        }

        // Resolve the dashboard's live VIEW node. Dashboards stay OPEN (in the VIEW
        // tree), so /Tools/Dashboards/<name> only exists when CLOSED. Try the
        // persisted node first (open it); otherwise find the already-open dashboard
        // tab in the active view by name (a dashboard tab carries a GraphPanel).
        treenode view = 0;
        bool foundOpen = false;
        treenode persisted = model()->find(("Tools/Dashboards/" + dashboardName).c_str());
        if (objectexists(persisted)) {
            Variant vv = applicationcommand("dashboard", Variant(persisted));
            if (vv.type == VariantType::TreeNode) view = static_cast<treenode>(vv);
        } else {
            treenode active = node("VIEW:/active", nullptr);
            if (objectexists(active)) {
                long long visited = 0;
                const long long VCAP = 300000;
                for (treenode n = nextforobjecttreeunder(active, active);
                     objectexists(n) && visited < VCAP && !objectexists(view);
                     n = nextforobjecttreeunder(n, active)) {
                    ++visited;
                    if (std::string(getname(n)) == dashboardName && objectexists(n->find("GraphPanel"))) {
                        view = n; foundOpen = true;
                    }
                }
            }
        }
        if (!objectexists(view)) {
            return returnError("dashboard_not_found",
                "Dashboard '" + dashboardName + "' not found — neither persisted under "
                "Tools/Dashboards (closed) nor open in the active view. Create it with "
                "modelerai_create_dashboard.");
        }
        treenode host = view;
        (void)foundOpen;

        // State-based charts (State / StaffState / LocationState) read their data
        // from a StateTable. FlexSim neither auto-creates nor auto-binds one, so
        // the chart comes up empty (collector's StateTable label = 0x0). Resolve a
        // table now — reuse the FIRST existing one, else create a default — and
        // point the chart's collector at it after the chart is instantiated.
        treenode stateTable = 0;
        if (templatePath.find("State") != std::string::npos) {
            treenode stCont = node("Tools/StateTables", model());
            if (objectexists(stCont) && content(stCont) > 0) {
                stateTable = rank(stCont, 1);            // first available
            } else {
                stateTable = Tools::create("StateTable", "Default");
            }
        }

        // Instantiate the chart from the template.
        treenode chart = 0;
        {
            Variant cv = function_s(host, "createGraphWindow", Variant(tmpl), Variant(1.0));
            if (cv.type == VariantType::TreeNode) chart = static_cast<treenode>(cv);
        }
        if (!objectexists(chart)) {
            return returnError("create_chart_failed",
                "createGraphWindow did not return a chart node (template may need a different host/args).");
        }

        // Resolve the primary tracked-objects Group: chart >stats/primaryGroup → .first.
        treenode primaryContainer = node(">stats/primaryGroup", chart);
        treenode groupNode = objectexists(primaryContainer) ? rank(primaryContainer, 1) : (treenode)0;

        int added = 0;
        bool groupOk = false;
        nlohmann::json notFound = nlohmann::json::array();
        if (objectexists(groupNode)) {
            Group* g = groupNode->object<Group>();
            if (g) {
                groupOk = true;
                for (const auto& nm : objectNames) {
                    treenode o = model()->find(nm.c_str());
                    if (objectexists(o)) { g->addMember(o); ++added; }
                    else notFound.push_back(nm);
                }
            }
        }

        // Commit.
        try { function_s(chart, "onApply"); } catch (...) {}

        // Bind the StateTable AFTER onApply. onApply rebuilds the collector from
        // the template and resets the StateTable label to null (0x0), so wiring it
        // before onApply gets clobbered. Re-resolve the label and point the
        // coupling at the resolved table — FlexScript equivalent:
        //   node("<chart>>stats/statisticsCollector/StatisticsCollector>labels/StateTable").value = StateTable.
        // The label ships as a null coupling; without this the collector has no
        // table to read and the chart renders empty.
        bool stateTableBound = false;
        std::string stateTablePath;
        if (objectexists(stateTable)) {
            treenode stLabel = node(">stats/statisticsCollector/StatisticsCollector>labels/StateTable", chart);
            if (objectexists(stLabel)) {
                nodeadddata(stLabel, DATATYPE_COUPLING);
                nodepoint(stLabel, stateTable);
                stateTableBound = true;
                if (const char* p = nodetomodelpath_cstr(stateTable, 1)) stateTablePath = p;
            }
        }

        // Grid placement — FlexSim's default widget placement is inconsistent
        // (some stack top-left). Tile the new widget so charts don't overlap: the
        // newest widget is the last child of the dashboard's GraphPanel; place it
        // by its 0-based index into a `cols`-wide grid, stepping by its own size.
        nlohmann::json placement = nlohmann::json::object();
        treenode panel = objectexists(view) ? view->find("GraphPanel") : (treenode)0;
        if (objectexists(panel) && content(panel) > 0) {
            int n = content(panel);
            treenode widget = rank(panel, n);   // newest widget = last child
            if (objectexists(widget)) {
                double w = 250.0, h = 180.0;
                try { double rw = (double)(spatialsx(widget)->value); if (rw > 1.0) w = rw; } catch (...) {}
                try { double rh = (double)(spatialsy(widget)->value); if (rh > 1.0) h = rh; } catch (...) {}
                const int cols = 3;
                const double gap = 12.0;
                int index = n - 1;
                double x = (index % cols) * (w + gap);
                double y = (index / cols) * (h + gap);
                try { spatialx(widget)->value = Variant(x); } catch (...) {}
                try { spatialy(widget)->value = Variant(y); } catch (...) {}
                placement["index"] = index;
                placement["cols"]  = cols;
                placement["w"] = w; placement["h"] = h; placement["x"] = x; placement["y"] = y;
            }
        }

        if (objectexists(view)) { try { refreshview(view); } catch (...) {} }

        nlohmann::json out;
        out["ok"]             = true;
        out["dashboard"]      = dashboardName;
        out["template"]       = templatePath;
        out["members_added"]  = added;
        out["group_resolved"] = groupOk;
        if (!notFound.empty()) out["objects_not_found"] = std::move(notFound);
        if (const char* p = nodetomodelpath_cstr(chart, 1)) out["chart_path"] = std::string(p);
        if (objectexists(groupNode)) { if (const char* p = nodetomodelpath_cstr(groupNode, 1)) out["group_path"] = std::string(p); }
        if (objectexists(view))      { if (const char* p = nodetomodelpath_cstr(view, 1))      out["view_path"]  = std::string(p); }
        if (!placement.empty()) out["placement"] = std::move(placement);
        if (stateTableBound) { out["state_table_bound"] = true; if (!stateTablePath.empty()) out["state_table"] = stateTablePath; }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_dashboard_chart", e.what()); }
      catch (...)                     { return returnException("add_dashboard_chart", "unknown"); }
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

        // Native (.1000084 — was executestring). Group::global doesn't link, so
        // resolve the group node by name and use the engine_export instance
        // methods isMember/addMember/length (proven via list_groups).
        TreeNode* gNode = resolveGroupNode(a.group);
        if (!gNode) {
            return returnError("group_not_found",
                "Group '" + a.group + "' does not exist. Use modelerai_create_group first.");
        }
        Group* gref = gNode->object<Group>();
        if (!gref) return returnError("add_member_failed", "'" + a.group + "' is not a Group.");

        int added = 0, skipped = 0;
        nlohmann::json nf = nlohmann::json::array();
        for (const auto& m : a.members) {
            TreeNode* obj = resolveMemberToNode(m);
            if (!objectexists(obj))            nf.push_back(m);
            else if (gref->isMember(obj, 0))   ++skipped;
            else { gref->addMember(obj);       ++added; }
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["group"]       = a.group;
        out["added"]       = added;
        out["skipped"]     = skipped;
        out["not_found"]   = std::move(nf);
        out["total_after"] = gref->length();
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

        // Native (.1000084 — was executestring). Same pattern as add_member.
        TreeNode* gNode = resolveGroupNode(a.group);
        if (!gNode) {
            return returnError("group_not_found", "Group '" + a.group + "' does not exist.");
        }
        Group* gref = gNode->object<Group>();
        if (!gref) return returnError("remove_member_failed", "'" + a.group + "' is not a Group.");

        int removed = 0, notMember = 0;
        nlohmann::json nf = nlohmann::json::array();
        for (const auto& m : a.members) {
            TreeNode* obj = resolveMemberToNode(m);
            if (!objectexists(obj))             nf.push_back(m);
            else if (!gref->isMember(obj, 0))   ++notMember;
            else { gref->removeMember(obj);     ++removed; }
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["group"]       = a.group;
        out["removed"]     = removed;
        out["not_member"]  = notMember;
        out["not_found"]   = std::move(nf);
        out["total_after"] = gref->length();
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("group_remove_member", e.what()); }
      catch (...)                     { return returnException("group_remove_member", "unknown"); }
}

// modelerai_list_groups({}) — every Group under Tools, with member counts.
modelerai_export Variant ModelerAi_listGroups(FLEXSIMINTERFACE)
{
    try {
        // Groups live under model().find("Tools/Groups"). The folder may not
        // exist yet on a brand-new model — handle null cleanly. Native walk:
        // each subnode is a Group; member count via Group::length() (an
        // engine_export instance method — links). Group::global() does NOT
        // link, so we get the Group* from the node via object<Group>().
        nlohmann::json groups = nlohmann::json::array();
        if (TreeNode* root = model()->find("Tools/Groups")) {
            int n = content(root);
            for (int i = 1; i <= n; ++i) {
                TreeNode* gNode = rank(root, i);
                if (!gNode) continue;
                Group* g = gNode->object<Group>();
                nlohmann::json e;
                e["name"]         = std::string(getname(gNode));
                e["member_count"] = g ? g->length() : 0;
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

        // Native (.1000100 — was executestring). Group::toFlatArray()/length() are
        // engine_export; Group::operator[] is NOT exported, so direct members (for
        // nested-group counting + the non-recursive listing) come from the bound
        // `members` coupling list. Each entry is one side of a two-way coupling;
        // ownerobject() of the dereferenced value resolves to the member object
        // whether the value is the member's holder or its far-side coupling node.
        TreeNode* gNode = resolveGroupNode(groupName);
        if (!gNode) {
            return returnError("group_not_found", "Group '" + groupName + "' does not exist.");
        }
        Group* gref = gNode->object<Group>();
        int directCount = gref ? gref->length() : 0;

        nlohmann::json members = nlohmann::json::array();
        int nestedCount = 0;

        TreeNode* memNode = getvarnode(gNode, "members");
        if (memNode) {
            int nd = content(memNode);
            for (int i = 1; i <= nd; ++i) {
                TreeNode* c = rank(memNode, i);
                if (!c) continue;
                // Guard per entry: reading c->value / ownerobject() can throw a
                // non-std FlexSim exception on some coupling node types — skip the
                // offending entry instead of aborting the whole listing. Detect a
                // nested group via isclasstype (reliable) rather than the class
                // node's name string.
                try {
                    Variant fv = c->value;                   // coupling -> partner
                    if (fv.type != VariantType::TreeNode) continue;
                    TreeNode* mem = ownerobject(static_cast<TreeNode*>(fv));
                    if (!objectexists(mem)) continue;
                    if (isclasstype(mem, "Group")) ++nestedCount;
                    if (!recursive) members.push_back(std::string(getname(mem)));
                } catch (...) { /* skip this coupling entry */ }
            }
        }

        if (recursive && gref) {
            Array flat = gref->toFlatArray();
            for (int i = 1; i <= flat.length; ++i) {
                Variant mv = flat[i];
                if (mv.type != VariantType::TreeNode) continue;
                TreeNode* mem = static_cast<TreeNode*>(mv);
                if (objectexists(mem)) members.push_back(std::string(getname(mem)));
            }
        }

        nlohmann::json out;
        out["ok"]                 = true;
        out["group"]              = groupName;
        out["recursive"]          = recursive;
        out["member_count"]       = (int)members.size();
        out["direct_count"]       = directCount;
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
//   modelerai_set_global_table_cell     — set one cell to any of FlexSim's 7 cell kinds
//                                          (Number, String, Pointer, Array, FlexScript, Bundle,
//                                          TrackedVariable) plus null. Mirrors modelerai_set_label.
//   modelerai_get_global_table_cell     — read one cell with value_kind
//                                          (number/string/pointer/array/null/flexscript/bundle/
//                                          tracked_variable/unknown) and table_storage (tree/bundle).
//   modelerai_list_global_tables        — every GlobalTable + dimensions
// ============================================================================

// Helpers: convert between JSON cell values and FlexScript. Tree-backed
// table cells accept every FlexSim Variant type — Number, String, TreeNode,
// Array, Null. Boolean is not a Variant type; JSON true/false is a
// convenience coerced to 1/0 Numbers on the way in. Bundle-backed tables
// are typed by column and the engine will throw if a value doesn't match
// the column type — we let that exception surface.
namespace {

// JSON shape -> FlexScript output for a cell write:
//   42 / 3.14            -> Number     expr=literal,                kind=number
//   true / false         -> Number     expr="1"/"0",                kind=number
//   "hello"              -> String     expr="\"hello\"",            kind=string
//   null                 -> Null       expr="Variant()",            kind=null
//   {node_path: "..."}   -> TreeNode   expr=node(...),              kind=pointer
//   {object: "..."}      -> TreeNode   expr=Model.find(...),        kind=pointer
//   [v1, v2, ...]        -> Array      expr=temp var name,          kind=array
//                                       (each element same shape;
//                                        nested arrays not supported)
//
// `outSetup` collects zero or more FlexScript statements (each ending in
// "\n") that must execute in the same script scope before `outExpr` is
// evaluated. `tmpTag` makes temp variable names unique across multiple
// calls in one script; pass any caller-unique string.
bool cellJsonToFlexScript(const nlohmann::json& v,
                          const std::string& tmpTag,
                          std::string& outSetup,
                          std::string& outExpr,
                          std::string& outKind,
                          std::string& outErr)
{
    outSetup.clear();
    if (v.is_null())    { outExpr = "Variant()";              outKind = "null";   return true; }
    if (v.is_boolean()) { outExpr = v.get<bool>() ? "1" : "0"; outKind = "number"; return true; }
    if (v.is_number()) {
        std::ostringstream s; s << v.get<double>();
        outExpr = s.str();
        outKind = "number";
        return true;
    }
    if (v.is_string()) {
        outExpr = "\"" + fsEscape(v.get<std::string>()) + "\"";
        outKind = "string";
        return true;
    }
    if (v.is_object()) {
        std::string nodePath  = v.value("node_path", std::string(""));
        std::string objectRef = v.value("object",    std::string(""));
        if (!nodePath.empty()) {
            outExpr = "node(\"" + fsEscape(nodePath) + "\", nullptr)";
            outKind = "pointer";
            return true;
        }
        if (!objectRef.empty()) {
            outExpr = "Model.find(\"" + fsEscape(objectRef) + "\")";
            outKind = "pointer";
            return true;
        }
        outErr = "object value must contain `node_path` or `object` (TreeNode reference)";
        return false;
    }
    if (v.is_array()) {
        std::string tmpName = "__cellArr_" + tmpTag;
        outSetup += "Array " + tmpName + " = Array();\n";
        for (size_t i = 0; i < v.size(); ++i) {
            const auto& e = v[i];
            if (e.is_array()) {
                outErr = "element " + std::to_string(i + 1) +
                         ": nested arrays are not supported in table cells";
                return false;
            }
            std::string elemSetup, elemExpr, elemKind, elemErr;
            if (!cellJsonToFlexScript(e, tmpTag + "_" + std::to_string(i),
                                      elemSetup, elemExpr, elemKind, elemErr)) {
                outErr = "element " + std::to_string(i + 1) + ": " + elemErr;
                return false;
            }
            outSetup += elemSetup;
            outSetup += tmpName + ".push(" + elemExpr + ");\n";
        }
        outExpr = tmpName;
        outKind = "array";
        return true;
    }
    outErr = "unsupported JSON type";
    return false;
}

// Variant -> JSON for a cell read. value_kind is one of:
// "number", "string", "pointer", "array", "null", "unknown".
nlohmann::json cellVariantToJson(const Variant& v, std::string& outKind)
{
    switch (v.type) {
        case VariantType::Number: {
            outKind = "number";
            double n = static_cast<double>(v);
            if (n == static_cast<double>(static_cast<long long>(n))
                && n >= -9.2e18 && n <= 9.2e18)
                return nlohmann::json(static_cast<long long>(n));
            return nlohmann::json(n);
        }
        case VariantType::String:
            outKind = "string";
            return nlohmann::json(std::string(v));
        case VariantType::TreeNode: {
            outKind = "pointer";
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
            outKind = "array";
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
        case VariantType::Null:
            outKind = "null";
            return nullptr;
        default:
            outKind = "unknown";
            return nullptr;
    }
}

// JSON cell value -> native Variant for the Variant cell kinds (number, bool,
// string, null, {node_path|object} pointer, array of those). Returns false +
// sets err on an unsupported shape. (.1000084 native replacement for the
// Variant branch of cellJsonToFlexScript — writes go through tbl[r][c]=Variant.)
bool cellJsonToVariant(const nlohmann::json& v, Variant& out, std::string& err)
{
    if (v.is_null())    { out = Variant(); return true; }
    if (v.is_boolean()) { out = Variant(v.get<bool>() ? 1.0 : 0.0); return true; }
    if (v.is_number())  { out = Variant(v.get<double>()); return true; }
    if (v.is_string())  { out = Variant(v.get<std::string>().c_str()); return true; }
    if (v.is_object()) {
        std::string nodePath  = v.value("node_path", std::string(""));
        std::string objectRef = v.value("object",    std::string(""));
        TreeNode* ref = nullptr;
        if      (!nodePath.empty())  ref = node(nodePath.c_str(), nullptr);
        else if (!objectRef.empty()) ref = model()->find(objectRef.c_str());
        else { err = "object cell must have node_path or object"; return false; }
        if (!objectexists(ref)) { err = "referenced node not found"; return false; }
        out = Variant(ref);
        return true;
    }
    if (v.is_array()) {
        Array a;
        for (const auto& e : v) {
            if (e.is_array()) { err = "nested arrays are not supported in a cell"; return false; }
            Variant ev; std::string e2;
            if (!cellJsonToVariant(e, ev, e2)) { err = "array element: " + e2; return false; }
            a.push(ev);
        }
        out = Variant(a);
        return true;
    }
    err = "unsupported cell value type";
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

        // Native (.1000084 — was executestring). reftable checks existence (no
        // Tools::get); Tools::create("GlobalTable") makes a new one; Table handles
        // size/headers; cells write via tbl[r][c]=Variant (cellJsonToVariant).
        treenode existing = reftable(tableName.c_str());
        bool wasExisting  = objectexists(existing);
        treenode tnode    = existing;
        if (!wasExisting) {
            tnode = Tools::create("GlobalTable");
            if (!objectexists(tnode)) {
                return returnError("create_table_failed", "Tools::create(\"GlobalTable\") returned null.");
            }
            setname(tnode, tableName.c_str());
        }
        Table tbl(tnode);
        if (rows > 0 && cols > 0) tbl.setSize(rows, cols, 0, 0);
        for (size_t r = 0; r < rowHeaders.size(); ++r) tbl.setRowHeader((int)r + 1, rowHeaders[r].c_str());
        for (size_t c = 0; c < colHeaders.size(); ++c) tbl.setColHeader((int)c + 1, colHeaders[c].c_str());

        // Initial cell values (cells[r][c] are 0-indexed in JSON → +1 for the
        // 1-indexed table subscript).
        int cellsWritten = 0, cellsSkipped = 0;
        if (cells.is_array()) {
            for (size_t r = 0; r < cells.size(); ++r) {
                const auto& row = cells[r];
                if (!row.is_array()) continue;
                for (size_t c = 0; c < row.size(); ++c) {
                    Variant cv; std::string err;
                    if (!cellJsonToVariant(row[c], cv, err)) { ++cellsSkipped; continue; }
                    tbl[(int)r + 1][(int)c + 1] = cv;
                    ++cellsWritten;
                }
            }
        }

        nlohmann::json out;
        out["ok"]            = true;
        out["name"]          = tableName;
        out["existing"]      = wasExisting;
        out["cells_written"] = cellsWritten;
        out["cells_skipped"] = cellsSkipped;
        if (const char* p = nodetomodelpath_cstr(tnode, 1)) out["path"] = std::string(p);
        out["num_rows"] = tbl.numRows;
        out["num_cols"] = tbl.numCols;
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
            // Accept either `table` (matches set/get_global_table_cell) or
            // `name` (.1000058 — was `name`-only, inconsistent with the rest
            // of the global-table family).
            tableName = j.value("table", std::string(""));
            if (tableName.empty()) tableName = j.value("name", std::string(""));
            if (j.contains("rows") && j["rows"].is_number_integer()) rows = j["rows"].get<int>();
            if (j.contains("cols") && j["cols"].is_number_integer()) cols = j["cols"].get<int>();
            overwrite = j.value("overwrite", false);
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (tableName.empty()) return returnError("missing_name", "table (or name) is required.");
        if (rows < 0 || cols < 0) return returnError("bad_dims", "rows and cols must be >= 0.");

        // Native: reftable() resolves the GlobalTable by name (null if absent);
        // settablesize() resizes; gettablerows/cols read back. (Was Tools.get +
        // Table(name).setSize through executestring.)
        treenode t = reftable(tableName.c_str());
        if (!objectexists(t)) {
            return returnError("table_not_found",
                "GlobalTable '" + tableName + "' does not exist. Use modelerai_create_global_table first.");
        }
        settablesize(tableName.c_str(), rows, cols, 0, overwrite ? 1 : 0);
        nlohmann::json out;
        out["ok"]       = true;
        out["name"]     = tableName;
        out["num_rows"] = gettablerows(tableName.c_str());
        out["num_cols"] = gettablecols(tableName.c_str());
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("resize_global_table", e.what()); }
      catch (...)                     { return returnException("resize_global_table", "unknown"); }
}

// modelerai_set_global_table_cell({ table, row, col, value })
//
// Value shape -> cell kind (mirrors modelerai_set_label):
//   42 / 3.14                          -> Number
//   true / false                       -> Number (1/0)
//   "hi"                               -> String
//   null                               -> Null   (clears cell)
//   { node_path | object }             -> Pointer (TreeNode)
//   [v1, v2, ...]                      -> Array  (elements: Number/String/Pointer/Null;
//                                        no nested arrays)
//   { flexscript: "body" }             -> FlexScript (bare expression gets the
//                                        standard `Object current = ownerobject(c);
//                                        Object item = param(1); return ...` wrapper)
//   { bundle: { fields: [...], rows?: [...] } } -> Bundle (column-typed)
//   { tracked_variable: { type?, start_value?, flags? } } -> TrackedVariable
//
// Variant kinds set via `Table(name)[r][c] = expr;` (works for tree- AND
// bundle-backed tables, subject to bundle column-type constraints). The
// three structural kinds mutate the underlying cell tree node via
// `Table(name).cell(r, c)` — that call throws on bundle-backed tables
// (a FlexSim engine limitation), so structural kinds are tree-table only.
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

        // Classify the JSON shape. Structural kinds need the cell tree node;
        // Variant kinds are direct subscript assignment.
        enum class Kind { Variant, FlexScript, Bundle, TrackedVariable };
        Kind kindEnum = Kind::Variant;
        if (valueJson.is_object()) {
            if      (valueJson.contains("flexscript"))       kindEnum = Kind::FlexScript;
            else if (valueJson.contains("bundle"))           kindEnum = Kind::Bundle;
            else if (valueJson.contains("tracked_variable")) kindEnum = Kind::TrackedVariable;
        }

        // Native (.1000084 — was executestring). Resolve via reftable(); Variant
        // kinds write through tbl[r][c]=Variant (works on tree- AND bundle-backed
        // tables); structural kinds mutate the cell tree node (tree-backed only —
        // tbl.cell() throws on bundle-backed tables, surfaced via the catch).
        treenode tnode = reftable(tableName.c_str());
        if (!objectexists(tnode)) {
            return returnError("table_not_found",
                "GlobalTable '" + tableName + "' does not exist. Use modelerai_create_global_table first.");
        }
        Table tbl(tnode);
        if (row > tbl.numRows || col > tbl.numCols) {
            nlohmann::json err;
            err["error"]   = "index_out_of_range";
            err["table"]   = tableName;
            err["row"]     = row;
            err["col"]     = col;
            err["message"] = "row/col exceed table dimensions; call resize first.";
            return returnJson(err);
        }

        std::string outKind;
        int droppedNodeRefCells = 0;   // bundle node/pointer row values we can't write
        try {
            if (kindEnum == Kind::Variant) {
                Variant cv; std::string err;
                if (!cellJsonToVariant(valueJson, cv, err)) {
                    return returnError("unsupported_value_type",
                        "value must be a FlexSim Variant or kind-tagged object — "
                        "Number, String, TreeNode ({node_path|object}), Array, null, "
                        "{flexscript:\"...\"}, {bundle:{...}}, or {tracked_variable:{...}}. ("
                        + err + ")");
                }
                cellVariantToJson(cv, outKind);   // report the resolved kind
                tbl[row][col] = cv;
            } else if (kindEnum == Kind::FlexScript) {
                outKind = "flexscript";
                if (!valueJson["flexscript"].is_string()) {
                    return returnError("bad_value_shape", "{flexscript} must be a string body.");
                }
                std::string body = valueJson["flexscript"].get<std::string>();
                if (body.find("return ") == std::string::npos) {
                    body = "Object current = ownerobject(c);\n"
                           "Object item = param(1);\n"
                           "return " + body + ";";
                }
                TreeNode* cellNode = tbl.cell(row, col);
                cellNode->value = Variant(body.c_str());
                switch_flexscript(cellNode, 1);
                buildnodeflexscript(cellNode);
            } else if (kindEnum == Kind::Bundle) {
                outKind = "bundle";
                const auto& spec = valueJson["bundle"];
                if (!spec.is_object()) {
                    return returnError("bad_value_shape",
                        "{bundle} must be an object with fields + optional rows.");
                }
                if (!spec.contains("fields") || !spec["fields"].is_array()) {
                    return returnError("bad_value_shape", "Bundle requires a `fields` array.");
                }
                TreeNode* cellNode = tbl.cell(row, col);
                cellNode->dataType = DATA_BUNDLE;   // make it a bundle before fields
                clearbundle(cellNode);
                std::vector<std::string> fieldNames, fieldTypes;
                for (const auto& f : spec["fields"]) {
                    if (!f.is_object() || !f.contains("name") || !f.contains("type")) {
                        return returnError("bad_value_shape",
                            "Each bundle field needs `name` and `type`.");
                    }
                    std::string fname = f.value("name", std::string(""));
                    std::string ftype = f.value("type", std::string(""));
                    int typeMacro;
                    if      (ftype == "number" || ftype == "double") typeMacro = BUNDLE_FIELD_TYPE_DOUBLE;
                    else if (ftype == "int")                          typeMacro = BUNDLE_FIELD_TYPE_INT;
                    else if (ftype == "string")                       typeMacro = BUNDLE_FIELD_TYPE_VARCHAR;
                    else if (ftype == "node"   || ftype == "pointer") typeMacro = BUNDLE_FIELD_TYPE_NODEREF;
                    else {
                        return returnError("bad_value_shape",
                            "Bundle field type '" + ftype + "' unsupported. "
                            "Use one of: number, int, string, node.");
                    }
                    addbundlefield(cellNode, fname.c_str(), typeMacro);
                    fieldNames.push_back(fname);
                    fieldTypes.push_back(ftype);
                }
                if (spec.contains("rows") && spec["rows"].is_array()) {
                    for (const auto& rowVals : spec["rows"]) {
                        if (!rowVals.is_array()) continue;
                        int e = addbundleentry(cellNode);   // entry number (0-based)
                        for (size_t c = 0; c < rowVals.size() && c < fieldNames.size(); ++c) {
                            const nlohmann::json& cell = rowVals[c];
                            int fieldIdx = static_cast<int>(c);   // 0-based
                            if (fieldTypes[c] == "string") {
                                std::string sv = cell.is_string() ? cell.get<std::string>()
                                    : (cell.is_number() ? std::to_string(cell.get<double>())
                                                        : std::string(""));
                                setbundlevalue(cellNode, e, fieldIdx, sv.c_str());
                            } else if (fieldTypes[c] == "node" || fieldTypes[c] == "pointer") {
                                // node-ref cells aren't expressible via the
                                // setbundlevalue overloads (no treenode overload),
                                // so a supplied value can't be written — count it
                                // and warn rather than silently claiming success.
                                if (!cell.is_null()) ++droppedNodeRefCells;
                            } else {
                                double dv = cell.is_boolean() ? (cell.get<bool>() ? 1.0 : 0.0)
                                    : (cell.is_number() ? cell.get<double>() : 0.0);
                                setbundlevalue(cellNode, e, fieldIdx, dv);
                            }
                        }
                    }
                }
            } else { // TrackedVariable
                outKind = "tracked_variable";
                const auto& spec = valueJson["tracked_variable"];
                int    tvType     = STAT_TYPE_TIME_SERIES;
                double startValue = 0.0;
                int    flags      = 0;
                if (spec.is_object()) {
                    std::string t = spec.value("type", std::string(""));
                    if      (t == "" || t == "time_series")    tvType = STAT_TYPE_TIME_SERIES;
                    else if (t == "categorical")               tvType = STAT_TYPE_CATEGORICAL;
                    else if (t == "categorical_combo")         tvType = STAT_TYPE_CATEGORICAL_COMBO;
                    else if (t == "level")                     tvType = STAT_TYPE_LEVEL;
                    else if (t == "cumulative")                tvType = STAT_TYPE_CUMULATIVE;
                    else if (t == "kinetic_level")             tvType = STAT_TYPE_KINETIC_LEVEL;
                    else if (t == "pointer")                   tvType = STAT_TYPE_POINTER;
                    else {
                        return returnError("bad_value_shape",
                            "TrackedVariable type '" + t + "' unknown. Use one of: "
                            "time_series, categorical, categorical_combo, level, "
                            "cumulative, kinetic_level, pointer.");
                    }
                    if (spec.contains("start_value") && spec["start_value"].is_number()) {
                        startValue = spec["start_value"].get<double>();
                    }
                    if (spec.contains("flags") && spec["flags"].is_number_integer()) {
                        flags = spec["flags"].get<int>();
                    }
                }
                TreeNode* cellNode = tbl.cell(row, col);
                TrackedVariable::init(cellNode, tvType, startValue, flags);
            }
        } catch (const std::exception& e) {
            return returnError("set_cell_failed",
                "kind=" + outKind + ": " + e.what() +
                (kindEnum != Kind::Variant
                    ? " (structural kinds require a tree-backed table; Table.cell() throws on bundle-backed tables)"
                    : ""));
        } catch (...) { return returnError("set_cell_failed", "non-std exception."); }

        nlohmann::json out;
        out["ok"]         = true;
        out["table"]      = tableName;
        out["row"]        = row;
        out["col"]        = col;
        out["value_kind"] = outKind;
        if (droppedNodeRefCells > 0) {
            out["warning"] = std::to_string(droppedNodeRefCells) +
                " node/pointer bundle cell value(s) were NOT written — node-ref "
                "bundle fields can't be populated through this tool (the field "
                "structure was created, but supply pointer values another way).";
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_global_table_cell", e.what()); }
      catch (...)                     { return returnException("set_global_table_cell", "unknown"); }
}

// modelerai_get_global_table_cell({ table, row, col })
//
// Reports value_kind = number / string / pointer / array / null / unknown
// for Variant cells, plus flexscript / bundle / tracked_variable when the
// cell's underlying tree node carries that structural kind. Bundle-backed
// tables only expose the Variant subset (Table.cell() throws on them).
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

        // Probe: dispatch on whether the table itself is bundle-backed
        // (DATA_BUNDLE on the table tools node — Table.cell() would throw)
        // or tree-backed (probe per-cell node for kind metadata).
        // Native (.1000084 — was executestring). reftable() resolves the table;
        // a DATA_BUNDLE table exposes only the Variant value (tbl.cell() throws
        // on it); tree-backed cells are probed for flexscript / bundle / TV.
        treenode tnode = reftable(tableName.c_str());
        if (!objectexists(tnode)) {
            return returnError("table_not_found", "GlobalTable '" + tableName + "' does not exist.");
        }
        Table tbl(tnode);
        if (row > tbl.numRows || col > tbl.numCols) {
            nlohmann::json err;
            err["error"] = "index_out_of_range";
            err["table"] = tableName;
            err["row"]   = row;
            err["col"]   = col;
            return returnJson(err);
        }

        nlohmann::json out;
        out["ok"]    = true;
        out["table"] = tableName;
        out["row"]   = row;
        out["col"]   = col;

        // Bundle-backed table: structural metadata unavailable; report the
        // Variant value via the subscript (which routes around cell()).
        if (tnode->dataType == DATA_BUNDLE) {
            Variant cell = tbl[row][col];
            std::string kind;
            nlohmann::json valueJson = cellVariantToJson(cell, kind);
            out["table_storage"] = "bundle";
            out["value"]         = std::move(valueJson);
            out["value_kind"]    = kind;
            return returnJson(out);
        }

        // Tree-backed table: branch on the cell node's structural kind.
        out["table_storage"] = "tree";
        TreeNode* cellNode = tbl.cell(row, col);
        int scrFlag = switch_flexscript(cellNode, -1);   // -1 = query flag
        int dt      = cellNode->dataType;

        if (dt == DATA_BUNDLE) {
            nlohmann::json b;
            b["row_count"]   = getbundlenrentries(cellNode);
            b["field_count"] = getbundlenrfields(cellNode);
            b["note"]        = "Use FlexSim bundle commands (getbundlevalue, etc.) "
                               "via run_script to read individual cells.";
            out["value_kind"] = "bundle";
            out["value"]      = std::move(b);
        } else if (scrFlag == 1) {
            out["value_kind"] = "flexscript";
            out["value"]      = std::string(cellNode->value);
            out["note"]       = "Value is the raw FlexScript body; not evaluated.";
        } else {
            Variant cell = cellNode->value;
            if (dt == DATA_SIMPLE && cell.type == VariantType::Number) {
                // Genuine TrackedVariable — evaluates to a Number (array cells
                // also report DATA_SIMPLE, so they fall through; parity with
                // the get_label fix, 2026-06-13).
                std::string kind;
                nlohmann::json valueJson = cellVariantToJson(cell, kind);
                out["value_kind"] = "tracked_variable";
                out["value"]      = std::move(valueJson);
                out["note"]       = "Reading evaluates the TrackedVariable's current value.";
            } else {
                std::string kind;
                nlohmann::json valueJson = cellVariantToJson(cell, kind);
                out["value_kind"] = kind;
                out["value"]      = std::move(valueJson);
            }
        }
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

        // Native (.1000084 — was executestring). Model::getParameters() is the
        // native accessor; findNode resolves the parameter node by name (null =
        // not found); ->value reads the current value (parameters store values,
        // they don't evaluate code like PMs).
        Parameters params = Model::getParameters();
        treenode pNode = params.findNode(name.c_str());
        if (!objectexists(pNode)) {
            return returnError("parameter_not_found",
                "No parameter named '" + name + "'. Call modelerai_list_parameters to see what's defined.");
        }
        Variant v = pNode->value;
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

        // Native (.1000084 — was executestring). Model::getParameters() enumerates
        // tables/names (engine_export); findNode(name)->value reads each value.
        Parameters paramsAcc = Model::getParameters();
        Array tableNames = paramsAcc.tableNames;
        nlohmann::json params = nlohmann::json::array();
        for (int t = 1; t <= tableNames.length; ++t) {
            std::string tn = std::string(tableNames[t]);
            if (!tableFilter.empty() && tn != tableFilter) continue;
            Array names = paramsAcc.names(tn.c_str());
            for (int i = 1; i <= names.length; ++i) {
                std::string nm = std::string(names[i]);
                nlohmann::json p;
                p["name"]  = nm;
                p["table"] = tn;
                treenode pNode = paramsAcc.findNode(nm.c_str());
                Variant val = objectexists(pNode) ? pNode->value : Variant();
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

        // Group memberships — native: walk Tools/Groups and test membership
        // via Group::global(name)->isMember(). (Was an executestring probe
        // that died on the LabelsArray compile error below, silently dropping
        // all four fields.)
        try {
            if (TreeNode* gRoot = model()->find("Tools/Groups")) {
                nlohmann::json groups = nlohmann::json::array();
                int gcount = content(gRoot);
                for (int i = 1; i <= gcount; ++i) {
                    TreeNode* gNode = rank(gRoot, i);
                    if (!gNode) continue;
                    Group* g = gNode->object<Group>();
                    if (g && g->isMember(obj, 0)) groups.push_back(std::string(getname(gNode)));
                }
                out["groups"] = std::move(groups);
            }
        } catch (...) {}

        // Labels — native: walk the labels container node's subnodes as plain
        // treenodes. This sidesteps the LabelsArray .length/[int] compile error
        // (Object.labels has no length/index) that broke the old probe.
        try {
            TreeNode* labelsNode = labels(obj);
            nlohmann::json labelNames = nlohmann::json::array();
            if (objectexists(labelsNode)) {
                int lcount = content(labelsNode);
                for (int i = 1; i <= lcount; ++i) {
                    TreeNode* l = rank(labelsNode, i);
                    if (l) labelNames.push_back(std::string(getname(l)));
                }
            }
            out["label_count"] = static_cast<int>(labelNames.size());
            out["label_names"] = std::move(labelNames);
        } catch (...) {}

        // Subnode count — native.
        try {
            out["subnode_count"] = content(obj);
        } catch (...) {}

        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_object_info", e.what()); }
      catch (...)                     { return returnException("get_object_info", "unknown"); }
}

// modelerai_list_global_tables({}) — every GlobalTable + its dimensions.
modelerai_export Variant ModelerAi_listGlobalTables(FLEXSIMINTERFACE)
{
    try {
        // Native: walk Tools/GlobalTables subnodes; dims via the name-based
        // gettablerows/gettablecols free functions (mirror Table(nm).numRows).
        nlohmann::json tables = nlohmann::json::array();
        if (TreeNode* root = model()->find("Tools/GlobalTables")) {
            int n = content(root);
            for (int i = 1; i <= n; ++i) {
                TreeNode* t = rank(root, i);
                if (!t) continue;
                std::string nm(getname(t));
                nlohmann::json e;
                e["name"]     = nm;
                e["num_rows"] = gettablerows(nm.c_str());
                e["num_cols"] = gettablecols(nm.c_str());
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

    // Native (.1000084 — was executestring). Tools.get(toolType,name) resolves
    // under Tools/<toolType>s (Groups / GlobalTables); resolve that node by name
    // and destroyobject it (the fsvisible delete — same path the UI uses).
    (void)failCode;
    std::string folder = std::string("Tools/") + toolType + "s";
    TreeNode* t = nullptr;
    if (TreeNode* root = model()->find(folder.c_str())) {
        int n = content(root);
        for (int i = 1; i <= n; ++i) {
            TreeNode* c = rank(root, i);
            if (c && std::string(getname(c)) == name) { t = c; break; }
        }
    }
    if (!objectexists(t)) {
        nlohmann::json err;
        err["ok"]            = false;
        err["error_code"]    = notFoundCode;
        err["error_message"] = std::string(toolName) + " '" + name + "' does not exist.";
        err["name"]          = name;
        return returnJson(err);
    }
    destroyobject(t);
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
// Lists (create + configure). FlexSim global Lists live under Tools/GlobalLists.
// v1 surface: create the list, add fields (expression/label/premade), set the
// initial content pushed on reset, and read back the config. Native: Tools::create
// + function_s("addField") / createcopy for premade + pointTo couplings for
// contentsOnReset — no executestring. (Design: docs/.../2026-06-24-modelerai-list-tools.)
// ============================================================================
namespace {

// Resolve a List instance by name (under Tools/GlobalLists) or by path. Returns
// null unless the resolved node is actually a List.
treenode resolveListNode(const std::string& nameOrPath)
{
    treenode n = nullptr;
    if (treenode lists = model()->find("Tools/GlobalLists")) {
        int c = content(lists);
        for (int i = 1; i <= c; ++i) {
            treenode ch = rank(lists, i);
            if (ch && std::string(getname(ch)) == nameOrPath) { n = ch; break; }
        }
    }
    if (!n) n = node(nameOrPath.c_str(), model());   // allow a model path
    if (n && isclasstype(n, "List")) return n;
    return nullptr;
}

treenode listFieldsNode(treenode list) { return getvarnode(list, "fields"); }

// The premade-field catalog lives on the List *class*, not the instance.
treenode menuFieldsRoot()
{
    return node("MAIN:/project/library/List>behaviour/eventfunctions/menuFields", nullptr);
}

// The 4 params in scope inside an expression field's body (value/puller/entry/pushTime).
static const char* kListFieldHeader =
    "Variant value = param(1);\r\nVariant puller = param(2);\r\n"
    "treenode entry = param(3);\r\ndouble pushTime = param(4);\r\n";

// Write header+body into a field's `expression` S-node and compile. Idempotent on
// the header: a body that already starts with kListFieldHeader is not doubled.
// Returns false if the field has no expression node (i.e. it's a Label field).
bool writeFieldExpression(treenode field, const std::string& body)
{
    if (!field) return false;
    treenode expr = field->find("expression");
    if (!expr) return false;
    std::string hdr = kListFieldHeader;
    std::string b   = body;
    if (b.rfind(hdr, 0) == 0) b = b.substr(hdr.size());   // strip a leading header
    std::string full = hdr + b;
    expr->value = Variant(full.c_str());
    switch_flexscript(expr, 1);
    buildnodeflexscript(expr);
    return true;
}

void setFieldDynamic(treenode field, bool dyn)
{
    if (!field) return;
    treenode d = field->find("isDynamic");
    if (d) d->value = dyn ? 1.0 : 0.0;
}

// Add+configure one field from a spec { source, name?, expression?, dynamic?,
// category?, premade_name? }. Returns the field node, or null + fills `err`.
treenode applyListField(treenode list, const nlohmann::json& spec, std::string& err)
{
    treenode fields = listFieldsNode(list);
    if (!fields) { err = "no_fields_node"; return nullptr; }
    std::string source = spec.value("source", std::string(""));
    bool        dyn    = spec.value("dynamic", false);

    if (source == "premade") {
        std::string category = spec.value("category", std::string(""));
        std::string pname    = spec.value("premade_name", std::string(""));
        treenode root = menuFieldsRoot();
        treenode cat  = root ? root->find(category.c_str()) : nullptr;
        treenode tmpl = cat  ? cat->find(pname.c_str())     : nullptr;
        if (!tmpl) { err = "premade_not_found"; return nullptr; }
        createcopy(tmpl, fields, 1);
        treenode field = rank(fields, content(fields));   // last child = the copy
        if (spec.contains("name") && spec["name"].is_string()
            && !spec["name"].get<std::string>().empty())
            setname(field, spec["name"].get<std::string>().c_str());
        if (spec.contains("dynamic")) setFieldDynamic(field, dyn);
        return field;
    }

    std::string name = spec.value("name", std::string(""));
    if (name.empty()) { err = "field_name_required"; return nullptr; }
    const char* type = (source == "expression") ? "ExpressionField"
                     : (source == "label")      ? "LabelField" : nullptr;
    if (!type) { err = "bad_source"; return nullptr; }

    function_s(list, "addField", Variant(type));
    treenode field = rank(fields, content(fields));   // last child = the new field
    if (!field) { err = "addfield_failed"; return nullptr; }
    setname(field, name.c_str());
    if (source == "expression")
        writeFieldExpression(field, spec.value("expression", std::string("0")));
    setFieldDynamic(field, dyn);
    return field;
}

// Resolve an initial-content target: a model object, else a Group by name.
bool resolveContentTarget(const std::string& name, treenode& target)
{
    target = model()->find(name.c_str());
    if (!objectexists(target)) target = model()->find(("Tools/Groups/" + name).c_str());
    return objectexists(target);
}

// Parse param(1) as a JSON object; returns false if not a JSON object.
bool parseListArg(const Variant& arg, nlohmann::json& j)
{
    if (arg.type != VariantType::String) return false;
    try { j = nlohmann::json::parse(std::string(arg)); } catch (...) { return false; }
    return j.is_object();
}

} // namespace

modelerai_export Variant ModelerAi_createList(FLEXSIMINTERFACE)
{
    try {
        nlohmann::json j;
        if (!parseListArg(param(1), j))
            return returnError("missing_args", "create_list requires { name }.");
        std::string name = j.value("name", std::string(""));
        if (name.empty()) return returnError("missing_args", "create_list requires a name.");

        treenode existing = resolveListNode(name);
        bool wasExisting  = objectexists(existing);
        treenode list = existing;
        if (!wasExisting) {
            list = Tools::create("List");
            if (!objectexists(list))
                return returnError("create_list_failed", "Tools::create(\"List\") returned null.");
            setname(list, name.c_str());
        }

        nlohmann::json fieldErrors = nlohmann::json::array();
        if (j.contains("fields") && j["fields"].is_array()) {
            for (auto& spec : j["fields"]) {
                std::string err;
                if (!applyListField(list, spec, err)) fieldErrors.push_back(err);
            }
        }

        nlohmann::json unresolved = nlohmann::json::array();
        nlohmann::json contentNames = nlohmann::json::array();
        if (j.contains("initial_content") && j["initial_content"].is_array()) {
            treenode cor = getvarnode(list, "contentsOnReset");
            for (auto& it : j["initial_content"]) {
                if (!it.is_string()) continue;
                std::string nm = it.get<std::string>();
                treenode tgt = nullptr;
                if (cor && resolveContentTarget(nm, tgt)) {
                    std::string tn = getname(tgt);
                    assertsubnode(cor, tn.c_str(), 0)->value = Variant(tgt);   // value = object node
                    contentNames.push_back(tn);
                } else unresolved.push_back(nm);
            }
        }

        nlohmann::json out;
        out["ok"]       = true;
        out["list"]     = name;
        out["existing"] = wasExisting;
        if (const char* p = nodetomodelpath_cstr(list, 1)) out["path"] = std::string(p);
        if (!contentNames.empty()) out["initial_content"] = std::move(contentNames);
        if (!fieldErrors.empty())  out["field_errors"]    = std::move(fieldErrors);
        if (!unresolved.empty())   out["unresolved"]      = std::move(unresolved);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_list", e.what()); }
      catch (...)                     { return returnException("create_list", "unknown"); }
}

modelerai_export Variant ModelerAi_addListField(FLEXSIMINTERFACE)
{
    try {
        nlohmann::json j;
        if (!parseListArg(param(1), j))
            return returnError("missing_args", "add_list_field requires { list, source, ... }.");
        std::string listName = j.value("list", std::string(""));
        treenode list = resolveListNode(listName);
        if (!list) return returnError("not_found", "List '" + listName + "' not found.");

        // Premade: pre-check so a bad name returns the available list (not a fault).
        if (j.value("source", std::string("")) == "premade") {
            std::string category = j.value("category", std::string(""));
            std::string pname    = j.value("premade_name", std::string(""));
            treenode root = menuFieldsRoot();
            treenode cat  = root ? root->find(category.c_str()) : nullptr;
            treenode tmpl = cat  ? cat->find(pname.c_str())     : nullptr;
            if (!tmpl) {
                nlohmann::json avail = nlohmann::json::array();
                treenode listFrom = cat ? cat : root;
                if (listFrom)
                    for (int i = 1; i <= content(listFrom); ++i)
                        avail.push_back(std::string(getname(rank(listFrom, i))));
                nlohmann::json e;
                e["ok"] = false; e["error"] = "premade_not_found";
                e["message"] = cat
                    ? ("No premade field '" + pname + "' in category '" + category + "'.")
                    : ("No premade category '" + category + "'. See `available` for categories.");
                e["available"] = std::move(avail);
                return returnJson(e);
            }
        }

        std::string err;
        treenode field = applyListField(list, j, err);
        if (!field) return returnError(err.c_str(), "add_list_field: " + err);

        nlohmann::json out;
        out["ok"]      = true;
        out["list"]    = listName;
        out["field"]   = std::string(getname(field));
        out["type"]    = j.value("source", std::string(""));
        out["dynamic"] = j.value("dynamic", false);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_list_field", e.what()); }
      catch (...)                     { return returnException("add_list_field", "unknown"); }
}

modelerai_export Variant ModelerAi_listPremadeFields(FLEXSIMINTERFACE)
{
    try {
        treenode mf = menuFieldsRoot();
        if (!mf) return returnError("not_found", "List menuFields catalog not found.");
        nlohmann::json categories = nlohmann::json::object();
        for (int i = 1; i <= content(mf); ++i) {
            treenode cat = rank(mf, i);
            if (!cat) continue;
            nlohmann::json arr = nlohmann::json::array();
            for (int k = 1; k <= content(cat); ++k) {
                treenode f = rank(cat, k);
                if (f) arr.push_back(std::string(getname(f)));
            }
            categories[std::string(getname(cat))] = std::move(arr);
        }
        nlohmann::json out;
        out["ok"] = true;
        out["categories"] = std::move(categories);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_premade_fields", e.what()); }
      catch (...)                     { return returnException("list_premade_fields", "unknown"); }
}

modelerai_export Variant ModelerAi_setListInitialContent(FLEXSIMINTERFACE)
{
    try {
        nlohmann::json j;
        if (!parseListArg(param(1), j))
            return returnError("missing_args", "set_list_initial_content requires { list, objects }.");
        std::string listName = j.value("list", std::string(""));
        treenode list = resolveListNode(listName);
        if (!list) return returnError("not_found", "List '" + listName + "' not found.");
        treenode cor = getvarnode(list, "contentsOnReset");
        if (!cor) return returnError("not_found", "List has no contentsOnReset node.");

        // Validate all targets first (atomic — no half-applied content).
        std::vector<std::pair<std::string, treenode>> targets;
        nlohmann::json unresolved = nlohmann::json::array();
        if (j.contains("objects") && j["objects"].is_array()) {
            for (auto& it : j["objects"]) {
                if (!it.is_string()) continue;
                std::string nm = it.get<std::string>();
                treenode tgt = nullptr;
                if (resolveContentTarget(nm, tgt)) targets.push_back({ std::string(getname(tgt)), tgt });
                else unresolved.push_back(nm);
            }
        }
        if (!unresolved.empty()) {
            nlohmann::json e;
            e["ok"] = false; e["error"] = "object_not_found";
            e["message"] = "Some initial-content names did not resolve as an object or group.";
            e["unresolved"] = std::move(unresolved);
            return returnJson(e);
        }

        if (j.value("replace", false))
            while (content(cor) > 0) rank(cor, 1)->destroy();

        nlohmann::json contentNames = nlohmann::json::array();
        for (auto& t : targets) {
            // The list's reset reads each contentsOnReset subnode's VALUE as the
            // object to push — set value to the target node (NOT a pointTo
            // coupling). Mirrors FlexScript `newNode.value = Group("...")`.
            assertsubnode(cor, t.first.c_str(), 0)->value = Variant(t.second);
            contentNames.push_back(t.first);
        }

        nlohmann::json out;
        out["ok"]   = true;
        out["list"] = listName;
        out["initial_content"] = std::move(contentNames);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_list_initial_content", e.what()); }
      catch (...)                     { return returnException("set_list_initial_content", "unknown"); }
}

modelerai_export Variant ModelerAi_getListInfo(FLEXSIMINTERFACE)
{
    try {
        nlohmann::json j;
        if (!parseListArg(param(1), j))
            return returnError("missing_args", "get_list_info requires { list }.");
        std::string listName = j.value("list", std::string(""));
        treenode list = resolveListNode(listName);
        if (!list) return returnError("not_found", "List '" + listName + "' not found.");

        nlohmann::json farr = nlohmann::json::array();
        if (treenode fields = listFieldsNode(list)) {
            for (int i = 1; i <= content(fields); ++i) {
                treenode f = rank(fields, i);
                if (!f) continue;
                treenode expr = f->find("expression");
                treenode d    = f->find("isDynamic");
                nlohmann::json fj;
                fj["name"]    = std::string(getname(f));
                fj["type"]    = expr ? "expression" : "label";
                fj["dynamic"] = (d && (double)d->value != 0.0);
                if (expr) {
                    std::string full = std::string(expr->value.toString());
                    std::string hdr  = kListFieldHeader;
                    fj["expression"] = (full.rfind(hdr, 0) == 0) ? full.substr(hdr.size()) : full;
                }
                farr.push_back(std::move(fj));
            }
        }

        nlohmann::json carr = nlohmann::json::array();
        if (treenode cor = getvarnode(list, "contentsOnReset"))
            for (int i = 1; i <= content(cor); ++i)
                carr.push_back(std::string(getname(rank(cor, i))));

        nlohmann::json out;
        out["ok"]   = true;
        out["name"] = listName;
        if (const char* p = nodetomodelpath_cstr(list, 1)) out["path"] = std::string(p);
        out["field_count"]     = (int)farr.size();
        out["fields"]          = std::move(farr);
        out["initial_content"] = std::move(carr);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_list_info", e.what()); }
      catch (...)                     { return returnException("get_list_info", "unknown"); }
}

// Edit an existing Expression field's code (replace semantics; header auto-applied,
// idempotent). Pair with get_list_info, which returns the current header-stripped
// body. Field must exist and be an Expression field (Label fields have no code).
modelerai_export Variant ModelerAi_setListFieldExpression(FLEXSIMINTERFACE)
{
    try {
        nlohmann::json j;
        if (!parseListArg(param(1), j))
            return returnError("missing_args",
                "set_list_field_expression requires { list, field, expression }.");
        std::string listName  = j.value("list",  std::string(""));
        std::string fieldName = j.value("field", std::string(""));
        if (fieldName.empty())
            return returnError("missing_args", "set_list_field_expression requires `field`.");
        treenode list = resolveListNode(listName);
        if (!list) return returnError("not_found", "List '" + listName + "' not found.");

        treenode fields = listFieldsNode(list);
        treenode field  = fields ? fields->find(fieldName.c_str()) : nullptr;
        if (!field) {
            nlohmann::json avail = nlohmann::json::array();
            if (fields)
                for (int i = 1; i <= content(fields); ++i)
                    avail.push_back(std::string(getname(rank(fields, i))));
            nlohmann::json e;
            e["ok"] = false; e["error"] = "field_not_found";
            e["message"]   = "No field '" + fieldName + "' on list '" + listName + "'.";
            e["available"] = std::move(avail);
            return returnJson(e);
        }
        if (!field->find("expression"))
            return returnError("not_an_expression_field",
                "Field '" + fieldName + "' is a Label field (no editable code). Only "
                "Expression fields have code.");

        writeFieldExpression(field, j.value("expression", std::string("0")));
        if (j.contains("dynamic") && j["dynamic"].is_boolean())
            setFieldDynamic(field, j["dynamic"].get<bool>());

        nlohmann::json out;
        out["ok"]      = true;
        out["list"]    = listName;
        out["field"]   = fieldName;
        out["written"] = std::string(field->find("expression")->value.toString());
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_list_field_expression", e.what()); }
      catch (...)                     { return returnException("set_list_field_expression", "unknown"); }
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
// Shared native helpers for the trigger tools. enumerateEvents /
// assertEventWithCode stay as function_s calls (proven event dispatch + the
// exact scratch-tree shape the walks expect); all the logic is native C++.
namespace {

// Create a scratch node under MAIN:/project/exec/globals and enumerate obj's
// events into it (function_s). Returns the scratch — caller destroyobject()s it.
treenode enumerateEventsScratch(treenode obj, const char* scratchName)
{
    treenode globals = node("MAIN:/project/exec/globals");
    if (!objectexists(globals)) return nullptr;
    treenode tmp = assertsubnode(globals, scratchName);
    for (int p = 0; p < 4096 && content(tmp) > 0; ++p) {  // bounded purge of leftovers
        treenode f = rank(tmp, 1);
        if (!f) break;
        destroyobject(f);
    }
    function_s(obj, "enumerateEvents", Variant(tmp));
    return tmp;
}

struct TriggerMatch {
    bool        matched = false;
    std::string header;          // codeHeader of the matched event
    std::string availableNames;  // comma-joined names of all enumerated events
    int         enumCount = 0;
    treenode    storage = nullptr;  // per-instance storage (reference target), null if unset
};

// Enumerate obj's events, match by name, extract header + reference storage +
// the available-names list. Destroys the scratch before returning (storage is
// the coupling target, which lives outside the scratch and survives).
TriggerMatch matchTrigger(treenode obj, const std::string& triggerName)
{
    TriggerMatch m;
    treenode tmp = enumerateEventsScratch(obj, "__mai_trigger_tmp__");
    if (!objectexists(tmp)) return m;
    m.enumCount = content(tmp);
    for (int i = 1; i <= m.enumCount; ++i) {
        treenode entry = rank(tmp, i);
        if (!entry) continue;
        treenode nameNode = entry->find("name");
        if (!nameNode) continue;
        std::string evName = std::string(nameNode->value.toString());
        if (!m.availableNames.empty()) m.availableNames += ", ";
        m.availableNames += evName;
        if (evName == triggerName) {
            m.matched = true;
            treenode hdr = entry->find("codeHeader");
            if (hdr) m.header = std::string(hdr->value.toString());
            m.storage = couplingTarget(entry->find("reference"));
        }
    }
    destroyobject(tmp);
    return m;
}

} // namespace

// ============================================================================
// Trigger picklist surface (forward-declared above apply_pick/get_pick).
//
// Triggers differ from parameters/PMs: the target is an (object, trigger) pair,
// the picklist is NOT derivable from the event (the AI supplies its name, e.g.
// "entryexittriggerpicklist"), the code header comes from the EVENT (matchTrigger),
// and the write goes to the event's storage node via assertEventWithCode — the
// same path set_trigger uses. Reuses the shared pick parser + collectPicks.
// ============================================================================
static nlohmann::json applyTriggerPickImpl(const nlohmann::json& j)
{
    nlohmann::json e;
    std::string objectName   = j.value("object",    std::string(""));
    std::string triggerName  = j.value("trigger",   std::string(""));
    std::string picklistName = j.value("picklist",  std::string(""));
    std::string pickName     = j.value("pick_name", std::string(""));
    if (objectName.empty() || triggerName.empty() || picklistName.empty() || pickName.empty()) {
        e["ok"] = false; e["error"] = "missing_args";
        e["message"] = "trigger apply_pick requires object, trigger, picklist, pick_name.";
        return e;
    }

    treenode obj = model()->find(objectName.c_str());
    if (!objectexists(obj)) {
        e["ok"] = false; e["error"] = "not_found";
        e["message"] = "Object '" + objectName + "' did not resolve via Model.find.";
        return e;
    }

    TriggerMatch tm = matchTrigger(obj, triggerName);
    if (!tm.matched) {
        e["ok"] = false; e["error"] = "trigger_not_found";
        e["message"] = "Trigger '" + triggerName + "' is not on this object.";
        e["available"] = tm.availableNames;
        return e;
    }

    // Resolve the named picklist + find the pick (recursive, handles category nesting).
    std::string viewPath = picklistViewPath(picklistName, j.value("module", std::string("")));
    treenode pl = node(viewPath.c_str(), nullptr);
    if (!pl) {
        e["ok"] = false; e["error"] = "not_found";
        e["message"] = viewPath + " not found — check the picklist name.";
        return e;
    }
    nlohmann::json all = nlohmann::json::array();
    collectPicks(pl, "", all);
    std::string tmpl;
    bool found = false;
    nlohmann::json available = nlohmann::json::array();
    for (auto& p : all) {
        std::string nm = p.value("pick_name", std::string(""));
        available.push_back(nm);
        if (nm == pickName) { tmpl = p.value("code_template", std::string("")); found = true; }
    }
    if (!found) {
        e["ok"] = false; e["error"] = "pick_not_found";
        e["message"] = "No pick named '" + pickName + "' in " + picklistName + ". See `available`.";
        e["available"] = std::move(available);
        return e;
    }

    // Fill tags. Unknown tag => error with the valid names.
    nlohmann::json validTags = nlohmann::json::array();
    for (const auto& t : parsePickTags(tmpl)) validTags.push_back(t.name);
    nlohmann::json unknownTags = nlohmann::json::array();
    if (j.contains("tags") && j["tags"].is_object()) {
        for (auto it = j["tags"].begin(); it != j["tags"].end(); ++it) {
            if (!it.value().is_string()) continue;
            if (!fillOneTag(tmpl, it.key(), it.value().get<std::string>())) {
                unknownTags.push_back(it.key());
            }
        }
    }
    if (!unknownTags.empty()) {
        e["ok"] = false; e["error"] = "unknown_tag";
        e["message"] = "Tag name(s) not present in this pick's template.";
        e["unknown_tags"] = std::move(unknownTags);
        e["valid_tags"]   = std::move(validTags);
        return e;
    }

    // Write event header + filled template to the event storage (set_trigger path).
    Variant storageV = function_s(obj, "assertEventWithCode", Variant(triggerName.c_str()));
    treenode storage = (storageV.type == VariantType::TreeNode) ? static_cast<treenode>(storageV) : nullptr;
    if (!objectexists(storage)) {
        e["ok"] = false; e["error"] = "apply_failed";
        e["message"] = "assertEventWithCode returned no storage node for '" + triggerName + "'.";
        return e;
    }
    // Header: prefer the PICKLIST's own header — it declares the variables its
    // picks reference (e.g. AGV picks need `te`/`agv`, which the event header does
    // NOT). Fall back to the event's codeHeader when the picklist carries none.
    // For the general trigger picklists the two are equivalent (item/current/port).
    std::string pickHeader = safePickStr(pl);
    if (pickHeader.empty()) pickHeader = tm.header;
    std::string finalCode = pickHeader + tmpl;
    storage->value = Variant(finalCode.c_str());

    e["ok"]        = true;
    e["surface"]   = "trigger";
    e["object"]    = objectName;
    e["trigger"]   = triggerName;
    e["picklist"]  = picklistName;
    e["pick_name"] = pickName;
    e["written"]   = finalCode;
    return e;
}

static nlohmann::json getTriggerPickImpl(const nlohmann::json& j)
{
    nlohmann::json out;
    std::string objectName  = j.value("object",  std::string(""));
    std::string triggerName = j.value("trigger", std::string(""));
    if (objectName.empty() || triggerName.empty()) {
        out["ok"] = false; out["error"] = "missing_args";
        out["message"] = "trigger get_pick requires object and trigger.";
        return out;
    }

    treenode obj = model()->find(objectName.c_str());
    if (!objectexists(obj)) {
        out["ok"] = false; out["error"] = "not_found";
        out["message"] = "Object '" + objectName + "' did not resolve via Model.find.";
        return out;
    }

    TriggerMatch tm = matchTrigger(obj, triggerName);
    if (!tm.matched) {
        out["ok"] = false; out["error"] = "trigger_not_found";
        out["available"] = tm.availableNames;
        return out;
    }

    out["ok"]      = true;
    out["surface"] = "trigger";
    out["object"]  = objectName;
    out["trigger"] = triggerName;
    if (!objectexists(tm.storage)) {
        out["set"]          = false;
        out["current_pick"] = nullptr;
        return out;
    }
    out["set"] = true;
    std::string code = std::string(tm.storage->value.toString());
    std::string label = parsePickLabel(code);
    std::string popupName, popupParams;
    parsePickPopup(code, popupName, popupParams);
    if (label.empty()) {
        out["current_pick"] = nullptr;   // hand-written / non-pick code
    } else {
        out["current_pick"] = label;
        out["popup"]        = popupName;
        nlohmann::json tarr = nlohmann::json::array();
        for (const auto& t : parsePickTags(code)) {
            nlohmann::json tj = { {"name", t.name}, {"value", t.defVal} };
            if (!t.options.empty()) tj["options"] = t.options;
            tarr.push_back(std::move(tj));
        }
        out["tags"] = std::move(tarr);
    }
    return out;
}

// ============================================================================
// Property picklist surface. Target is an (object, property) pair where the
// property's value is a FlexScript expression (e.g. a Processor's process time).
// AUTO-RESOLVES from the QuickProperties panel: `property` may be the DISPLAY name
// (e.g. "ProcessTime") — resolvePropertyPanel gives the real variable (cycletime),
// the panel's codeheader, and the picklist union, so `picklist` is OPTIONAL. Picks
// are collected with link-following (timeitempicklist -> statisticaldistribution).
// Backward compatible: if the panel doesn't know `property`, it's treated as the
// variable name directly and `picklist` is required.
// ============================================================================
static nlohmann::json applyPropertyPickImpl(const nlohmann::json& j)
{
    nlohmann::json e;
    std::string objectName   = j.value("object",    std::string(""));
    std::string propertyName = j.value("property",  std::string(""));
    std::string picklistName = j.value("picklist",  std::string(""));   // optional
    std::string pickName     = j.value("pick_name", std::string(""));
    if (objectName.empty() || propertyName.empty() || pickName.empty()) {
        e["ok"] = false; e["error"] = "missing_args";
        e["message"] = "property apply_pick requires object, property, pick_name "
                       "(picklist is optional — auto-resolved from the property's panel).";
        return e;
    }

    treenode obj = model()->find(objectName.c_str());
    if (!objectexists(obj)) {
        e["ok"] = false; e["error"] = "not_found";
        e["message"] = "Object '" + objectName + "' did not resolve via Model.find.";
        return e;
    }

    // Auto-resolve from the QuickProperties panel (real variable + header + picklist
    // union). `property` may be a display name; if the panel doesn't know it, fall
    // back to treating `property` as the variable directly (then `picklist` required).
    PropPanelResult panel = resolvePropertyPanel(obj, propertyName);
    std::string variable = panel.found ? panel.variable    : propertyName;
    std::string header   = panel.found ? panel.codeHeader  : std::string("");

    std::vector<std::string> paths;
    if (!picklistName.empty()) {
        paths.push_back(picklistViewPath(picklistName, j.value("module", std::string(""))));
    } else if (panel.found) {
        paths = panel.picklistPaths;
    } else {
        e["ok"] = false; e["error"] = "picklist_unresolved";
        e["message"] = "Could not auto-resolve a picklist for '" + propertyName + "' on "
                       + objectName + " — pass `picklist` explicitly, or use the property's "
                       "display name (e.g. \"ProcessTime\").";
        return e;
    }

    treenode vnode = getvarnode(obj, variable.c_str());
    if (!objectexists(vnode)) {
        e["ok"] = false; e["error"] = "property_not_found";
        e["message"] = "No variable '" + variable + "' on '" + objectName + "'.";
        return e;
    }

    // Header: the panel's is authoritative; else preserve the current value's
    // header; else the standard per-item fallback (picks reference `current`).
    if (header.empty()) {
        std::string current; try { current = std::string(vnode->value.toString()); } catch (...) {}
        header = extractPropertyHeader(current);
    }
    if (header.empty()) header = "Object current = ownerobject(c);\r\nObject item = param(1);\r\n";

    // Collect picks from the picklist union, following picklist-reference links.
    nlohmann::json all = nlohmann::json::array();
    std::vector<std::string> seen;
    for (const auto& path : paths) collectPicksDeep(path, all, seen, 0);

    std::string tmpl;
    bool found = false;
    nlohmann::json available = nlohmann::json::array();
    for (auto& p : all) {
        std::string nm = p.value("pick_name", std::string(""));
        available.push_back(nm);
        if (!found && nm == pickName) { tmpl = p.value("code_template", std::string("")); found = true; }
    }
    if (!found) {
        e["ok"] = false; e["error"] = "pick_not_found";
        e["message"]   = "No pick named '" + pickName + "' in the resolved picklist(s). See `available`.";
        e["available"] = std::move(available);
        nlohmann::json pj = nlohmann::json::array();
        for (const auto& path : paths) pj.push_back(path);
        e["picklists"] = std::move(pj);
        return e;
    }

    // Fill tags. Unknown tag => error with the valid names.
    nlohmann::json validTags = nlohmann::json::array();
    for (const auto& t : parsePickTags(tmpl)) validTags.push_back(t.name);
    nlohmann::json unknownTags = nlohmann::json::array();
    if (j.contains("tags") && j["tags"].is_object()) {
        for (auto it = j["tags"].begin(); it != j["tags"].end(); ++it) {
            if (!it.value().is_string()) continue;
            if (!fillOneTag(tmpl, it.key(), it.value().get<std::string>())) {
                unknownTags.push_back(it.key());
            }
        }
    }
    if (!unknownTags.empty()) {
        e["ok"] = false; e["error"] = "unknown_tag";
        e["message"] = "Tag name(s) not present in this pick's template.";
        e["unknown_tags"] = std::move(unknownTags);
        e["valid_tags"]   = std::move(validTags);
        return e;
    }

    // Write header + filled template to the property value node + compile.
    std::string finalCode = header + tmpl;
    vnode->value = Variant(finalCode.c_str());
    switch_flexscript(vnode, 1);
    buildnodeflexscript(vnode);

    e["ok"]            = true;
    e["surface"]       = "property";
    e["object"]        = objectName;
    e["property"]      = propertyName;
    e["variable"]      = variable;
    e["auto_resolved"] = (panel.found && picklistName.empty());
    e["pick_name"]     = pickName;
    e["written"]       = finalCode;
    return e;
}

static nlohmann::json getPropertyPickImpl(const nlohmann::json& j)
{
    nlohmann::json out;
    std::string objectName   = j.value("object",   std::string(""));
    std::string propertyName = j.value("property", std::string(""));
    if (objectName.empty() || propertyName.empty()) {
        out["ok"] = false; out["error"] = "missing_args";
        out["message"] = "property get_pick requires object and property.";
        return out;
    }
    treenode obj = model()->find(objectName.c_str());
    if (!objectexists(obj)) {
        out["ok"] = false; out["error"] = "not_found";
        out["message"] = "Object '" + objectName + "' did not resolve via Model.find.";
        return out;
    }
    // Accept the DISPLAY name (auto-resolve the variable from the panel) or the
    // variable name directly.
    PropPanelResult panel = resolvePropertyPanel(obj, propertyName);
    std::string variable = panel.found ? panel.variable : propertyName;
    treenode vnode = getvarnode(obj, variable.c_str());
    if (!objectexists(vnode)) {
        out["ok"] = false; out["error"] = "property_not_found";
        out["message"] = "No variable '" + variable + "' on '" + objectName + "'.";
        return out;
    }
    out["variable"] = variable;

    out["ok"]       = true;
    out["surface"]  = "property";
    out["object"]   = objectName;
    out["property"] = propertyName;
    std::string code;
    try { code = std::string(vnode->value.toString()); } catch (...) {}
    std::string label = parsePickLabel(code);
    std::string popupName, popupParams;
    parsePickPopup(code, popupName, popupParams);
    // Distributions carry no /**Label*/ — fall back to the popup name so the
    // caller still sees what kind of pick is set.
    if (label.empty() && !popupName.empty()) label = popupName;
    if (label.empty()) {
        out["current_pick"] = nullptr;
    } else {
        out["current_pick"] = label;
        out["popup"]        = popupName;
        nlohmann::json tarr = nlohmann::json::array();
        for (const auto& t : parsePickTags(code)) {
            nlohmann::json tj = { {"name", t.name}, {"value", t.defVal} };
            if (!t.options.empty()) tj["options"] = t.options;
            tarr.push_back(std::move(tj));
        }
        out["tags"] = std::move(tarr);
    }
    return out;
}

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

        // Native: enumerate + match (function_s enumerate, C++ logic), then
        // assert the per-instance storage node and write header + body.
        TriggerMatch tm = matchTrigger(obj, triggerName);
        if (!tm.matched) {
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(tm.enumCount) + " entries. "
                "Available: " + tm.availableNames);
        }
        Variant storageV = function_s(obj, "assertEventWithCode", Variant(triggerName.c_str()));
        treenode storage = (storageV.type == VariantType::TreeNode) ? static_cast<treenode>(storageV) : nullptr;
        if (!objectexists(storage)) {
            return returnError("set_trigger_failed",
                "assertEventWithCode returned no storage node for '" + triggerName + "'.");
        }
        // codeHeader strings end with \n; just concatenate the user body.
        std::string finalCode  = tm.header + code;
        std::string headerOnly = tm.header;
        storage->value = Variant(finalCode.c_str());
        std::string storagePath;
        const char* sp = nodetomodelpath_cstr(storage, 1);
        if (sp) storagePath = sp;

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

        // Native: enumerate + match, then read the per-instance storage (the
        // reference coupling target) and strip the codeHeader prefix.
        TriggerMatch tm = matchTrigger(obj, triggerName);
        if (!tm.matched) {
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(tm.enumCount) + " entries. "
                "Available: " + tm.availableNames);
        }
        bool isSet = objectexists(tm.storage);
        std::string headerOnly = tm.header;
        std::string storagePath, storedCode, userCode;
        bool headerMatch = false;
        if (isSet) {
            const char* sp = nodetomodelpath_cstr(tm.storage, 1);
            if (sp) storagePath = sp;
            storedCode = std::string(tm.storage->value.toString());
            if (!tm.header.empty() && storedCode.rfind(tm.header, 0) == 0) {
                headerMatch = true;
                userCode = storedCode.substr(tm.header.size());
            } else {
                userCode = storedCode;
            }
        }

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
        // Native: enumerate the object's events into a scratch node, then walk
        // it directly below (no nodetopath → re-find bounce).
        treenode tmp = enumerateEventsScratch(obj, "__mai_list_triggers_tmp__");
        if (!objectexists(tmp)) {
            return returnError("list_triggers_failed",
                "Could not create/enumerate the scratch node (MAIN:/project/exec/globals missing?).");
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

        // Native: enumerate + match, then destroy the per-instance storage node
        // (the reference coupling target) — the inverse of assertEventWithCode.
        TriggerMatch tm = matchTrigger(obj, triggerName);
        if (!tm.matched) {
            return returnError("trigger_not_found",
                "Trigger '" + triggerName + "' is not enumerated on this object. "
                "enumerateEvents returned " + std::to_string(tm.enumCount) + " entries. "
                "Available: " + tm.availableNames);
        }
        bool wasSet  = objectexists(tm.storage);
        bool removed = false;
        if (wasSet) { destroyobject(tm.storage); removed = true; }

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
// modelerai_find_objects(json | "namesubstring")
//   json: { name_pattern?, class?, in_group?, parent_path?, recursive?, limit? }
//
// Read-only search of the model for 3D objects matching the criteria. Returns
// { ok, count, truncated, objects:[{ name, path, class }] }.
//
// Resolved semantics:
//   - name_pattern: case-insensitive SUBSTRING (matches the slash-command UI).
//     A bare non-JSON string arg is treated as the name substring.
//   - class: isclasstype() filter — accepts a short ("Source") OR qualified
//     ("AGV::ControlPoint") name AND honors inheritance, so class:"FixedResource"
//     matches every FR, class:"TaskExecuter" every TE, etc.
//   - in_group: only objects that are (recursive) members of that Group.
//   - parent_path: tree path to start from (default = model root).
//   - recursive: descend the whole subtree (default true) vs direct children only.
//   - limit: max matches returned (default 200); truncated=true if hit.
// "Object" filter = isclasstype(n, "FlexSimObject") — the 3D-object base — so
// Tools (tables/groups/PMs) and internal label/port nodes are excluded.
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_findObjects(FLEXSIMINTERFACE)
{
    try {
        std::string namePattern, className, inGroup, parentPath;
        bool recursive = true;
        int limit = 200;

        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty()) {
                bool parsed = false;
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object()) {
                        namePattern = j.value("name_pattern", std::string(""));
                        className   = j.value("class",        std::string(""));
                        inGroup     = j.value("in_group",     std::string(""));
                        parentPath  = j.value("parent_path",  std::string(""));
                        recursive   = j.value("recursive",    true);
                        if (j.contains("limit") && j["limit"].is_number_integer()) limit = j["limit"].get<int>();
                        parsed = true;
                    } else if (j.is_string()) {
                        namePattern = j.get<std::string>(); parsed = true;
                    }
                } catch (...) {}
                if (!parsed) namePattern = s;   // not JSON → bare name substring
            }
        }
        if (limit <= 0) limit = 200;

        // Start node (default model root).
        treenode start = model();
        if (!parentPath.empty()) {
            treenode p = node(parentPath.c_str(), model());
            if (!objectexists(p)) p = model()->find(parentPath.c_str());
            if (!objectexists(p)) return returnError("parent_not_found", "parent_path not found: " + parentPath);
            start = p;
        }

        // Optional group filter.
        Group* groupObj = nullptr;
        if (!inGroup.empty()) {
            treenode gNode = resolveGroupNode(inGroup);
            if (!gNode) return returnError("group_not_found", "in_group not found: " + inGroup);
            groupObj = gNode->object<Group>();
        }

        // When searching from the model root (no explicit parent_path), exclude the
        // Tools subtree — it holds FlexSimObjects that aren't placed model objects
        // (FlowItemBin prototypes like Man/Woman/Truck, etc.). If the caller gives
        // an explicit parent_path, respect it literally (no exclusion).
        treenode toolsNode = parentPath.empty() ? model()->find("Tools") : (treenode)0;

        // Lowercase the pattern once (manual ASCII — no <cctype> dependency).
        std::string pat = namePattern;
        for (auto& ch : pat) if (ch >= 'A' && ch <= 'Z') ch = (char)(ch + 32);

        nlohmann::json objects = nlohmann::json::array();
        int matched = 0;
        bool truncated = false;

        // Test one node; returns false to signal "stop walking" (limit reached).
        auto consider = [&](treenode n) -> bool {
            if (!objectexists(n)) return true;
            if (!isclasstype(n, "FlexSimObject")) return true;     // model objects only
            if (objectexists(toolsNode)) {                         // skip Tools-resident objects
                bool underTools = false;
                for (treenode a = n->up; objectexists(a); a = a->up) {
                    if (a == toolsNode) { underTools = true; break; }
                    if (a == start) break;
                }
                if (underTools) return true;
            }
            if (!pat.empty()) {
                std::string low = std::string(getname(n));
                for (auto& ch : low) if (ch >= 'A' && ch <= 'Z') ch = (char)(ch + 32);
                if (low.find(pat) == std::string::npos) return true;
            }
            if (!className.empty() && !isclasstype(n, className.c_str())) return true;
            if (groupObj && !groupObj->isMember(n, 1)) return true;

            nlohmann::json o;
            o["name"] = std::string(getname(n));
            if (const char* p = nodetomodelpath_cstr(n, 1)) o["path"] = std::string(p);
            treenode cls = classobject(n);
            if (objectexists(cls)) o["class"] = std::string(getname(cls));
            objects.push_back(std::move(o));
            if (++matched >= limit) { truncated = true; return false; }
            return true;
        };

        if (recursive) {
            long long visited = 0;
            const long long VISIT_CAP = 500000;   // safety net on pathological trees
            for (treenode n = nextforobjecttreeunder(start, start);
                 objectexists(n) && visited < VISIT_CAP;
                 n = nextforobjecttreeunder(n, start)) {
                ++visited;
                if (!consider(n)) break;
            }
        } else {
            int nc = content(start);
            for (int i = 1; i <= nc; ++i) {
                if (!consider(rank(start, i))) break;
            }
        }

        nlohmann::json out;
        out["ok"]        = true;
        out["count"]     = matched;
        out["truncated"] = truncated;
        out["objects"]   = std::move(objects);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("find_objects", e.what()); }
      catch (...)                     { return returnException("find_objects", "unknown"); }
}

// ----------------------------------------------------------------------------
// modelerai_validate_model(json | ["check", ...])
//   json: { checks?: ["source_no_output","sink_no_input","no_input","no_output"] }
//
// Read-only FLOW-CONNECTIVITY sanity checks across the model — the common
// "forgot to wire it" mistakes, computed reliably from native port counts
// (nrip/nrop). Returns { ok, checked_objects, issue_count, errors, warnings,
// truncated, issues:[{check, severity, object, path, message}] }.
//
// v1 checks (all run by default; pass `checks` to subset):
//   source_no_output : a Source with no output connection            (error)
//   sink_no_input    : a Sink with no input connection               (warn)
//   no_input         : a non-Source FixedResource with no input      (warn)
//   no_output        : a non-Sink FixedResource with no output       (warn)
// Only FixedResources are checked (flow objects); Tools-resident objects
// (FlowItemBin prototypes) are skipped. Property-value checks (undefined
// process times, infinite capacity, arrival schedules) are deferred — they
// need fragile picklist-property reading and risk false positives.
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_validateModel(FLEXSIMINTERFACE)
{
    try {
        std::set<std::string> only;
        Variant arg = param(1);
        if (arg.type == VariantType::String) {
            std::string s(arg);
            if (!s.empty()) {
                try {
                    auto j = nlohmann::json::parse(s);
                    if (j.is_object() && j.contains("checks") && j["checks"].is_array()) {
                        for (auto& c : j["checks"]) if (c.is_string()) only.insert(c.get<std::string>());
                    } else if (j.is_array()) {
                        for (auto& c : j) if (c.is_string()) only.insert(c.get<std::string>());
                    }
                } catch (...) {}
            }
        }
        auto wants = [&](const char* name) { return only.empty() || only.count(name) > 0; };

        const int CAP = 300;
        nlohmann::json issues = nlohmann::json::array();
        int checkedObjects = 0;
        bool truncated = false;
        treenode toolsNode = model()->find("Tools");

        auto addIssue = [&](const char* check, const char* severity, treenode obj,
                            const std::string& msg) -> bool {
            nlohmann::json it;
            it["check"]    = check;
            it["severity"] = severity;
            it["object"]   = std::string(getname(obj));
            if (const char* p = nodetomodelpath_cstr(obj, 1)) it["path"] = std::string(p);
            it["message"]  = msg;
            issues.push_back(std::move(it));
            if ((int)issues.size() >= CAP) { truncated = true; return false; }
            return true;
        };

        long long visited = 0;
        const long long VISIT_CAP = 500000;
        bool stop = false;
        for (treenode n = nextforobjecttreeunder(model(), model());
             objectexists(n) && visited < VISIT_CAP && !stop;
             n = nextforobjecttreeunder(n, model())) {
            ++visited;
            if (!isclasstype(n, "FixedResource")) continue;
            // skip Tools-resident FixedResources (FlowItemBin prototypes etc.)
            if (objectexists(toolsNode)) {
                bool underTools = false;
                for (treenode a = n->up; objectexists(a); a = a->up) {
                    if (a == toolsNode) { underTools = true; break; }
                    if (a == model()) break;
                }
                if (underTools) continue;
            }

            ++checkedObjects;
            int in  = (int)nrip(n);
            int out = (int)nrop(n);

            if (isclasstype(n, "Source")) {
                if (out == 0 && wants("source_no_output"))
                    if (!addIssue("source_no_output", "error", n,
                        "Source has no output connection — created items have nowhere to go.")) stop = true;
            } else if (isclasstype(n, "Sink")) {
                if (in == 0 && wants("sink_no_input"))
                    if (!addIssue("sink_no_input", "warn", n,
                        "Sink has no input connection — nothing will arrive here.")) stop = true;
            } else {
                if (in == 0 && wants("no_input"))
                    if (!addIssue("no_input", "warn", n,
                        "FixedResource has no input connection — nothing flows in.")) stop = true;
                if (!stop && out == 0 && wants("no_output"))
                    if (!addIssue("no_output", "warn", n,
                        "FixedResource has no output connection — items can't leave (dead end).")) stop = true;
            }
        }

        int errors = 0, warns = 0;
        for (auto& it : issues) {
            std::string sev = it.value("severity", std::string(""));
            if      (sev == "error") ++errors;
            else if (sev == "warn")  ++warns;
        }

        nlohmann::json out;
        out["ok"]              = true;
        out["checked_objects"] = checkedObjects;
        out["issue_count"]     = (int)issues.size();
        out["errors"]          = errors;
        out["warnings"]        = warns;
        out["truncated"]       = truncated;
        out["issues"]          = std::move(issues);
        return returnJson(out);
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
// modelerai_create_tracked_variable(json)
//   json: { name, type?, start_value?, keep_history?, profile?,
//           ignore_warmup? (alias survive_warmup), flags?, replace? }
//
// Creates a standalone TrackedVariable tool under Tools/TrackedVariables, the
// way Josh does it by hand:
//   treenode tv = Model.find("Tools").subnodes.assert("TrackedVariables").subnodes.add();
//   TrackedVariable.init(tv, type, startValue, flags).as(treenode).name = name;
// Native here: model()->find("Tools") -> assertsubnode("TrackedVariables") ->
// nodeinsertinto() -> TrackedVariable::init() -> setname().
//
// THE point of this tool: the GUI doesn't let you set the stat FLAGS, so things
// like surviving warmup / keeping history / profiling have always been painful.
// We expose them as named booleans (no bit-math needed):
//   keep_history   -> STAT_USE_HISTORY    (0x10)
//   profile        -> STAT_USE_PROFILE    (0x20)
//   ignore_warmup  -> STAT_IGNORE_WARMUP  (0x40)  (survives the warmup reset)
// A raw `flags` int is OR'd in too for anything not covered by a named bool.
//
// type ∈ level (default), cumulative, time_series, categorical, kinetic_level,
//        pointer, categorical_combo  (STAT_TYPE_* — same set as set_label).
// ----------------------------------------------------------------------------
modelerai_export Variant ModelerAi_createTrackedVariable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_tracked_variable expects { name, type?, start_value?, "
                "keep_history?, profile?, ignore_warmup?, flags?, replace? } JSON.");
        }
        std::string name, typeStr;
        double startValue = 0.0;
        bool keepHistory = false, profile = false, ignoreWarmup = false, replace = false;
        int rawFlags = 0;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            name    = j.value("name", std::string(""));
            typeStr = j.value("type", std::string("level"));
            if (j.contains("start_value") && j["start_value"].is_number())
                startValue = j["start_value"].get<double>();
            keepHistory  = j.value("keep_history", false);
            profile      = j.value("profile", false);
            ignoreWarmup = j.value("ignore_warmup", j.value("survive_warmup", false));
            if (j.contains("flags") && j["flags"].is_number_integer())
                rawFlags = j["flags"].get<int>();
            replace = j.value("replace", false);
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (name.empty()) return returnError("missing_name", "name is required.");

        int tvType;
        if      (typeStr == "level")             tvType = STAT_TYPE_LEVEL;
        else if (typeStr == "cumulative")        tvType = STAT_TYPE_CUMULATIVE;
        else if (typeStr == "time_series")       tvType = STAT_TYPE_TIME_SERIES;
        else if (typeStr == "categorical")       tvType = STAT_TYPE_CATEGORICAL;
        else if (typeStr == "kinetic_level")     tvType = STAT_TYPE_KINETIC_LEVEL;
        else if (typeStr == "pointer")           tvType = STAT_TYPE_POINTER;
        else if (typeStr == "categorical_combo") tvType = STAT_TYPE_CATEGORICAL_COMBO;
        else return returnError("bad_type",
            "type must be one of: level, cumulative, time_series, categorical, "
            "kinetic_level, pointer, categorical_combo. Got: '" + typeStr + "'.");

        int flags = rawFlags;
        if (keepHistory)  flags |= STAT_USE_HISTORY;
        if (profile)      flags |= STAT_USE_PROFILE;
        if (ignoreWarmup) flags |= STAT_IGNORE_WARMUP;

        treenode tools = model()->find("Tools");
        if (!objectexists(tools)) return returnError("tools_missing", "model has no Tools node.");
        treenode container = assertsubnode(tools, "TrackedVariables", 0);

        // Duplicate-by-name handling (idempotent unless replace).
        treenode existing = 0;
        int nc = content(container);
        for (int i = 1; i <= nc; ++i) {
            treenode c = rank(container, i);
            if (c && std::string(getname(c)) == name) { existing = c; break; }
        }
        if (existing) {
            if (!replace) {
                nlohmann::json out;
                out["ok"]       = true;
                out["existing"] = true;
                out["name"]     = name;
                if (const char* p = nodetomodelpath_cstr(existing, 1)) out["path"] = std::string(p);
                out["note"]     = "A TrackedVariable with this name already exists. "
                                  "Pass replace:true to recreate it with new type/flags.";
                return returnJson(out);
            }
            destroyobject(existing);
        }

        // Create: add the subnode, init the TrackedVariable on it, then name it.
        treenode newTV = nodeinsertinto(container);
        if (!objectexists(newTV)) {
            return returnError("create_failed", "could not add a Tools/TrackedVariables subnode.");
        }
        TrackedVariable::init(newTV, tvType, startValue, flags);
        setname(newTV, name.c_str());

        nlohmann::json out;
        out["ok"]            = true;
        out["existing"]      = false;
        out["name"]          = name;
        out["type"]          = typeStr;
        out["start_value"]   = startValue;
        out["flags"]         = flags;
        out["flags_applied"] = {
            { "keep_history",  (flags & STAT_USE_HISTORY)   != 0 },
            { "profile",       (flags & STAT_USE_PROFILE)   != 0 },
            { "ignore_warmup", (flags & STAT_IGNORE_WARMUP) != 0 }
        };
        if (const char* p = nodetomodelpath_cstr(newTV, 1)) out["path"] = std::string(p);
        out["accessor"] = "Model.find(\"Tools/TrackedVariables/" + name + "\")";
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_tracked_variable", e.what()); }
      catch (...)                     { return returnException("create_tracked_variable", "unknown"); }
}

// ============================================================================
// modelerai_create_processflow({ kind, name?, category?, template?, attached_to?, open_view? })
//
// Creates a ProcessFlow tool of the requested kind under the model's
// Tools/Toolbox/ProcessFlow tree.
//
// kind values (required):
//   "general"  — PF_TYPE_GENERAL  — self-attaches
//   "object"   — PF_TYPE_OBJECT   — attaches to `attached_to` object if given
//   "sub_flow" — PF_TYPE_SUB_FLOW — reusable sub-flow, no implicit attachment
//   "person"   — person flow      — created via applicationcommand("addPersonFlow")
//
// Optional:
//   name        — rename via setname() after creation
//   category    — toolbox folder name (defaults by kind: General / Object / Sub Flow)
//   template    — tree path to a template node (loadprocessflowtemplate)
//   attached_to — model object name to attach to (kind "object" only)
//   open_view   — default true; pass false to suppress the viewer tab
//
// Returns: { ok, kind, name, path, attached_to? }
// ============================================================================
modelerai_export Variant ModelerAi_createProcessFlow(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_create_processflow expects a JSON object arg: "
                "{ kind, name?, category?, template?, attached_to?, open_view? }.");
        }

        std::string kind, desiredName, category, templatePath, attachedTo;
        bool openView = true;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            kind         = j.value("kind",        std::string(""));
            desiredName  = j.value("name",        std::string(""));
            category     = j.value("category",    std::string(""));
            templatePath = j.value("template",    std::string(""));
            attachedTo   = j.value("attached_to", std::string(""));
            if (j.contains("open_view")) {
                const auto& ov = j["open_view"];
                if      (ov.is_boolean()) openView = ov.get<bool>();
                else if (ov.is_number())  openView = ov.get<double>() != 0.0;
                else if (ov.is_string()) {
                    std::string s = ov.get<std::string>();
                    openView = !(s == "0" || s == "false" || s == "False" || s == "FALSE");
                }
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (kind != "general" && kind != "object" && kind != "sub_flow" && kind != "person") {
            return returnError("bad_kind",
                "kind must be one of: general, object, sub_flow, person. Got: \"" + kind + "\".");
        }

        // Default category per kind. Person flows don't use the toolbox folder
        // path — applicationcommand("addPersonFlow") handles placement itself.
        if (category.empty()) {
            if      (kind == "general")  category = "General";
            else if (kind == "object")   category = "Object";
            else if (kind == "sub_flow") category = "Sub Flow";
        }

        // Native creation. applicationcommand("addprocessflow", type) does the
        // WHOLE job — instantiate + toolbox/sidebar registration + type setup
        // (via its internal postaddprocessflow). type ints from
        // ProcessFlowDefinitions.h: GENERAL=0, OBJECT=1, SUB_FLOW=3, PERSON=4
        // (person is NOT a separate addPersonFlow command — that double-created).
        // We deliberately do NOT register a toolbox coupling ourselves
        // (addprocessflow already does — doing it twice duplicated the flow) and
        // do NOT open the view (that's a separate tool now).
        int pfType = (kind == "object") ? 1 : (kind == "sub_flow") ? 3 : (kind == "person") ? 4 : 0;
        treenode pf = nullptr;
        if (!templatePath.empty()) {
            treenode tmpl = node(templatePath.c_str());
            Variant v = applicationcommand("loadprocessflowtemplate", Variant(tmpl));
            if (v.type == VariantType::TreeNode) pf = static_cast<treenode>(v);
        } else {
            Variant v = applicationcommand("addprocessflow", Variant((double)pfType));
            if (v.type == VariantType::TreeNode) pf = static_cast<treenode>(v);
        }

        if (!objectexists(pf)) {
            return returnError("create_failed",
                "ProcessFlow creation returned no treenode. Possible causes: model "
                "limit reached, invalid template path, or ProcessFlow module not loaded.");
        }

        // Attach. A GENERAL flow must be attached to ITSELF to get its runtime
        // instance (under >stats/instances) — without it the flow has no
        // instance and the engine treats it like an object flow awaiting an
        // attachment, so it won't run. addprocessflow does NOT do this for
        // general (it only attaches when an instanceObject is passed). An
        // OBJECT flow attaches to its target. Both use the same attachObject
        // call FlexSim's own addprocessflow uses.
        if (kind == "general") {
            function_s(pf, "attachObject", Variant(pf));
        } else if (kind == "object" && !attachedTo.empty()) {
            treenode target = model()->find(attachedTo.c_str());
            if (objectexists(target)) function_s(pf, "attachObject", Variant(target));
        }

        // Optional rename.
        if (!desiredName.empty()) setname(pf, desiredName.c_str());

        // Build response. Path-tail is the canonical name (preserves ~N, which
        // .name would lose); every PF tool's lookup parses ~N from it.
        nlohmann::json out;
        out["ok"]   = true;
        out["kind"] = kind;
        std::string fullPath;
        const char* p = nodetomodelpath_cstr(pf, 1);
        if (p) fullPath = p;
        if (!fullPath.empty()) {
            out["path"] = fullPath;
            size_t lastSlash = fullPath.find_last_of('/');
            out["name"] = (lastSlash != std::string::npos) ? fullPath.substr(lastSlash + 1) : fullPath;
        } else {
            out["name"] = std::string(getname(pf));
        }
        if (kind == "object" && !attachedTo.empty()) out["attached_to"] = attachedTo;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("create_processflow", e.what()); }
      catch (...)                     { return returnException("create_processflow", "unknown"); }
}

// ============================================================================
// modelerai_open_processflow_view({ processflow })
//
// Opens the ProcessFlow view/window for an existing flow. Separate from
// create_processflow (which no longer auto-opens a view). Native: resolve the
// PF storage node by name (~N selects among duplicates), then
// applicationcommand("openprocessflowview", pf) — a direct C++ call.
// ============================================================================
modelerai_export Variant ModelerAi_openProcessFlowView(FLEXSIMINTERFACE)
{
    try {
        std::string pfName = resolveNameArg(param(1), "processflow");
        if (pfName.empty()) {
            return returnError("missing_args",
                "modelerai_open_processflow_view expects { processflow: \"Name\" } "
                "or a bare name string.");
        }

        // Resolve the PF storage node (flat list under Tools/ProcessFlow; a
        // trailing ~N selects the Nth flow of that name).
        std::string base = pfName;
        int idx = 1;
        auto tilde = pfName.rfind('~');
        if (tilde != std::string::npos) {
            try { idx = std::stoi(pfName.substr(tilde + 1)); base = pfName.substr(0, tilde); } catch (...) {}
        }
        treenode pfStorage = model()->find("Tools/ProcessFlow");
        treenode pf = nullptr;
        if (objectexists(pfStorage)) {
            int seen = 0, n = content(pfStorage);
            for (int i = 1; i <= n; ++i) {
                treenode c = rank(pfStorage, i);
                if (c && std::string(getname(c)) == base && ++seen == idx) { pf = c; break; }
            }
        }
        if (!objectexists(pf)) {
            return returnError("not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }

        Variant viewV = applicationcommand("openprocessflowview", Variant(pf));

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        const char* p = nodetomodelpath_cstr(pf, 1);
        if (p) out["path"] = std::string(p);
        out["view_opened"] = (viewV.type == VariantType::TreeNode
                              && objectexists(static_cast<treenode>(viewV)));
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("open_processflow_view", e.what()); }
      catch (...)                     { return returnException("open_processflow_view", "unknown"); }
}

// ============================================================================
// modelerai_list_processflows({})
//
// Walks Tools/Toolbox/ProcessFlow/* and emits one entry per ProcessFlow.
// Infers `kind` from the parent folder name:
//   "General"   -> "general"
//   "Object"    -> "object"
//   "Sub Flow"  -> "sub_flow"
//   "Person Flow" -> "person"
//   anything else -> "unknown"
//
// activity_count is the raw subnode count of the PF node (v1 — good enough
// for the AI to judge size without running the model).
//
// Returns: { ok, count, processflows: [{ name, kind, path, activity_count }] }
// ============================================================================
modelerai_export Variant ModelerAi_listProcessFlows(FLEXSIMINTERFACE)
{
    try {
        // Walk /Tools/ProcessFlow/* directly — the storage location for all PFs.
        // Returns kind via getvarnum detection (restored .1000056 after the
        // array-append fix proved the getvarnum path works — see the loop
        // body). Note: the .1000055 smoke only exercised a "general" PF;
        // the sub_flow/object/person branches mirror delete_processflow's
        // (which has used them in production) but the non-general branches
        // haven't been independently re-verified post-fix.
        // Native walk of /Tools/ProcessFlow/* (the storage location for all PFs).
        // ~N disambiguation for duplicate names; kind via getvarnum detection.
        treenode pfStorage = node("Tools/ProcessFlow", model());
        nlohmann::json results = nlohmann::json::array();
        if (objectexists(pfStorage)) {
            int n = content(pfStorage);
            for (int iP = 1; iP <= n; ++iP) {
                treenode pf = rank(pfStorage, iP);
                if (!objectexists(pf)) continue;
                std::string baseName = getname(pf);
                int priorCount = 0;
                for (int iJ = 1; iJ < iP; ++iJ) {
                    treenode prior = rank(pfStorage, iJ);
                    if (prior && std::string(getname(prior)) == baseName) priorCount++;
                }
                std::string displayName = (priorCount > 0)
                    ? baseName + "~" + std::to_string(priorCount + 1)
                    : baseName;
                std::string kind = "general";
                if (getvarnum(pf, "isSubFlow") == 1) kind = "sub_flow";
                else if (getvarnum(pf, "isPersonFlow") == 1) kind = "person";
                else {
                    treenode attachedObj = getvarnode(pf, "attachedObject");
                    if (objectexists(attachedObj) && attachedObj != pf) kind = "object";
                }
                nlohmann::json row;
                row["name"] = displayName;
                row["kind"] = kind;
                const char* p = nodetomodelpath_cstr(pf, 1);
                row["path"] = p ? std::string(p) : std::string("");
                // activity_count omitted — use modelerai_list_activities on a
                // specific PF to get its activity list / count.
                results.push_back(std::move(row));
            }
        }

        nlohmann::json out;
        out["ok"]           = true;
        out["count"]        = static_cast<int>(results.size());
        out["processflows"] = std::move(results);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_processflows", e.what()); }
      catch (...)                     { return returnException("list_processflows", "unknown"); }
}

// Forward declarations for PF-tool helpers (definitions live just before
// set_activity_variable and inside the activity-table namespace block).
// Needed here because delete_processflow / add_activity / connect_activities
// / delete_activity / list_activities use them and appear before the
// definition sites. The full DisambiguatedName struct is required (not just
// forward-declared) because parseDisambiguatedName returns it by value.
namespace {
struct DisambiguatedName {
    std::string base;
    int index;
};
DisambiguatedName parseDisambiguatedName(const std::string& nameIn);
std::string emitPfResolveScript(const std::string& toolName, const std::string& pfName);
std::string emitActivityResolveScript(
    const std::string& toolName,
    const std::string& actName,
    const std::string& actVarName,
    const std::string& pfName,
    const std::string& loopVar);
nlohmann::json parseErrString(const std::string& s, const char* defaultCode);
} // anonymous namespace

// ============================================================================
// modelerai_delete_processflow({ name } | "name")
//
// Finds the ProcessFlow by name under any Tools/Toolbox/ProcessFlow/* folder
// and destroys it (and its toolbox coupling entry).
//
// Safety: only destroys nodes that are actually inside the
// Tools/Toolbox/ProcessFlow subtree — won't touch arbitrary model objects.
//
// Returns: { ok, name, kind, removed }
//   ok == false  + error_code "not_found" if no PF by that name exists.
// ============================================================================
modelerai_export Variant ModelerAi_deleteProcessFlow(FLEXSIMINTERFACE)
{
    try {
        // Accept either a bare string param or { "name": "..." }.
        std::string name = resolveNameArg(param(1), "name");
        if (name.empty()) {
            return returnError("missing_name",
                "modelerai_delete_processflow requires a name: pass a string or { \"name\": \"...\" }.");
        }

        // Native: resolve the PF under /Tools/ProcessFlow (~N selects among
        // duplicate names), infer kind, remove any toolbox coupling, destroy.
        std::string base = name;
        int idx = 1;
        auto tilde = name.rfind('~');
        if (tilde != std::string::npos) {
            try { idx = std::stoi(name.substr(tilde + 1)); base = name.substr(0, tilde); } catch (...) {}
        }
        treenode pfStorage = node("Tools/ProcessFlow", model());
        treenode pf = nullptr;
        if (objectexists(pfStorage)) {
            int seen = 0, n = content(pfStorage);
            for (int i = 1; i <= n; ++i) {
                treenode c = rank(pfStorage, i);
                if (c && std::string(getname(c)) == base && ++seen == idx) { pf = c; break; }
            }
        }
        if (!objectexists(pf)) {
            nlohmann::json err;
            err["ok"]            = false;
            err["error_code"]    = "not_found";
            err["error_message"] = "No ProcessFlow named \"" + name + "\" found under Tools/ProcessFlow.";
            err["name"]          = name;
            return returnJson(err);
        }

        // Infer kind before destroying.
        std::string kind = "general";
        if (getvarnum(pf, "isSubFlow") == 1) kind = "sub_flow";
        else if (getvarnum(pf, "isPersonFlow") == 1) kind = "person";
        else {
            treenode attachedObj = getvarnode(pf, "attachedObject");
            if (objectexists(attachedObj) && attachedObj != pf) kind = "object";
        }

        // Best-effort: remove any toolbox coupling entry pointing to this PF.
        treenode toolbox = node("Tools/Toolbox/ProcessFlow", model());
        if (objectexists(toolbox)) {
            int nf = content(toolbox);
            for (int iF = 1; iF <= nf; ++iF) {
                treenode folder = rank(toolbox, iF);
                if (!folder) continue;
                for (int iC = content(folder); iC >= 1; --iC) {
                    treenode coupling = rank(folder, iC);
                    if (!coupling || coupling->dataType != DATATYPE_COUPLING) continue;
                    Variant cv = coupling->value;
                    if (cv.type == VariantType::TreeNode && static_cast<treenode>(cv) == pf) {
                        destroyobject(coupling);
                    }
                }
            }
        }

        destroyobject(pf);

        nlohmann::json out;
        out["ok"]      = true;
        out["name"]    = name;
        out["kind"]    = kind;
        out["removed"] = true;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("delete_processflow", e.what()); }
      catch (...)                     { return returnException("delete_processflow", "unknown"); }
}

// ============================================================================
// modelerai_add_activity({ processflow, type, library_path?, name?, after?, position? })
//
// Adds an activity of the given type to a named ProcessFlow using the
// view-based createActivity API (openprocessflowview + createActivity).
//
// IMPORTANT: ProcessFlows live at /Tools/ProcessFlow/<name> (storage location).
// Model.find("MyFlow") returns null. This function looks up via the storage
// location; do not use Model.find shortcuts or the toolbox coupling path.
//
// Required:
//   processflow  — PF name under /Tools/ProcessFlow
//   type         — activity class name in the library (e.g. "Delay", "Source")
//
// Optional:
//   library_path — library subtree path; defaults to "processflow/activities".
//                  Use "people/Activities" for People activities.
//   name         — setname() on the new activity after creation.
//   after        — name of an existing activity in the same PF; when given the
//                  new activity is auto-wired AND stacked visually below the
//                  predecessor via createActivity's 4th arg. Without `after`,
//                  the activity is placed standalone (no connector drawn).
//                  Use modelerai_connect_activities to draw explicit arrows.
//   position     — [x, y] — override the placement position (skipped in
//                  stacked mode unless explicitly provided).
//
// Returns: { ok, name, type, processflow, path, connected_to? }
//   connected_to is only present when `after` was used.
//
// Errors: processflow_not_found | library_path_not_found |
//         activity_type_not_found | previous_not_found | add_activity_failed
// ============================================================================
modelerai_export Variant ModelerAi_addActivity(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_add_activity expects a JSON object arg: "
                "{ processflow, type, library_path?, name?, after?, position? }.");
        }

        std::string pfName, actType, libraryPath, actName, afterName;
        bool hasPosition = false;
        double posX = 0.0, posY = 0.0;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName      = j.value("processflow",  std::string(""));
            actType     = j.value("type",         std::string(""));
            libraryPath = j.value("library_path", std::string("processflow/activities"));
            actName     = j.value("name",         std::string(""));
            afterName   = j.value("after",        std::string(""));
            if (j.contains("position") && j["position"].is_array() && j["position"].size() == 2) {
                posX = j["position"][0].get<double>();
                posY = j["position"][1].get<double>();
                hasPosition = true;
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actType.empty()) return returnError("missing_args", "type is required.");
        if (libraryPath.empty()) libraryPath = "processflow/activities";

        // Native: resolve the PF + library template, then create the activity via
        // the ProcessFlow::createObject class method — no view, no executestring.
        // (createActivity, the view path, ultimately wraps this same method.)

        // 1. Resolve the ProcessFlow storage node (~N selects among duplicates).
        std::string pfBase = pfName;
        int pfIdx = 1;
        {
            auto t = pfName.rfind('~');
            if (t != std::string::npos) {
                try { pfIdx = std::stoi(pfName.substr(t + 1)); pfBase = pfName.substr(0, t); } catch (...) {}
            }
        }
        treenode pfStorage = node("Tools/ProcessFlow", model());
        treenode pf = nullptr;
        if (objectexists(pfStorage)) {
            int seen = 0, n = content(pfStorage);
            for (int i = 1; i <= n; ++i) {
                treenode c = rank(pfStorage, i);
                if (c && std::string(getname(c)) == pfBase && ++seen == pfIdx) { pf = c; break; }
            }
        }
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }

        // 2. Resolve the library activity template: library()/<libraryPath>/<type>.
        treenode libPath = library()->find(libraryPath.c_str());
        if (!objectexists(libPath)) {
            return returnError("library_path_not_found",
                "library_path '" + libraryPath + "' not found. Try processflow/activities or people/Activities.");
        }
        treenode libObj = libPath->find(actType.c_str());
        if (!objectexists(libObj)) {
            return returnError("activity_type_not_found",
                "activity type '" + actType + "' not found under library_path '" + libraryPath +
                "'. Common types: InterArrivalSource, ScheduleSource, DateTimeSource, Delay, Decide, AssignLabels, Sink.");
        }

        // 3. Create natively via the ProcessFlow::createObject class method.
        ProcessFlow::ProcessFlow* pfObj = pf->object<ProcessFlow::ProcessFlow>();
        if (!pfObj) {
            return returnError("add_activity_failed", "could not obtain the ProcessFlow object from the PF node.");
        }
        treenode newActivity = nullptr;
        try {
            newActivity = pfObj->createObject(libObj, nullptr, false);
        } catch (...) {
            return returnError("add_activity_failed", "ProcessFlow::createObject threw.");
        }
        if (!objectexists(newActivity)) {
            return returnError("add_activity_failed",
                "createObject returned no activity (the library type may be invalid for this flow).");
        }

        // 4. Optional position + rename (native).
        if (hasPosition) setloc(newActivity, posX, posY, 0);
        if (!actName.empty()) setname(newActivity, actName.c_str());

        std::string newName = std::string(getname(newActivity));
        std::string newPath;
        const char* np = nodetomodelpath_cstr(newActivity, 1);
        if (np) newPath = np;

        nlohmann::json out;
        out["ok"]          = true;
        out["name"]        = newName;
        out["type"]        = actType;
        out["processflow"] = pfName;
        out["path"]        = newPath;
        // Native createObject does NOT auto-wire to a predecessor (that was a
        // view-only feature). If `after` was given, say so explicitly.
        if (!afterName.empty()) {
            out["after_note"] = "Native add_activity does not auto-connect; "
                                "use modelerai_connect_activities to wire " + afterName + " -> " + newName + ".";
        }
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("add_activity", e.what()); }
      catch (...)                     { return returnException("add_activity", "unknown"); }
}

// ============================================================================
// modelerai_connect_activities({ processflow, from, to })
//
// Draws an explicit connector arrow between two existing activities in a
// named ProcessFlow. Both activities must already exist in the PF.
//
// IMPORTANT: ProcessFlows live at /Tools/ProcessFlow/<name> (storage location).
// Model.find("MyFlow") returns null. This function looks up via the storage
// location; do not use Model.find shortcuts or the toolbox coupling path.
//
// Required:
//   processflow — PF name under /Tools/ProcessFlow
//   from        — name of the source activity in the PF
//   to          — name of the target activity in the PF
//
// Returns: { ok, processflow, from, to, connected: true }
//
// Errors: processflow_not_found | from_not_found | to_not_found | missing_args |
//         connect_activities_failed
// ============================================================================
modelerai_export Variant ModelerAi_connectActivities(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_connect_activities expects a JSON object: "
                "{ processflow, from, to }.");
        }

        std::string pfName, fromName, toName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName   = j.value("processflow", std::string(""));
            fromName = j.value("from",        std::string(""));
            toName   = j.value("to",          std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (pfName.empty())   return returnError("missing_args", "processflow is required.");
        if (fromName.empty()) return returnError("missing_args", "from is required.");
        if (toName.empty())   return returnError("missing_args", "to is required.");

        // Native: resolve PF + from/to activities (~N), then wire a connector via
        // the ProcessFlow::createConnector class method — no view, no executestring.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }
        treenode fromAct = resolveActivityNode(pf, fromName);
        if (!objectexists(fromAct)) {
            return returnError("from_not_found",
                "from activity '" + fromName + "' not found in ProcessFlow '" + pfName + "'.");
        }
        treenode toAct = resolveActivityNode(pf, toName);
        if (!objectexists(toAct)) {
            return returnError("to_not_found",
                "to activity '" + toName + "' not found in ProcessFlow '" + pfName + "'.");
        }

        ProcessFlow::ProcessFlow* pfObj = pf->object<ProcessFlow::ProcessFlow>();
        ProcessFlow::ProcessFlowObject* fromObj = fromAct->object<ProcessFlow::ProcessFlowObject>();
        ProcessFlow::ProcessFlowObject* toObj   = toAct->object<ProcessFlow::ProcessFlowObject>();
        if (!pfObj || !fromObj || !toObj) {
            return returnError("connect_activities_failed",
                "could not obtain ProcessFlow/activity C++ objects from their nodes.");
        }

        try {
            ProcessFlow::Connector* c = pfObj->createConnector();
            if (!c) {
                return returnError("connect_activities_failed", "createConnector returned null.");
            }
            c->setFromBlock(fromObj);
            c->setToBlock(toObj);
            c->onCreate();
        } catch (...) {
            return returnError("connect_activities_failed", "ProcessFlow::createConnector wiring threw.");
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["from"]        = fromName;
        out["to"]          = toName;
        out["connected"]   = true;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("connect_activities", e.what()); }
      catch (...)                     { return returnException("connect_activities", "unknown"); }
}

// ============================================================================
// modelerai_delete_activity({ activity, processflow })
//
// Finds the named activity within the named ProcessFlow and destroys it.
// Both arguments are required because activity names are only unique per-PF.
//
// Note (v1): Does not explicitly clean up connectors that referenced the
// deleted activity — FlexSim's connector destruction follows the activity
// destroy in practice; dangling cases will surface at run time.
//
// Returns: { ok, name, processflow, removed: true }
// Errors: processflow_not_found | activity_not_found | missing_args
// ============================================================================
modelerai_export Variant ModelerAi_deleteActivity(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_delete_activity expects a JSON object: { activity, processflow }.");
        }

        std::string pfName, actName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");

        // Native: resolve PF + activity (~N), destroy. No executestring.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }
        treenode activity = resolveActivityNode(pf, actName);
        if (!objectexists(activity)) {
            return returnError("activity_not_found",
                "Activity '" + actName + "' not found in ProcessFlow '" + pfName + "'.");
        }
        destroyobject(activity);

        nlohmann::json out;
        out["ok"]          = true;
        out["name"]        = actName;
        out["processflow"] = pfName;
        out["removed"]     = true;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("delete_activity", e.what()); }
      catch (...)                     { return returnException("delete_activity", "unknown"); }
}

// ============================================================================
// modelerai_set_activity_variable({ processflow, activity, variable, value })
//
// Sets a variable on an activity within a named ProcessFlow.
//
// Required:
//   processflow — PF name
//   activity    — activity name within that PF
//   variable    — variable name on the activity (as seen in its class schema)
//   value       — one of:
//     number              → setvarnum(activity, varName, n)
//     string              → getvarnode(activity, varName).value = "s"
//     { "ref": "Name" }   → look up "Name" as an activity in the same PF;
//                           getvarnode(activity, varName).value = otherActivity
//     { "ref_pf": "Name" }→ look up "Name" as a ProcessFlow (flowRef pattern);
//                           getvarnode(activity, varName).value = otherFlow
//
// Returns: { ok, activity, variable, value_kind, processflow }
//   value_kind: "number" | "string" | "ref" | "ref_pf" | "model_object" | "flexscript"
//
// Errors: processflow_not_found | activity_not_found | ref_not_found |
//         ref_pf_not_found | model_object_not_found | missing_args | bad_args_json
// ============================================================================

// PF activity codeHeader generator. Shared by set_activity_variable and
// set_activity_table_cell's {flexscript:...} handlers; prepends the right
// param-bindings header so `current`/`token`/`activity`/etc. are in scope
// when the body evaluates. The four variants match what FlexSim's engine
// passes to each variable's body — see the activity deep-dive docs.
//
// Variants:
//   "standard"               — most PF activity vars (Delay, AssignLabels,
//                              CustomCode, CreateObject.destination, etc.)
//   "pre_token"              — runs before the token exists
//                              (InterArrivalSource.interArrivalTime;
//                              ScheduleSource arrivals Time/Quantity cells)
//   "create_object_quantity" — CreateObject.quantity body has an extra
//                              `Object item` param
//   "schedule_label"         — ScheduleSource arrivals label cells (cols
//                              4+); rowNumber + repeatCount + tokenIndex +
//                              labelName extras
//   "create_object_label"    — CreateObject "Assign Labels to Created
//                              Objects" value cells; item + assignTo alias +
//                              createdrank + labelName
//
// `maybePrependCodeHeader` skips the prepend when the body already declares
// `Object current` (the signature line every header starts with) — avoids
// double-prepending when the caller already supplied a header.
namespace {
std::string activityCodeHeader(const std::string& mode)
{
    if (mode == "pre_token") {
        return "Object current = param(1);\n"
               "treenode activity = param(2);\n"
               "treenode processFlow = ownerobject(activity);\n";
    }
    if (mode == "create_object_quantity") {
        return "Object current = param(1);\n"
               "treenode activity = param(2);\n"
               "Token token = param(3);\n"
               "Object item = param(4);\n"
               "treenode processFlow = ownerobject(activity);\n";
    }
    if (mode == "schedule_label") {
        return "Object current = param(1);\n"
               "treenode activity = param(2);\n"
               "Token token = param(3);\n"
               "int rowNumber = param(4);\n"
               "int repeatCount = param(5);\n"
               "int tokenIndex = param(6);\n"
               "string labelName = param(7);\n"
               "treenode processFlow = ownerobject(activity);\n";
    }
    if (mode == "create_object_label") {
        // CreateObject's "Assign Labels to Created Objects" table — label
        // VALUE cells. The label is applied to EACH created item (when
        // quantity > 1, every item gets it). `assignTo` is a convenience
        // alias for the item being labeled; `createdrank` is the 1-based
        // index of the item within this creation batch.
        return "Object current = param(1);\n"
               "treenode activity = param(2);\n"
               "Token token = param(3);\n"
               "Object item = param(4);\n"
               "Variant assignTo = item;\n"
               "int createdrank = param(5);\n"
               "string labelName = param(6);\n"
               "treenode processFlow = ownerobject(activity);\n";
    }
    // standard / default
    return "Object current = param(1);\n"
           "treenode activity = param(2);\n"
           "Token token = param(3);\n"
           "treenode processFlow = ownerobject(activity);\n";
}

std::string maybePrependCodeHeader(const std::string& body, const std::string& mode)
{
    if (body.find("Object current") != std::string::npos) return body;
    return activityCodeHeader(mode) + body;
}

// Implementation of parseDisambiguatedName — see forward declaration above
// (placed before delete_processflow so the earlier tools can call it).
// Splits "MainFlow~2" into ("MainFlow", 2); "MainFlow" stays as ("MainFlow", 1).
DisambiguatedName parseDisambiguatedName(const std::string& nameIn)
{
    DisambiguatedName out{nameIn, 1};
    size_t tildePos = nameIn.find('~');
    if (tildePos == std::string::npos) return out;
    try {
        int parsed = std::stoi(nameIn.substr(tildePos + 1));
        if (parsed >= 1) {
            out.base  = nameIn.substr(0, tildePos);
            out.index = parsed;
        }
    } catch (...) {
        // Not a real suffix — leave the name as-is and treat as the 1st match.
    }
    return out;
}

// Emit the standard PF storage walk + name-with-disambiguation match.
// Uses `node("Tools/ProcessFlow", model())` directly. (An earlier
// assertsubnode-based form was blamed for empty results, but that was the
// same array-append misdiagnosis — both forms reach the same storage node;
// node() is just the more direct expression of the well-known path.)
//
// After this script runs, `pf` is in scope (or the script has already
// returned an ERR string and bailed).
std::string emitPfResolveScript(const std::string& toolName, const std::string& pfName)
{
    auto disambig = parseDisambiguatedName(pfName);
    std::ostringstream fs;
    fs << "print(\"[ModelerAI] " << toolName << ": resolving PF '" << fsEscape(pfName)
       << "' (base='" << fsEscape(disambig.base) << "' match#=" << disambig.index << ")\\n\");\n";
    fs << "treenode pfStorage = node(\"Tools/ProcessFlow\", model());\n";
    fs << "if (!objectexists(pfStorage)) { print(\"[ModelerAI] " << toolName
       << ": /Tools/ProcessFlow node missing — model has no PFs yet\\n\"); "
       << "return \"ERR:processflow_not_found:No ProcessFlow named \\\"" << fsEscape(pfName)
       << "\\\" found (no PF storage exists yet).\"; }\n";
    fs << "treenode pf = NULL;\n";
    fs << "int pfMatchesSoFar = 0;\n";
    fs << "for (int iP = 1; iP <= pfStorage.subnodes.length; iP++) {\n";
    fs << "    if (string(pfStorage.subnodes[iP].name) == \"" << fsEscape(disambig.base) << "\") {\n";
    fs << "        pfMatchesSoFar = pfMatchesSoFar + 1;\n";
    fs << "        if (pfMatchesSoFar == " << disambig.index << ") { pf = pfStorage.subnodes[iP]; break; }\n";
    fs << "    }\n";
    fs << "}\n";
    fs << "if (!objectexists(pf)) { print(\"[ModelerAI] " << toolName
       << ": PF '" << fsEscape(pfName) << "' not in storage\\n\"); "
       << "return \"ERR:processflow_not_found:No ProcessFlow named \\\"" << fsEscape(pfName)
       << "\\\" found.\"; }\n";
    fs << "print(\"[ModelerAI] " << toolName << ": found pf at \" + string(nodetomodelpath(pf, 1)) + \"\\n\");\n";
    return fs.str();
}

// Emit the activity-by-name walk on the resolved `pf` from above. Also
// handles `~N` disambiguation for activity names (engine auto-suffixes
// when the modeler creates a second Source1 in the same PF).
//
// Sets `<actVarName>` treenode in scope, or bails with ERR.
std::string emitActivityResolveScript(
    const std::string& toolName,
    const std::string& actName,
    const std::string& actVarName,
    const std::string& pfName,
    const std::string& loopVar)
{
    auto disambig = parseDisambiguatedName(actName);
    std::ostringstream fs;
    fs << "treenode " << actVarName << " = NULL;\n";
    fs << "int " << loopVar << "MatchesSoFar = 0;\n";
    fs << "for (int " << loopVar << " = 1; " << loopVar << " <= pf.subnodes.length; " << loopVar << "++) {\n";
    fs << "    if (string(pf.subnodes[" << loopVar << "].name) == \"" << fsEscape(disambig.base) << "\") {\n";
    fs << "        " << loopVar << "MatchesSoFar = " << loopVar << "MatchesSoFar + 1;\n";
    fs << "        if (" << loopVar << "MatchesSoFar == " << disambig.index << ") { "
       << actVarName << " = pf.subnodes[" << loopVar << "]; break; }\n";
    fs << "    }\n";
    fs << "}\n";
    fs << "if (!objectexists(" << actVarName << ")) { print(\"[ModelerAI] " << toolName
       << ": activity '" << fsEscape(actName) << "' not found in PF '" << fsEscape(pfName) << "'\\n\"); "
       << "return \"ERR:activity_not_found:Activity \\\"" << fsEscape(actName)
       << "\\\" not found in ProcessFlow \\\"" << fsEscape(pfName) << "\\\".\"; }\n";
    return fs.str();
}
} // anonymous namespace

modelerai_export Variant ModelerAi_setActivityVariable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_activity_variable expects a JSON object: "
                "{ processflow, activity, variable, value }.");
        }

        std::string pfName, actName, varName, valueKind;
        std::string strValue, refName, refPfName, modelObjName, flexBody;
        double numValue = 0.0;

        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
            if (!j.contains("value")) {
                return returnError("missing_args", "value is required.");
            }
            const auto& val = j["value"];
            if (val.is_number()) {
                numValue  = val.get<double>();
                valueKind = "number";
            } else if (val.is_string()) {
                strValue  = val.get<std::string>();
                valueKind = "string";
            } else if (val.is_object()) {
                if (val.contains("ref") && val["ref"].is_string()) {
                    refName   = val["ref"].get<std::string>();
                    valueKind = "ref";
                } else if (val.contains("ref_pf") && val["ref_pf"].is_string()) {
                    refPfName = val["ref_pf"].get<std::string>();
                    valueKind = "ref_pf";
                } else if (val.contains("model_object") && val["model_object"].is_string()) {
                    // Pointer to an existing model object (Queue1, Source3, etc.)
                    // Storage: the variable node becomes a COUPLING pointing
                    // at the resolved Model.find(name). This is the right
                    // shape when the modeler references a SPECIFIC existing
                    // object — not a string literal, not a FlexScript body.
                    modelObjName = val["model_object"].get<std::string>();
                    valueKind    = "model_object";
                } else if (val.contains("flexscript") && val["flexscript"].is_string()) {
                    // Explicit FlexScript-mode body. The node is marked as
                    // FlexScript via switch_flexscript+buildnodeflexscript
                    // so the engine compiles + evaluates on read instead of
                    // treating the body as a literal string.
                    //
                    // We auto-prepend the right codeHeader so `current`,
                    // `token`, `activity` etc. are in scope. The optional
                    // `header` field picks which one (defaults to "standard"
                    // — see activityCodeHeader for the variants).
                    flexBody  = val["flexscript"].get<std::string>();
                    valueKind = "flexscript";
                    std::string headerMode = "standard";
                    if (val.contains("header") && val["header"].is_string()) {
                        headerMode = val["header"].get<std::string>();
                    }
                    flexBody = maybePrependCodeHeader(flexBody, headerMode);
                } else {
                    return returnError("bad_value",
                        "value object must have one of: \"ref\" (sibling activity), "
                        "\"ref_pf\" (other ProcessFlow), \"model_object\" (specific "
                        "existing model object — stores as a pointer), or "
                        "\"flexscript\" (FlexScript body — marked as a flexscript "
                        "node so the engine evaluates it).");
                }
            } else {
                return returnError("bad_value",
                    "value must be a number, string, {ref:\"...\"}, "
                    "{ref_pf:\"...\"}, {model_object:\"...\"}, or "
                    "{flexscript:\"...\"}.");
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");

        // Native (.1000084 — was executestring). Resolve PF + activity, then apply
        // the typed value via fsvisible var/node primitives.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "No ProcessFlow named \"" + pfName + "\" found.");
        }
        treenode activity = resolveActivityNode(pf, actName);
        if (!objectexists(activity)) {
            return returnError("activity_not_found",
                "Activity \"" + actName + "\" not found in ProcessFlow \"" + pfName + "\".");
        }

        if (valueKind == "number") {
            // Write the number AND force literal-number mode. Dual-mode nodes
            // (e.g. Delay.delayTimeNode) ship as FlexScript nodes holding a
            // default expression; setvarnum alone leaves the FlexScript flag on,
            // so the engine keeps evaluating the expression and the literal never
            // takes effect (.1000085 fix). Set the value then clear the flag.
            treenode varNode = getvarnode(activity, varName.c_str());
            if (objectexists(varNode)) {
                varNode->value = Variant(numValue);
                switch_flexscript(varNode, 0);   // 0 = force literal/off
            } else {
                setvarnum(activity, varName.c_str(), numValue);  // fallback
            }
        } else {
            // All other kinds write through the variable node.
            treenode varNode = getvarnode(activity, varName.c_str());
            if (!objectexists(varNode)) {
                return returnError("variable_not_found",
                    "Variable \"" + varName + "\" not found on activity \"" + actName + "\".");
            }
            if (valueKind == "string") {
                varNode->value = Variant(strValue.c_str());
            } else if (valueKind == "ref") {
                treenode refActivity = resolveActivityNode(pf, refName);
                if (!objectexists(refActivity)) {
                    return returnError("activity_not_found",
                        "Referenced activity \"" + refName + "\" not found in ProcessFlow \"" + pfName + "\".");
                }
                varNode->value = Variant(refActivity);
            } else if (valueKind == "ref_pf") {
                treenode refPf = resolvePfNode(refPfName);
                if (!objectexists(refPf)) {
                    return returnError("ref_pf_not_found",
                        "ProcessFlow \"" + refPfName + "\" not found under Tools/ProcessFlow.");
                }
                varNode->value = Variant(refPf);
            } else if (valueKind == "model_object") {
                treenode targetObj = model()->find(modelObjName.c_str());
                if (!objectexists(targetObj)) {
                    return returnError("model_object_not_found",
                        "Model object \"" + modelObjName + "\" not found.");
                }
                nodeadddata(varNode, DATATYPE_COUPLING);
                nodepoint(varNode, targetObj);
            } else if (valueKind == "flexscript") {
                varNode->value = Variant(flexBody.c_str());
                switch_flexscript(varNode, 1);
                buildnodeflexscript(varNode);
            }
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["value_kind"]  = valueKind;
        out["processflow"] = pfName;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_activity_variable", e.what()); }
      catch (...)                     { return returnException("set_activity_variable", "unknown"); }
}

// ============================================================================
// modelerai_list_activities({ processflow } | "processflowName")
//
// Lists every activity inside a named ProcessFlow. Reads from
// /Tools/ProcessFlow/<name> storage — does NOT touch the toolbox coupling.
//
// Returns: { ok, processflow, count, activities: [{ name, class, path }] }
// Errors:  processflow_not_found | missing_args | bad_args_json
// ============================================================================
modelerai_export Variant ModelerAi_listActivities(FLEXSIMINTERFACE)
{
    try {
        std::string pfName = resolveNameArg(param(1), "processflow");
        if (pfName.empty()) {
            return returnError("missing_args",
                "modelerai_list_activities requires a processflow name: "
                "pass a string or { \"processflow\": \"...\" }.");
        }

        // Native: resolve the PF (~N), walk its direct subnodes. Real activities
        // have a classobject (bookkeeping subnodes like variables/connectors
        // don't); the class node's name is the activity class.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }
        nlohmann::json activities = nlohmann::json::array();
        int n = content(pf);
        for (int iA = 1; iA <= n; ++iA) {
            treenode a = rank(pf, iA);
            if (!objectexists(a)) continue;
            treenode clsNode = classobject(a);
            if (!objectexists(clsNode)) continue;
            std::string cls(getname(clsNode));
            if (cls.empty()) continue;
            nlohmann::json row;
            row["name"]  = std::string(getname(a));
            row["class"] = cls;
            const char* p = nodetomodelpath_cstr(a, 1);
            row["path"]  = p ? std::string(p) : std::string("");
            activities.push_back(std::move(row));
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["count"]       = static_cast<int>(activities.size());
        out["activities"]  = std::move(activities);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("list_activities", e.what()); }
      catch (...)                     { return returnException("list_activities", "unknown"); }
}

// ============================================================================
// modelerai_get_activity_info({ processflow, activity })
//
// Returns descriptive info about a single activity: name, class, full path.
// (A variable-name enumeration was intentionally dropped — for valid
// variable names per class, read KNOWLEDGE/topics/modelerai/
// processflow-activity-variables.md.)
//
// Returns: { ok, processflow, activity, class, path }
// Errors:  processflow_not_found | activity_not_found | missing_args
// ============================================================================
modelerai_export Variant ModelerAi_getActivityInfo(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_activity_info expects a JSON object: "
                "{ processflow, activity }.");
        }
        std::string pfName, actName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");

        // Native: resolve PF + activity (~N), read name/class/path. (The earlier
        // getactivity() linkage probe is removed now that the native PF pipeline
        // is established.) For per-class variable names see
        // KNOWLEDGE/topics/modelerai/processflow-activity-variables.md.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "ProcessFlow '" + pfName + "' not found under Tools/ProcessFlow.");
        }
        treenode activity = resolveActivityNode(pf, actName);
        if (!objectexists(activity)) {
            return returnError("activity_not_found",
                "Activity '" + actName + "' not found in ProcessFlow '" + pfName + "'.");
        }
        std::string cls;
        treenode clsNode = classobject(activity);
        if (objectexists(clsNode)) cls = std::string(getname(clsNode));
        std::string path;
        const char* ap = nodetomodelpath_cstr(activity, 1);
        if (ap) path = ap;

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = std::string(getname(activity));
        out["class"]       = cls;
        out["path"]        = path;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_activity_info", e.what()); }
      catch (...)                     { return returnException("get_activity_info", "unknown"); }
}

// ============================================================================
// modelerai_get_activity_variable({ processflow, activity, variable })
//
// Reads a single variable on an activity. Symmetric with
// modelerai_set_activity_variable. Determines the variable's kind from its
// node datatype and returns the value in the corresponding JSON shape:
//   number  → { kind: "number", value: 42.0 }
//   string  → { kind: "string", value: "expr" }
//   ref     → { kind: "ref",    value: "OtherActivity" }  (sibling activity)
//   ref_pf  → { kind: "ref_pf", value: "OtherFlow" }      (another PF)
//   unknown → { kind: "unknown", value: <best-effort string> }
//
// Returns: { ok, processflow, activity, variable, kind, value }
// Errors:  processflow_not_found | activity_not_found | variable_not_found
// ============================================================================
modelerai_export Variant ModelerAi_getActivityVariable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_activity_variable expects a JSON object: "
                "{ processflow, activity, variable }.");
        }
        std::string pfName, actName, varName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");

        // Native (.1000084 — was executestring). Resolve PF + activity + var node,
        // classify by datatype (1=number, 2=string, 3=node/coupling). For a
        // coupling, resolve the target and detect sibling-activity (ref) vs
        // sibling-ProcessFlow (ref_pf) vs some other node (path).
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found",
                "No ProcessFlow named \"" + pfName + "\" found.");
        }
        treenode activity = resolveActivityNode(pf, actName);
        if (!objectexists(activity)) {
            return returnError("activity_not_found",
                "Activity \"" + actName + "\" not found in ProcessFlow \"" + pfName + "\".");
        }
        treenode varNode = getvarnode(activity, varName.c_str());
        if (!objectexists(varNode)) {
            return returnError("variable_not_found",
                "Variable \"" + varName + "\" not found on activity \"" + actName + "\".");
        }
        treenode pfStorage = node("Tools/ProcessFlow", model());

        std::string kind, valStr;
        double valNum = 0.0;
        int dt = getdatatype(varNode);
        if (dt == 1) {
            kind   = "number";
            valNum = getvarnum(activity, varName.c_str());
        } else if (dt == 2) {
            kind   = "string";
            valStr = std::string(varNode->value);
        } else if (dt == 3) {
            Variant tv = varNode->value;
            treenode target = (tv.type == VariantType::TreeNode) ? static_cast<treenode>(tv) : nullptr;
            if (!objectexists(target)) {
                kind = "unknown";
            } else {
                treenode parent = target->up;
                if (objectexists(parent) && parent == pf) {
                    kind   = "ref";
                    valStr = std::string(getname(target));
                } else if (objectexists(parent) && parent == pfStorage) {
                    kind   = "ref_pf";
                    valStr = std::string(getname(target));
                } else {
                    kind = "node";
                    const char* p = nodetomodelpath_cstr(target, 1);
                    valStr = p ? std::string(p) : std::string("");
                }
            }
        } else {
            kind = "unknown";
            Variant cv = varNode->value;
            if (cv.type == VariantType::String) valStr = std::string(cv);
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["kind"]        = kind;
        if      (kind == "number") out["value"] = valNum;
        else if (kind == "string") out["value"] = valStr;
        else if (kind == "ref")    out["value"] = { { "ref",    valStr } };
        else if (kind == "ref_pf") out["value"] = { { "ref_pf", valStr } };
        else                       out["value"] = valStr;  // unknown / node path
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_activity_variable", e.what()); }
      catch (...)                     { return returnException("get_activity_variable", "unknown"); }
}

// ============================================================================
// Activity-table tool family
// ============================================================================
// (codeHeader helpers — `activityCodeHeader`, `maybePrependCodeHeader` —
// live alongside set_activity_variable above so both tool families share
// the same auto-prepend behaviour.)
// ============================================================================
// ScheduleSource's `arrivals`, AssignLabels' `>labels`, and similar
// table-shaped variables on PF activities all behave like a FlexScript
// `Table`. The five tools below give the agent uniform get / set / size /
// resize / column-header access:
//
//   modelerai_set_activity_table_cell
//   modelerai_get_activity_table_cell
//   modelerai_get_activity_table_size
//   modelerai_resize_activity_table
//   modelerai_set_activity_table_column_header
//
// Lookup convention:
//   - variable starts with ">" (e.g. ">labels"): hidden subnode, resolved
//     via `activity.find(variable)` (FlexSim's hidden-container access).
//   - otherwise: regular variable resolved via `getvarnode(activity, name)`.
//
// All five walk /Tools/ProcessFlow/<name> for the PF (the proven storage
// pattern from the rest of the PF tool family) and then walk pf.subnodes
// by exact-name match for the activity (proven pattern from list_activities).
// pf.find() and getclassname() are NOT used (both unreliable on this
// FlexScript build — see KB feedback memory for rationale).
// ============================================================================

namespace {

// Emit the storage walk + activity walk + table-node resolution. Pushes
// a treenode `tblNode` into scope for callers. Bails to the supplied
// `errPrefix` ERR: string on any failure (matches set_activity_variable's
// pattern for FlexScript→C++ error marshaling via a leading ERR: string).
//
// On success leaves these in scope:
//   treenode pfStorage
//   treenode pf
//   treenode activity
//   treenode tblNode      ← the Table-shaped node
//
// Callers append their own per-tool logic afterwards.
std::string emitActivityTableResolveScript(
    const std::string& errPrefix,
    const std::string& pfName,
    const std::string& actName,
    const std::string& varName)
{
    std::ostringstream fs;
    fs << "print(\"[ModelerAI] " << errPrefix << ": enter — pf=" << fsEscape(pfName)
       << " activity=" << fsEscape(actName) << " variable=" << fsEscape(varName) << "\\n\");\n";
    fs << emitPfResolveScript(errPrefix, pfName);
    fs << emitActivityResolveScript(errPrefix, actName, "activity", pfName, "iAct");

    // Resolve the table node. Leading '>' → hidden subnode (activity.find);
    // otherwise → regular variable (getvarnode).
    fs << "treenode tblNode = NULL;\n";
    if (!varName.empty() && varName[0] == '>') {
        fs << "tblNode = activity.find(\"" << fsEscape(varName) << "\");\n";
    } else {
        fs << "tblNode = getvarnode(activity, \"" << fsEscape(varName) << "\");\n";
    }
    fs << "if (!objectexists(tblNode)) { print(\"[ModelerAI] " << errPrefix << ": variable '" << fsEscape(varName)
       << "' not found on activity\\n\"); return \"ERR:variable_not_found:Variable \\\"" << fsEscape(varName)
       << "\\\" not found on activity \\\"" << fsEscape(actName) << "\\\".\"; }\n";
    return fs.str();
}

// Common ERR: parser shared by the activity-table tools. Mirrors the
// pattern in set_activity_variable.
nlohmann::json parseErrString(const std::string& s, const char* defaultCode)
{
    nlohmann::json err;
    if (s.rfind("ERR:", 0) == 0) {
        std::string rest = s.substr(4);
        auto sep = rest.find(':');
        err["code"] = (sep != std::string::npos) ? rest.substr(0, sep) : defaultCode;
        err["msg"]  = (sep != std::string::npos) ? rest.substr(sep + 1) : rest;
    } else {
        err["code"] = defaultCode;
        err["msg"]  = s;
    }
    return err;
}

// Native equivalent of emitActivityTableResolveScript (.1000084+ migration):
// resolve pf → activity → the variable's table/var node, with no executestring.
// Returns the var node, or nullptr with errCode/errMsg filled. A leading '>' in
// varName means a hidden subnode (activity.find); otherwise a bound variable
// (getvarnode).
TreeNode* resolveActivityVarNode(const std::string& pfName, const std::string& actName,
                                 const std::string& varName,
                                 std::string& errCode, std::string& errMsg)
{
    treenode pf = resolvePfNode(pfName);
    if (!objectexists(pf)) {
        errCode = "processflow_not_found";
        errMsg  = "No ProcessFlow named \"" + pfName + "\" found.";
        return nullptr;
    }
    treenode activity = resolveActivityNode(pf, actName);
    if (!objectexists(activity)) {
        errCode = "activity_not_found";
        errMsg  = "Activity \"" + actName + "\" not found in ProcessFlow \"" + pfName + "\".";
        return nullptr;
    }
    treenode varNode = nullptr;
    if (!varName.empty() && varName[0] == '>') varNode = activity->find(varName.c_str());
    else                                       varNode = getvarnode(activity, varName.c_str());
    if (!objectexists(varNode)) {
        errCode = "variable_not_found";
        errMsg  = "Variable \"" + varName + "\" not found on activity \"" + actName + "\".";
        return nullptr;
    }
    return varNode;
}

} // anonymous namespace

// ============================================================================
// modelerai_set_activity_table_cell
//   { processflow, activity, variable, row, col, value }
//
// Sets one cell of an activity's table-shaped variable.
//   - value is a JSON number → cell becomes a number node.
//   - value is a JSON string → cell becomes a string node (engine will
//     evaluate as literal vs FlexScript based on content; if you want
//     explicit FlexScript-mode for an ambiguous string, prepend `return `
//     and a trailing semicolon).
//
// Returns: { ok, processflow, activity, variable, row, col, value_kind }
// Errors:  processflow_not_found | activity_not_found | variable_not_found |
//          bad_index | missing_args | bad_args_json
// ============================================================================
modelerai_export Variant ModelerAi_setActivityTableCell(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_activity_table_cell expects { processflow, activity, "
                "variable, row, col, value }.");
        }

        std::string pfName, actName, varName;
        int row = 0, col = 0;
        nlohmann::json valueJson;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
            if (j.contains("row") && j["row"].is_number_integer()) row = j["row"].get<int>();
            if (j.contains("col") && j["col"].is_number_integer()) col = j["col"].get<int>();
            if (!j.contains("value")) return returnError("missing_args", "value is required.");
            valueJson = j["value"];
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }

        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");
        if (row < 1 || col < 1) return returnError("bad_index", "row and col are 1-indexed (>= 1).");

        // Native (.1000084 — was executestring). Resolve the table node, cast to
        // Table, auto-grow, then write the cell via cell()->value (the proven
        // VariantLValue path) / switch_flexscript / nodepoint.
        std::string errCode, errMsg;
        TreeNode* tblNode = resolveActivityVarNode(pfName, actName, varName, errCode, errMsg);
        if (!tblNode) return returnError(errCode.c_str(), errMsg);

        Table tbl(tblNode);
        // Auto-grow to fit the requested cell (never shrink existing data).
        int curRows = tbl.numRows, curCols = tbl.numCols;
        int newRows = row > curRows ? row : curRows;
        int newCols = col > curCols ? col : curCols;
        if (newRows != curRows || newCols != curCols) tbl.setSize(newRows, newCols, 0, 0);

        std::string valueKind;
        if (valueJson.is_number()) {
            valueKind = "number";
            tbl.cell(row, col)->value = Variant(valueJson.get<double>());
        } else if (valueJson.is_string()) {
            valueKind = "string";
            tbl.cell(row, col)->value = Variant(valueJson.get<std::string>().c_str());
        } else if (valueJson.is_object() && valueJson.contains("flexscript")
                   && valueJson["flexscript"].is_string()) {
            // FlexScript-mode cell: store the body + mark the node FlexScript so
            // the engine compiles/evaluates on read. Auto-prepend the codeHeader
            // so current/token/etc. are in scope (header field picks the variant).
            valueKind = "flexscript";
            std::string body = valueJson["flexscript"].get<std::string>();
            std::string headerMode = "standard";
            if (valueJson.contains("header") && valueJson["header"].is_string()) {
                headerMode = valueJson["header"].get<std::string>();
            }
            body = maybePrependCodeHeader(body, headerMode);
            TreeNode* cellNode = tbl.cell(row, col);
            cellNode->value = Variant(body.c_str());
            switch_flexscript(cellNode, 1);
            buildnodeflexscript(cellNode);
        } else if (valueJson.is_object() && valueJson.contains("model_object")
                   && valueJson["model_object"].is_string()) {
            // Pointer-to-model-object: make the cell a coupling node pointing at
            // Model.find(name) — pointer storage, not a string/FlexScript body.
            valueKind = "model_object";
            std::string objName = valueJson["model_object"].get<std::string>();
            TreeNode* targetObj = model()->find(objName.c_str());
            if (!objectexists(targetObj)) {
                return returnError("model_object_not_found",
                    "Model object \"" + objName + "\" not found.");
            }
            TreeNode* cellNode = tbl.cell(row, col);
            nodeadddata(cellNode, DATATYPE_COUPLING);
            nodepoint(cellNode, targetObj);
        } else {
            return returnError("unsupported_value_type",
                "value must be: a JSON number, a JSON string (stored as a "
                "string node — engine evaluates literal-vs-expression based on "
                "content), {flexscript:\"<body>\"} (force FlexScript-mode), or "
                "{model_object:\"Queue1\"} (pointer to a model object).");
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["row"]         = row;
        out["col"]         = col;
        out["value_kind"]  = valueKind;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_activity_table_cell", e.what()); }
      catch (...)                     { return returnException("set_activity_table_cell", "unknown"); }
}

// ============================================================================
// modelerai_get_activity_table_cell
//   { processflow, activity, variable, row, col }
//
// Reads one cell. Returns { ok, value, kind } where kind is
// "number" | "string" depending on the cell's stored dataType.
// ============================================================================
modelerai_export Variant ModelerAi_getActivityTableCell(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_activity_table_cell expects { processflow, activity, "
                "variable, row, col }.");
        }
        std::string pfName, actName, varName;
        int row = 0, col = 0;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
            if (j.contains("row") && j["row"].is_number_integer()) row = j["row"].get<int>();
            if (j.contains("col") && j["col"].is_number_integer()) col = j["col"].get<int>();
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");
        if (row < 1 || col < 1) return returnError("bad_index", "row and col are 1-indexed (>= 1).");

        // Native (.1000084 — was executestring). Resolve the table node, read the
        // cell node, classify by datatype (1=number, 2=string).
        std::string errCode, errMsg;
        TreeNode* tblNode = resolveActivityVarNode(pfName, actName, varName, errCode, errMsg);
        if (!tblNode) return returnError(errCode.c_str(), errMsg);

        Table tbl(tblNode);
        TreeNode* cellNode = tbl.cell(row, col);
        std::string kind, valStr;
        double valNum = 0.0;
        if (objectexists(cellNode)) {
            int dt = getdatatype(cellNode);
            if (dt == 1) {
                kind   = "number";
                valNum = static_cast<double>(cellNode->value);
            } else if (dt == 2) {
                kind   = "string";
                valStr = std::string(cellNode->value);
            } else {
                kind = "unknown";
                Variant cv = cellNode->value;
                if (cv.type == VariantType::String) valStr = std::string(cv);
            }
        } else {
            kind = "empty";
        }

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["row"]         = row;
        out["col"]         = col;
        out["kind"]        = kind;
        if      (kind == "number") out["value"] = valNum;
        else if (kind == "string") out["value"] = valStr;
        else                       out["value"] = valStr;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_activity_table_cell", e.what()); }
      catch (...)                     { return returnException("get_activity_table_cell", "unknown"); }
}

// ============================================================================
// modelerai_get_activity_table_size
//   { processflow, activity, variable }
//
// Returns: { ok, rows, cols, headers: [string, ...] }
// Headers correspond to column names — for ScheduleSource arrivals these are
// "Time", "Name", "Quantity", and any additional label column names.
// ============================================================================
modelerai_export Variant ModelerAi_getActivityTableSize(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_get_activity_table_size expects { processflow, activity, variable }.");
        }
        std::string pfName, actName, varName;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");

        // Native (.1000084 — was executestring). Read dims + column headers off
        // the Table directly.
        std::string errCode, errMsg;
        TreeNode* tblNode = resolveActivityVarNode(pfName, actName, varName, errCode, errMsg);
        if (!tblNode) return returnError(errCode.c_str(), errMsg);

        Table tbl(tblNode);
        int rows = tbl.numRows;
        int cols = tbl.numCols;
        nlohmann::json headers = nlohmann::json::array();
        for (int iC = 1; iC <= cols; ++iC) headers.push_back(tbl.getColHeader(iC));

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["rows"]        = rows;
        out["cols"]        = cols;
        out["headers"]     = std::move(headers);
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("get_activity_table_size", e.what()); }
      catch (...)                     { return returnException("get_activity_table_size", "unknown"); }
}

// ============================================================================
// modelerai_resize_activity_table
//   { processflow, activity, variable, rows, cols }
//
// Resizes the table. Growing preserves existing cells; shrinking truncates.
// ============================================================================
modelerai_export Variant ModelerAi_resizeActivityTable(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_resize_activity_table expects { processflow, activity, "
                "variable, rows, cols }.");
        }
        std::string pfName, actName, varName;
        int rows = -1, cols = -1;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
            if (j.contains("rows") && j["rows"].is_number_integer()) rows = j["rows"].get<int>();
            if (j.contains("cols") && j["cols"].is_number_integer()) cols = j["cols"].get<int>();
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");
        if (rows < 0 || cols < 0) return returnError("bad_size", "rows and cols must be >= 0.");

        // Native (.1000084 — was executestring). setSize(rows,cols,datatype,overwrite).
        std::string errCode, errMsg;
        TreeNode* tblNode = resolveActivityVarNode(pfName, actName, varName, errCode, errMsg);
        if (!tblNode) return returnError(errCode.c_str(), errMsg);

        Table tbl(tblNode);
        tbl.setSize(rows, cols, 0, 0);

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["rows"]        = rows;
        out["cols"]        = cols;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("resize_activity_table", e.what()); }
      catch (...)                     { return returnException("resize_activity_table", "unknown"); }
}

// ============================================================================
// modelerai_set_activity_table_column_header
//   { processflow, activity, variable, col, header }
//
// Sets a column's header. For ScheduleSource arrivals, the header of any
// column past column 3 becomes the label name assigned to created tokens.
// ============================================================================
modelerai_export Variant ModelerAi_setActivityTableColumnHeader(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_activity_table_column_header expects { processflow, "
                "activity, variable, col, header }.");
        }
        std::string pfName, actName, varName, header;
        int col = 0;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName  = j.value("processflow", std::string(""));
            actName = j.value("activity",    std::string(""));
            varName = j.value("variable",    std::string(""));
            header  = j.value("header",      std::string(""));
            if (j.contains("col") && j["col"].is_number_integer()) col = j["col"].get<int>();
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())  return returnError("missing_args", "processflow is required.");
        if (actName.empty()) return returnError("missing_args", "activity is required.");
        if (varName.empty()) return returnError("missing_args", "variable is required.");
        if (col < 1)         return returnError("bad_index", "col is 1-indexed (>= 1).");
        if (header.empty())  return returnError("missing_args", "header is required.");

        // Native (.1000084 — was executestring). setColHeader(col, name).
        std::string errCode, errMsg;
        TreeNode* tblNode = resolveActivityVarNode(pfName, actName, varName, errCode, errMsg);
        if (!tblNode) return returnError(errCode.c_str(), errMsg);

        Table tbl(tblNode);
        tbl.setColHeader(col, header.c_str());

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["variable"]    = varName;
        out["col"]         = col;
        out["header"]      = header;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_activity_table_column_header", e.what()); }
      catch (...)                     { return returnException("set_activity_table_column_header", "unknown"); }
}

// ============================================================================
// modelerai_set_create_object_target_label
//   { processflow, activity, label_name, append? }
//
// CreateObject token-label mode for `assignTo`: copy/replace the assignTo
// node from the library template (which carries the SDT-style "assign to
// token label" structure), set the second subnode (stringValue) to the
// label name, then set assignType to 0 (overwrite) or 1 (append).
//
// `append` defaults to false → assignType = 0 (overwrite).
// ============================================================================
modelerai_export Variant ModelerAi_setCreateObjectTargetLabel(FLEXSIMINTERFACE)
{
    try {
        Variant arg = param(1);
        if (arg.type != VariantType::String) {
            return returnError("missing_args",
                "modelerai_set_create_object_target_label expects "
                "{ processflow, activity, label_name, append? }.");
        }
        std::string pfName, actName, labelName;
        bool append = false;
        try {
            auto j = nlohmann::json::parse(std::string(arg));
            if (!j.is_object()) return returnError("bad_args_shape", "expected JSON object");
            pfName    = j.value("processflow", std::string(""));
            actName   = j.value("activity",    std::string(""));
            labelName = j.value("label_name",  std::string(""));
            if (j.contains("append")) {
                const auto& av = j["append"];
                if      (av.is_boolean()) append = av.get<bool>();
                else if (av.is_number())  append = av.get<double>() != 0.0;
            }
        } catch (const std::exception& e) {
            return returnError("bad_args_json", std::string("parse: ") + e.what());
        }
        if (pfName.empty())    return returnError("missing_args", "processflow is required.");
        if (actName.empty())   return returnError("missing_args", "activity is required.");
        if (labelName.empty()) return returnError("missing_args", "label_name is required.");

        // Native (.1000100 — was an executestring builder). Resolve PF + activity
        // with the shared native helpers; library template via node("MAIN:/..."),
        // instance var via getvarnode; copy the template over the instance
        // (TreeNode::copy + COPY_FLAG_REPLACE); set stringValue subnode + assignType.
        treenode pf = resolvePfNode(pfName);
        if (!objectexists(pf)) {
            return returnError("processflow_not_found", "ProcessFlow '" + pfName + "' not found.");
        }
        treenode activity = resolveActivityNode(pf, actName);
        if (!objectexists(activity)) {
            return returnError("activity_not_found",
                "Activity '" + actName + "' not found in ProcessFlow '" + pfName + "'.");
        }

        treenode libAssignTo = node(
            "MAIN:/project/library/processflow/activities/CreateObject>variables/assignTo", nullptr);
        if (!objectexists(libAssignTo)) {
            return returnError("library_template_missing",
                "CreateObject library template assignTo node not found.");
        }
        treenode instAssignTo = getvarnode(activity, "assignTo");
        if (!objectexists(instAssignTo)) {
            return returnError("variable_not_found",
                "assignTo variable not found on activity (is this a CreateObject?).");
        }

        // copy(destination, COPY_FLAG_REPLACE) replaces the instance node with a
        // copy of the template; the returned node is the new one (old handle may
        // be stale), so re-resolve if it comes back null.
        treenode newAssignTo = libAssignTo->copy(instAssignTo, COPY_FLAG_REPLACE);  // macro = 0x1
        if (!objectexists(newAssignTo)) newAssignTo = getvarnode(activity, "assignTo");
        if (!objectexists(newAssignTo) || content(newAssignTo) < 2) {
            return returnError("malformed_template",
                "assignTo template lacks the stringValue subnode (post-copy structure unexpected).");
        }
        treenode stringValueNode = rank(newAssignTo, 2);   // second subnode = stringValue
        if (!objectexists(stringValueNode)) {
            return returnError("malformed_template", "assignTo stringValue subnode missing.");
        }
        stringValueNode->value = Variant(labelName.c_str());
        setvarnum(activity, "assignType", append ? 1.0 : 0.0);   // 0=overwrite 1=append

        nlohmann::json out;
        out["ok"]          = true;
        out["processflow"] = pfName;
        out["activity"]    = actName;
        out["label_name"]  = labelName;
        out["append"]      = append;
        return returnJson(out);
    } catch (const std::exception& e) { return returnException("set_create_object_target_label", e.what()); }
      catch (...)                     { return returnException("set_create_object_target_label", "unknown"); }
}
