// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // FlexSim SDK — must precede json.h
#include "tools/write/modelerai_call.h"
#include "mainthread/mainthread.h"
#include "commands/commands.h"   // ModelerAi_* exports (the dispatcher resolves them by name)
#include "agent/agent.h"         // ModelerAi::agent::Agent::cancelRequested
#include "bootstrap.h"           // ModelerAi::bootstrap::agent()
#include "third_party/json.h"
#include "tree/condense.h"

#include <chrono>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>

namespace ModelerAi::tools {

namespace {

// Escape a string for embedding as a FlexScript string literal. Order
// matters: backslashes first so we don't double-escape escapes we add.
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

// Render one JSON arg as a FlexScript literal that applicationcommand will
// accept. Strings/numbers/bools pass through as primitives. Objects and
// arrays are JSON-encoded and passed as a string literal — the receiving
// command's body parses them back. This is the convention for "complex
// args": the command takes a single string param holding the JSON. Null
// becomes an empty string. Always returns true (no shape is invalid).
void jsonArgToFlexScript(const nlohmann::json& v, std::string& out)
{
    if (v.is_string()) {
        out = "\"" + fsEscape(v.get<std::string>()) + "\"";
        return;
    }
    if (v.is_number_integer()) {
        out = std::to_string(v.get<long long>());
        return;
    }
    if (v.is_number()) {
        std::ostringstream s;
        s << v.get<double>();
        out = s.str();
        return;
    }
    if (v.is_boolean()) {
        out = v.get<bool>() ? "1" : "0";
        return;
    }
    if (v.is_null()) {
        out = "\"\"";
        return;
    }
    // Object / array → JSON-encoded string literal. The receiving command
    // reads param(N) as a string and json-decodes it.
    out = "\"" + fsEscape(v.dump()) + "\"";
}

// Convert the result Variant from applicationcommand into a JSON value the
// agent / model can read. Strings, numbers, nulls; treenodes get rendered
// as { treenode_path, treenode_name }. Arrays + maps render shallowly.
// Mirrors the helper RunScriptTool uses — kept local because modelerai_call
// is the only consumer.
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
            std::string s = std::string(v);
            // Commands by convention return a JSON-encoded string from their
            // body. Try to parse it back as JSON so the agent sees structured
            // data, not a string blob. If parsing fails the raw string flows
            // through unchanged.
            try {
                return nlohmann::json::parse(s);
            } catch (...) {
                return s;
            }
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

} // namespace

std::string ModelerAiCallTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "name": {
      "type": "string",
      "pattern": "^modelerai_[a-z][a-z0-9_]*$",
      "description": "The curated command to invoke (lowercase + underscore convention throughout — e.g. \"modelerai_create_object\", \"modelerai_connect_fixed_resources\", \"modelerai_inspect_connections\"). See modelerai-quickref for the current tool surface."
    },
    "args": {
      "description": "Arguments for the command. Most tools take a SINGLE STRUCTURED OBJECT (the schema is per-tool — see modelerai-quickref for each command's args shape). The object is JSON-encoded and passed as the first positional arg. Examples:\n  - modelerai_create_object: { class: \"Source\", name?: \"Src1\", location?: [x,y,z], rotation?: [x,y,z], size?: [x,y,z], parent?: \"Plane1\" }\n  - modelerai_delete_object: \"Source1\"  (or { name: \"Source1\" })\n  - modelerai_clone_object: { source: \"Src1\", offset?: [dx,dy,dz], count?: N, copy_connections?: bool, parent?, name? }\n  - modelerai_connect_fixed_resources: { from: \"Source1\", to: \"Queue1\" }\n  - modelerai_connect_task_executer_to_navigator: { executer: \"Operator1\", navigator: \"AGVNetwork1\", point?: \"GISPoint1\" }\n  - modelerai_connect_fixed_resource_to_navigator: { fr: \"Processor1\", anchor: \"ControlPoint5\" }\n  - modelerai_connect_dispatcher_to_task_executer: { dispatcher: \"Dispatcher1\", executer: \"Operator1\" }\n  - modelerai_connect_centerport: { from: \"Processor1\", to: \"Operator1\" }\n  - modelerai_disconnect: { from, to, kind: \"flow\"|\"centerport\"|\"navigator\" }\n  - modelerai_inspect_connections: \"Source1\"  (or { object: \"Source1\" })\nLegacy positional-array form (args: [a, b, c, ...]) is still accepted for compatibility but the per-tool object shape is preferred and is what each tool's docstring specifies.",
      "anyOf": [
        { "type": "object" },
        { "type": "string" },
        { "type": "array", "items": { "type": ["string", "number", "boolean", "object", "array", "null"] } },
        { "type": "null" }
      ]
    }
  },
  "required": ["name"]
})";
}

ToolResult ModelerAiCallTool::run(std::string_view args_json)
{
    ToolResult r;
    std::string commandName;
    nlohmann::json args;

    try {
        auto j = nlohmann::json::parse(args_json);
        if (!j.contains("name") || !j["name"].is_string()) {
            r.ok = false;
            r.error_code = "missing_name";
            r.error_message = "modelerai_call expects a 'name' string argument.";
            return r;
        }
        commandName = j["name"].get<std::string>();

        // Be tolerant about `args` shape — the AI sometimes passes a single
        // value or object instead of wrapping it in an array. All four
        // shapes converge to the same semantics:
        //   array  → positional args as-is
        //   object → wrap as [object]  (one structured arg; gets JSON-encoded)
        //   string → wrap as [string]  (one positional string arg)
        //   missing/null → no args
        if (!j.contains("args") || j["args"].is_null()) {
            args = nlohmann::json::array();
        } else if (j["args"].is_array()) {
            args = j["args"];
        } else if (j["args"].is_object() || j["args"].is_string()
                || j["args"].is_number() || j["args"].is_boolean()) {
            args = nlohmann::json::array();
            args.push_back(j["args"]);
        } else {
            r.ok = false;
            r.error_code = "bad_args_shape";
            r.error_message = "modelerai_call: 'args' must be an array, object, string, primitive, or omitted.";
            return r;
        }
    } catch (const std::exception& e) {
        r.ok = false;
        r.error_code = "bad_args_json";
        r.error_message = std::string("Couldn't parse args JSON: ") + e.what();
        return r;
    }

    if (commandName.rfind("modelerai_", 0) != 0) {
        r.ok = false;
        r.error_code = "bad_command_name";
        r.error_message = "modelerai_call: 'name' must start with 'modelerai_'. Got: " + commandName;
        return r;
    }

    // SNOWBALL cache invalidation. Read-only commands leave the cache
    // alone so the next turn's condense reuses the previous tree walk
    // (saves ~200-500ms per turn on non-trivial models). Anything else
    // is treated as potentially mutating — invalidate to force a fresh
    // walk on the next turn. Keep this allowlist conservative: missing
    // a true read-only command costs one extra walk per call (small);
    // missing a true mutator costs the AI seeing stale state (big).
    static const std::unordered_set<std::string> kReadOnly = {
        "modelerai_ping",
        "modelerai_inspect_connections",
        "modelerai_list_performance_measures",
        "modelerai_get_performance_measure",
        "modelerai_get_performance_measures",
        "modelerai_get_object_stats",
        "modelerai_get_model_summary",
        "modelerai_get_run_state",
        "modelerai_export_tree_json",
        "modelerai_list_processflows",
        "modelerai_list_activities",
        "modelerai_get_activity_info",
        "modelerai_get_activity_variable",
        "modelerai_get_activity_table_cell",
        "modelerai_get_activity_table_size",
        "modelerai_list_picks",
        "modelerai_get_pick",
    };
    if (!kReadOnly.count(commandName)) {
        ModelerAi::tree::invalidateCondenseCache();
    }

    // ---- Special path: modelerai_run_to_time -----------------------------
    //
    // We tried two architectures before this one and both failed:
    //   1. Sync C++ on main thread with `MsgWaitForMultipleObjects` +
    //      message pump → sim never advanced (`time()==0` every poll).
    //   2. C++20 coroutine on main thread with `co_await Delay::realTime(.1)`
    //      → sim ran to the target but the coroutine never resumed after
    //      its first yield. Suspect: under `runspeed(INT_MAX)` FlexSim is
    //      processing events so aggressively it never serves the
    //      wall-clock Delay timer; when sim hits the stop time, FlexSim
    //      halts but may not service queued wall-clock timers anymore.
    //      The coroutine's resume never gets called.
    //
    // What works here: keep the wait on the WORKER thread, drive each
    // small main-thread operation through `runAndWait`. The main thread
    // owns FlexSim's event loop for the long stretches between our polls,
    // sim advances normally, and the worker stays responsive (cancel +
    // timeout safety).
    //
    // Diagnostics from this code path land in System Console as
    // `[runToTime] …` via FlexScript `print(...)` inside the runAndWait
    // closures (worker-thread consolePrint isn't safe).
    if (commandName == "modelerai_run_to_time") {
        // Extract target_sim_time from args.
        double target = -1.0;
        if (!args.empty()) {
            const auto& a = args[0];
            if (a.is_number()) {
                target = a.get<double>();
            } else if (a.is_object()) {
                for (const char* k : { "target_sim_time", "target", "time", "sim_time" }) {
                    if (a.contains(k) && a[k].is_number()) {
                        target = a[k].get<double>();
                        break;
                    }
                }
            } else if (a.is_string()) {
                try {
                    auto j = nlohmann::json::parse(a.get<std::string>());
                    if (j.is_object()) {
                        for (const char* k : { "target_sim_time", "target", "time", "sim_time" }) {
                            if (j.contains(k) && j[k].is_number()) {
                                target = j[k].get<double>();
                                break;
                            }
                        }
                    } else if (j.is_number()) {
                        target = j.get<double>();
                    }
                } catch (...) {}
            }
        }
        if (target <= 0.0) {
            r.ok = false;
            r.error_code = "bad_target";
            r.error_message = "modelerai_run_to_time expects a numeric "
                              "target_sim_time > 0. Shapes: bare number "
                              "100000, or JSON {\"target_sim_time\": 100000}.";
            return r;
        }

        // Unique marker so cleanup finds OUR temp stop, even if a prior
        // crashed run left a stale one behind.
        long long stamp = static_cast<long long>(
            std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count());
        std::string marker = "modelerai-rtt:" + std::to_string(stamp);

        // -- Phase 1: install + go (single main-thread incursion).
        std::string installScript;
        installScript += "treenode stopTimes = getmodelunit(STOP_TIME_NODE);\n";
        installScript += "if (!stopTimes) { print(\"[runToTime] STOP_TIME_NODE not found\"); return 0; }\n";
        installScript += "treenode newStop = createcopy(stopTimes.last, stopTimes, 1);\n";
        installScript += "setsdtvalue(newStop, \"enabled\", 1);\n";
        installScript += "function_s(newStop, \"setModelTime\", "
                       + std::to_string(target) + ");\n";
        installScript += "setsdtvalue(newStop, \"dateString\", \"" + marker + "\");\n";
        installScript += "stoptime(0, 0);\n";
        installScript += "resetmodel(1);\n";
        installScript += "applicationcommand(\"switchRunning\", 0);\n";
        installScript += "runspeed(INT_MAX);\n";
        installScript += "go();\n";
        installScript += "print(\"[runToTime] INSTALL ok target=" + std::to_string(target)
                       + " nextStop=\", Model.nextStopTime);\n";
        installScript += "return Model.nextStopTime;\n";

        double nextStopTime = target;
        std::string installErr;
        ModelerAi::mainthread::runAndWait([&]() {
            try {
                Variant v = executestring(installScript.c_str(),
                                          nullptr, nullptr, Variant());
                if (v.type == VariantType::Number) nextStopTime = double(v);
            } catch (const std::exception& e) {
                installErr = e.what();
            }
        });
        if (!installErr.empty()) {
            r.ok = false;
            r.error_code = "install_failed";
            r.error_message = "Install script threw: " + installErr;
            return r;
        }

        // -- Phase 2: poll until exit condition fires.
        ModelerAi::agent::Agent* agent = ModelerAi::bootstrap::agent();
        double lastCheckTime = 0.0;
        int    earlyBreak    = 0;  // 0=target, 1=drained, 2=stall, 3=canceled, 4=timeout
        int    iter          = 0;
        auto   wallStart     = std::chrono::steady_clock::now();
        constexpr auto kHardTimeout   = std::chrono::minutes(10);
        constexpr auto kPollInterval  = std::chrono::milliseconds(100);

        while (true) {
            std::this_thread::sleep_for(kPollInterval);
            ++iter;

            if (agent && agent->cancelRequested()) {
                earlyBreak = 3;
                break;
            }
            if (std::chrono::steady_clock::now() - wallStart >= kHardTimeout) {
                earlyBreak = 4;
                break;
            }

            double currentTime = 0.0;
            int    eventQty    = 0;
            int    runState    = 0;
            bool   readOk      = false;
            ModelerAi::mainthread::runAndWait([&]() {
                try {
                    Variant tv = executestring("return time();", nullptr, nullptr, Variant());
                    if (tv.type == VariantType::Number) currentTime = double(tv);
                    Variant ev = executestring("return eventqty();", nullptr, nullptr, Variant());
                    if (ev.type == VariantType::Number) eventQty = (int)double(ev);
                    Variant rs = executestring("return getrunstate();", nullptr, nullptr, Variant());
                    if (rs.type == VariantType::Number) runState = (int)double(rs);
                    readOk = true;
                } catch (...) {}
            });

            if (iter == 1 || iter % 10 == 0) {
                ModelerAi::mainthread::runAndWait([&]() {
                    std::string msg = "print(\"[runToTime] POLL iter="
                                    + std::to_string(iter)
                                    + " time=\", time(), \" events=\", eventqty(),"
                                    + " \" runState=\", getrunstate());";
                    try { executestring(msg.c_str(), nullptr, nullptr, Variant()); }
                    catch (...) {}
                });
            }

            if (!readOk) continue;  // transient SDK hiccup; try again next tick

            if (currentTime == target) {
                break;  // target reached
            }

            if ((currentTime - lastCheckTime) < 1.0) {
                if (eventQty == 0) {
                    earlyBreak = 1;
                    break;
                }
                if (currentTime == nextStopTime && runState == 0) {
                    // Multi-stop continuation: advance past a user stop time.
                    ModelerAi::mainthread::runAndWait([&]() {
                        try {
                            Variant nv = executestring("return Model.nextStopTime;",
                                                       nullptr, nullptr, Variant());
                            if (nv.type == VariantType::Number) nextStopTime = double(nv);
                            executestring("go();", nullptr, nullptr, Variant());
                        } catch (...) {}
                    });
                } else {
                    earlyBreak = 2;
                    break;
                }
            }
            lastCheckTime = currentTime;
        }

        // -- Phase 3: cleanup (single main-thread incursion).
        std::string cleanupScript;
        if (earlyBreak == 3 || earlyBreak == 4) {
            cleanupScript += "stop(1);\n";
        }
        cleanupScript += "applicationcommand(\"switchRunning\", 0);\n";
        cleanupScript += "treenode stopTimes = getmodelunit(STOP_TIME_NODE);\n";
        cleanupScript += "if (stopTimes && stopTimes.subnodes.length > 1) {\n";
        cleanupScript += "    for (int i = stopTimes.subnodes.length; i >= 1; i--) {\n";
        cleanupScript += "        treenode n = stopTimes.subnodes[i];\n";
        cleanupScript += "        if (string(getsdtvalue(n, \"dateString\")) == \""
                       + marker + "\") {\n";
        cleanupScript += "            n.destroy();\n";
        cleanupScript += "            break;\n";
        cleanupScript += "        }\n";
        cleanupScript += "    }\n";
        cleanupScript += "}\n";
        cleanupScript += "stoptime(0, 0);\n";
        cleanupScript += "print(\"[runToTime] CLEANUP ok finalTime=\", time());\n";
        cleanupScript += "return time();\n";

        double finalTime = target;
        ModelerAi::mainthread::runAndWait([&]() {
            try {
                Variant v = executestring(cleanupScript.c_str(),
                                          nullptr, nullptr, Variant());
                if (v.type == VariantType::Number) finalTime = double(v);
            } catch (...) {}
        });

        // -- Phase 4: build result.
        nlohmann::json result;
        std::string reason;
        bool completed = false;
        switch (earlyBreak) {
            case 0: reason = "target_reached"; completed = true; break;
            case 1: reason = "events_drained"; break;
            case 2: reason = "stall_detected"; break;
            case 3: reason = "canceled";        break;
            case 4: reason = "wall_timeout";    break;
        }
        result["ok"]              = true;
        result["completed"]       = completed;
        result["reason"]          = reason;
        result["target_sim_time"] = target;
        result["final_sim_time"]  = finalTime;
        if (earlyBreak == 1) {
            result["note"] = "Event queue drained before reaching target. "
                             "Model ran out of work (e.g. ArrivalSchedule "
                             "exhausted, all items reached Sink).";
        } else if (earlyBreak == 2) {
            result["note"] = "Sim time stopped advancing between 100ms polls. "
                             "Likely a runaway trigger body or zero-time event "
                             "storm.";
        } else if (earlyBreak == 3) {
            result["note"] = "Run canceled by the chat's Stop button or the agent.";
        } else if (earlyBreak == 4) {
            result["note"] = "Hard 10-minute wall-clock timeout reached. Sim may "
                             "still be running in FlexSim — check the toolbar.";
        }

        nlohmann::json out;
        out["command"] = commandName;
        out["result"]  = result;
        r.ok           = true;
        r.result_json  = out.dump();
        return r;
    }

    // Build the FlexScript that calls applicationcommand with the marshalled
    // args. Using executestring instead of a direct variadic C++ call sidesteps
    // the FlexSim SDK's lack of a clean variadic applicationcommand binding.
    //
    // Tools that need long-running waits (e.g. modelerai_run_to_time) take
    // a special path above — see kSpecialCommands logic. Everything else
    // follows the normal executestring → applicationcommand → DLL chain.
    std::string script;
    script.reserve(64 + commandName.size() + args.size() * 16);
    script += "return applicationcommand(\"" + fsEscape(commandName) + "\"";
    for (size_t i = 0; i < args.size(); ++i) {
        std::string lit;
        jsonArgToFlexScript(args[i], lit);
        script += ", " + lit;
    }
    script += ");";

    // Marshal to the FlexSim main thread — executestring + every SDK call the
    // underlying command makes are NOT thread-safe.
    struct Outcome {
        bool ok = false;
        std::string error;
        nlohmann::json result;
    };

    Outcome outcome = mainthread::runAndWait([&]() -> Outcome {
        Outcome o;
        try {
            Variant v = executestring(script.c_str(), nullptr, nullptr, Variant());
            o.ok     = true;
            o.result = variantToJson(v);
        } catch (const std::exception& e) {
            o.error = e.what();
        } catch (const char* msg) {
            o.error = msg ? msg : "FlexScript error (unknown)";
        } catch (...) {
            o.error = "FlexScript threw a non-standard exception.";
        }
        return o;
    });

    if (!outcome.ok) {
        r.ok = false;
        r.error_code = "dispatch_failed";
        r.error_message = "modelerai_call '" + commandName + "' failed: "
                        + (outcome.error.empty() ? "(no error message)" : outcome.error);
        return r;
    }

    nlohmann::json out;
    out["command"] = commandName;
    out["result"]  = outcome.result;
    r.ok          = true;
    r.result_json = out.dump();
    return r;
}

} // namespace ModelerAi::tools
