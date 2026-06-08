// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // FlexSim SDK — must precede json.h
#include "tools/write/run_script.h"
#include "commands/antipattern.h"
#include "commands/identifier_validator.h"
#include "mainthread/mainthread.h"
#include "third_party/json.h"
#include "bootstrap.h"
#include "kb/kb_index.h"
#include "tree/condense.h"

#include <regex>
#include <set>
#include <string>

namespace ModelerAi::tools {

namespace {

// Render a Variant returned by executestring() into a JSON value. Shallow
// for arrays/maps to keep token cost bounded.
nlohmann::json variantToJson(const Variant& v)
{
    switch (v.type) {
        case VariantType::Null:
            return nullptr;
        case VariantType::Number: {
            double d = static_cast<double>(v);
            if (d == static_cast<double>(static_cast<long long>(d))
                && d >= -9.2e18 && d <= 9.2e18) {
                return static_cast<long long>(d);
            }
            return d;
        }
        case VariantType::String: {
            return std::string(v);
        }
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
            for (int idx = 1; idx <= a.length; ++idx) {
                arr.push_back(variantToJson(a[idx]));
            }
            return arr;
        }
        case VariantType::Map: {
            nlohmann::json obj = nlohmann::json::object();
            try {
                Map m = static_cast<Map>(v);
                Array keys = m.keys;
                for (int idx = 1; idx <= keys.length; ++idx) {
                    std::string k = std::string(keys[idx]);
                    obj[k] = variantToJson(m[keys[idx]]);
                }
            } catch (...) {}
            return obj;
        }
        default:
            return nullptr;
    }
}

// Snapshot of top-level model state — names of direct children under MAIN:/
// model. Used to compute a delta around run_script execution so the AI
// CANNOT claim success without proof. Cheap: one shallow walk per call.
struct ModelSnapshot {
    std::vector<std::string> topLevelNames;
};

ModelSnapshot snapshotModel()
{
    ModelSnapshot s;
    try {
        TreeNode* m = model();
        if (!m) return s;
        int n = 0;
        try { n = m->subnodes.length; } catch (...) {}
        s.topLevelNames.reserve(static_cast<size_t>(n));
        for (int i = 1; i <= n; ++i) {
            TreeNode* c = nullptr;
            try { c = m->subnodes[i]; } catch (...) {}
            if (!c) continue;
            const char* nm = nullptr;
            try { nm = c->getName(); } catch (...) {}
            s.topLevelNames.emplace_back(nm ? nm : "");
        }
    } catch (...) {}
    return s;
}

// Build the verification block — what changed in the model between the
// before- and after- snapshots. Always emitted; the AI can use it as
// ground truth instead of claiming "I checked, they're there."
nlohmann::json verificationBlock(const ModelSnapshot& before,
                                 const ModelSnapshot& after)
{
    std::set<std::string> beforeSet(before.topLevelNames.begin(), before.topLevelNames.end());
    std::set<std::string> afterSet (after.topLevelNames.begin(),  after.topLevelNames.end());

    nlohmann::json added   = nlohmann::json::array();
    nlohmann::json removed = nlohmann::json::array();
    for (const auto& nm : after.topLevelNames) {
        if (!beforeSet.count(nm)) added.push_back(nm);
    }
    for (const auto& nm : before.topLevelNames) {
        if (!afterSet.count(nm)) removed.push_back(nm);
    }

    nlohmann::json v;
    v["top_level_count_before"] = static_cast<int>(before.topLevelNames.size());
    v["top_level_count_after"]  = static_cast<int>(after.topLevelNames.size());
    v["top_level_added"]        = std::move(added);
    v["top_level_removed"]      = std::move(removed);
    return v;
}

// Pull identifier names out of a FlexScript error message that the model
// likely hallucinated. The chief patterns FlexSim emits are:
//   "Unknown command <name> being called"
//   "Unknown function <name> being called"
//   "Unknown method <name>"
// Returns the deduplicated set in encounter order.
std::vector<std::string> extractUnknownIdents(const std::string& errMsg)
{
    std::vector<std::string> out;
    std::set<std::string>    seen;
    static const std::regex re(R"(Unknown\s+(?:command|function|method)\s+(\w+))",
                               std::regex::ECMAScript | std::regex::icase);
    auto begin = std::sregex_iterator(errMsg.begin(), errMsg.end(), re);
    auto end   = std::sregex_iterator();
    for (auto it = begin; it != end; ++it) {
        std::string ident = (*it)[1].str();
        if (ident.empty()) continue;
        if (seen.insert(ident).second) out.push_back(std::move(ident));
    }
    return out;
}

// For each unknown identifier, run an in-process KB search and return the
// top hits as a JSON array. Empty array if no KB or no matches. The point
// is to put real command names in front of the model AT THE POINT OF
// FAILURE so it has somewhere to retarget instead of guessing again.
nlohmann::json kbSuggestionsFor(const std::vector<std::string>& idents)
{
    nlohmann::json out = nlohmann::json::array();
    auto* idx = ModelerAi::bootstrap::kbIndex();
    if (!idx || !idx->loaded() || idx->size() == 0) return out;

    for (const std::string& ident : idents) {
        ModelerAi::kb::SearchOptions opts;
        opts.limit = 5;
        opts.includeDeprecated = false;
        auto hits = idx->search(ident, opts);
        if (hits.empty()) continue;

        nlohmann::json group;
        group["unknown"] = ident;
        nlohmann::json arr = nlohmann::json::array();
        for (const auto& h : hits) {
            if (h.topicIdx >= idx->topics().size()) continue;
            const auto& t = idx->topics()[h.topicIdx];
            nlohmann::json e;
            e["id"]   = t.id;
            e["name"] = t.name;
            e["kind"] = t.kind;
            if (!t.signature.empty())   e["signature"]   = t.signature;
            if (!t.description.empty()) e["description"] = t.description;
            if (!t.module.empty())      e["module"]      = t.module;
            if (!t.group.empty())       e["group"]       = t.group;
            arr.push_back(std::move(e));
        }
        group["candidates"] = std::move(arr);
        out.push_back(std::move(group));
    }
    return out;
}

} // namespace

std::string RunScriptTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "script": {
      "type": "string",
      "description": "FlexScript source to execute. May span multiple statements. End with `return <expr>;` to surface a result back to the chat. WITHOUT `return`, the script still runs and any side effects on the model take effect, but the tool result will be `null`. Inside the script, `current`, `involved`, and `param(1)` are NOT bound (those are event-handler conventions that don't apply here). Use `Model.find(\"path/to/object\")` to address objects. Anything else valid in a FlexScript Script Console session is valid here."
    },
    "label": {
      "type": "string",
      "description": "Short human-readable label describing what this script does (e.g. 'Set Source1 interArrivalTime to 30'). Shown in the approval modal so the modeler can decide whether to allow this snippet to run."
    }
  },
  "required": ["script", "label"],
  "additionalProperties": false
})";
}

ToolResult RunScriptTool::run(std::string_view args_json)
{
    ToolResult r;
    std::string script;
    std::string label;

    try {
        auto j = nlohmann::json::parse(args_json);
        if (!j.contains("script") || !j["script"].is_string()) {
            r.ok = false;
            r.error_code = "missing_script";
            r.error_message = "run_script requires a 'script' string.";
            return r;
        }
        script = j["script"].get<std::string>();
        if (j.contains("label") && j["label"].is_string()) {
            label = j["label"].get<std::string>();
        }
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }

    if (script.empty()) {
        r.ok = false;
        r.error_code = "empty_script";
        r.error_message = "run_script was called with an empty script.";
        return r;
    }

    // Antipattern check — reject known-bad FlexScript forms BEFORE executing.
    // Catches the API hallucinations the AI keeps producing despite prompt
    // directives (createObject, connectPorts, getoutput, while(time())step()
    // loops, etc.). The structured error names both the deprecated form AND
    // the modern alternative, so the model fixes course in one round-trip.
    {
        std::string anName, anRem;
        if (ModelerAi::scanAntiPatterns(script, anName, anRem)) {
            r.ok = false;
            r.error_code = "deprecated_api";
            r.error_message = anRem;
            // Embed structured detail so the model can program against it
            // (and the viewer can render a richer error bubble).
            nlohmann::json detail;
            detail["antipattern"] = anName;
            detail["script"]      = script;
            r.result_json = detail.dump();
            return r;
        }
    }

    // Identifier validator — the structural enforcement. Parses every
    // top-level `name(...)` call shape in the script and rejects names
    // that aren't in the KB-derived catalog (active commands +
    // classes + modelerai_* prefix + a small language-keyword
    // whitelist). Method calls (`obj.foo()`, `Class::bar()`) are
    // skipped — can't be typechecked against a static set. When an
    // unknown is found, the error response embeds KB search results
    // for it so the AI's next turn sees real candidates instead of
    // guessing again. Deprecated names are warned but allowed to
    // execute (the replacement is included in the warning).
    ModelerAi::ValidationResult vr = ModelerAi::validateIdentifiers(script);
    if (vr.hasUnknown()) {
        std::vector<std::string> unknownsForKb;
        std::vector<std::pair<std::string,std::string>> didYouMeans; // (bad, suggested)
        nlohmann::json findings = nlohmann::json::array();
        for (const auto& f : vr.findings) {
            if (f.status != ModelerAi::ValidationFinding::Unknown) continue;
            nlohmann::json e;
            e["name"]        = f.name;
            e["approx_line"] = f.approx_line;
            if (!f.didYouMean.empty()) {
                e["did_you_mean"] = f.didYouMean;
                didYouMeans.emplace_back(f.name, f.didYouMean);
            } else {
                // Only ask the KB for help when cascade recovery failed.
                unknownsForKb.push_back(f.name);
            }
            findings.push_back(std::move(e));
        }

        nlohmann::json detail;
        detail["script"]   = script;
        detail["unknowns"] = std::move(findings);
        if (!unknownsForKb.empty()) {
            detail["suggestions"] = kbSuggestionsFor(unknownsForKb);
        }

        // Build a singular, specific repair message naming the
        // failing identifier(s) and the direct correction when one
        // exists. AlphaCodium / Aider lesson: "specific" beats
        // "try again."
        std::string msg;
        if (!didYouMeans.empty()) {
            msg = "Identifier validator rejected this script. ";
            msg += "Replace ";
            for (size_t i = 0; i < didYouMeans.size(); ++i) {
                if (i) msg += "; ";
                msg += "`" + didYouMeans[i].first + "` with `" + didYouMeans[i].second + "`";
            }
            msg += ".";
            if (!unknownsForKb.empty()) {
                msg += " Also unknown (no exact match found — see `suggestions`): ";
                for (size_t i = 0; i < unknownsForKb.size(); ++i) {
                    if (i) msg += ", ";
                    msg += "`" + unknownsForKb[i] + "`";
                }
                msg += ".";
            }
            msg += " Retry once with the corrections; do not invent further names.";
        } else {
            msg = "Identifier validator rejected this script. Unknown name(s): ";
            for (size_t i = 0; i < unknownsForKb.size(); ++i) {
                if (i) msg += ", ";
                msg += "`" + unknownsForKb[i] + "`";
            }
            msg += ". These do not exist in FlexSim — they are hallucinations. "
                   "See `suggestions` for KB matches per name. Pick the best "
                   "match, do NOT retry with a similar-looking guess. If you "
                   "are sure the name should exist, tell the modeler — do not "
                   "invent it.";
        }

        r.ok = false;
        r.error_code = "unknown_identifiers";
        r.error_message = std::move(msg);
        r.result_json = detail.dump();
        return r;
    }

    // Deprecated warnings — passed through to the success response so
    // the AI is nudged toward the replacement without blocking the
    // current run. (Some deprecated commands still work; the modeler
    // may have legitimate reasons for the legacy form.)
    nlohmann::json deprecatedWarnings = nlohmann::json::array();
    if (vr.hasDeprecated()) {
        for (const auto& f : vr.findings) {
            if (f.status != ModelerAi::ValidationFinding::Deprecated) continue;
            nlohmann::json e;
            e["name"]        = f.name;
            e["approx_line"] = f.approx_line;
            if (!f.replacedBy.empty()) e["replaced_by"] = f.replacedBy;
            deprecatedWarnings.push_back(std::move(e));
        }
    }

    struct ScriptOutcome {
        bool           ok = false;
        std::string    error;
        nlohmann::json result;
        ModelSnapshot  before;
        ModelSnapshot  after;
    };

    ScriptOutcome outcome = mainthread::runAndWait([&]() -> ScriptOutcome {
        ScriptOutcome o;
        // Snapshot BEFORE so we can show the AI exactly what its script
        // changed (or didn't). Cheap.
        o.before = snapshotModel();
        // Arbitrary FlexScript can touch anything in the tree; assume
        // mutation and invalidate the SNOWBALL cache. Read-only scripts
        // pay one extra walk on the next turn (acceptable cost vs the
        // risk of stale state from a script we couldn't statically
        // classify).
        ModelerAi::tree::invalidateCondenseCache();
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            o.ok = true;
            o.result = variantToJson(v);
        } catch (const std::exception& e) {
            o.error = e.what();
        } catch (const char* msg) {
            o.error = msg ? msg : "FlexScript error (unknown)";
        } catch (...) {
            o.error = "FlexScript threw a non-standard exception.";
        }
        // Snapshot AFTER even on failure — partial scripts can leave
        // partial mutations, and the AI needs to see those before
        // retrying.
        o.after = snapshotModel();
        return o;
    });

    nlohmann::json verification = verificationBlock(outcome.before, outcome.after);

    if (!outcome.ok) {
        r.ok = false;
        r.error_code = "flexscript_error";
        r.error_message = outcome.error.empty()
            ? "FlexScript execution failed."
            : ("FlexScript error: " + outcome.error);

        // Recovery scaffold: when FlexSim says "Unknown command X being
        // called", pull X out, search the KB for it, and embed the top
        // hits as `suggestions`. The AI's next turn sees a structured
        // "here's what actually exists" alongside the raw error — much
        // more likely to fix course than the bare error alone.
        nlohmann::json detail;
        detail["script"]       = script;
        detail["raw_error"]    = outcome.error;
        detail["verification"] = verification; // ground truth even on failure
        std::vector<std::string> idents = extractUnknownIdents(outcome.error);
        if (!idents.empty()) {
            detail["hallucinated_identifiers"] = idents;
            nlohmann::json sugg = kbSuggestionsFor(idents);
            if (!sugg.empty()) {
                detail["suggestions"] = std::move(sugg);
                r.error_message += "\n\nKB suggestions: see `suggestions` in detail. "
                                   "DO NOT guess again — pick a real command from there, "
                                   "or call `search_kb` for more options.";
            } else {
                r.error_message += "\n\nThose identifiers aren't in the KB either. "
                                   "Call `search_kb` for what you're actually trying to do, "
                                   "or `list_commands` for the full active-command catalog.";
            }
        }
        r.result_json = detail.dump();
        return r;
    }

    nlohmann::json out;
    out["label"]        = label;
    out["result"]       = outcome.result;
    out["script"]       = script;
    out["verification"] = std::move(verification);
    if (!deprecatedWarnings.empty()) {
        out["deprecated_warnings"] = std::move(deprecatedWarnings);
    }
    r.ok = true;
    r.result_json = out.dump();
    return r;
}

} // namespace ModelerAi::tools
