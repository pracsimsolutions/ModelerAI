// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // FlexSim SDK — must precede json.h
#include "tools/write/modelerai_call.h"
#include "mainthread/mainthread.h"
#include "third_party/json.h"
#include "tree/condense.h"

#include <sstream>
#include <string>
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
    };
    if (!kReadOnly.count(commandName)) {
        ModelerAi::tree::invalidateCondenseCache();
    }

    // Build the FlexScript that calls applicationcommand with the marshalled
    // args. Using executestring instead of a direct variadic C++ call sidesteps
    // the FlexSim SDK's lack of a clean variadic applicationcommand binding.
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
