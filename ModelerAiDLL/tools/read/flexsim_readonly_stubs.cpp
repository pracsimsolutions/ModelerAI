// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "module.h"   // FlexSim SDK — must precede json.h
#include "tools/read/flexsim_readonly_stubs.h"
#include "mainthread/mainthread.h"
#include "third_party/json.h"

#include <map>
#include <string>
#include <vector>

namespace ModelerAi::tools {

namespace {

// ----- helpers (all called on the FlexSim main thread) ----------------

// Internal-tagged SDK nodes (their names start with '>') are not modeler-
// facing objects. Filter them out of all walks.
bool isInternalName(const std::string& nm)
{
    return !nm.empty() && nm[0] == '>';
}

// Match a "glob"-style pattern against a name. Supports:
//   "Source*"  -> prefix match
//   "*Queue"   -> suffix match
//   "*proc*"   -> substring match
//   "Source1"  -> exact match
// Empty pattern matches everything (used when name_filter is omitted).
bool matchesNameFilter(const std::string& name, const std::string& pattern)
{
    if (pattern.empty()) return true;
    bool leadingStar  = pattern.front() == '*';
    bool trailingStar = pattern.back() == '*';
    if (leadingStar && trailingStar && pattern.size() >= 2) {
        std::string mid = pattern.substr(1, pattern.size() - 2);
        return name.find(mid) != std::string::npos;
    }
    if (leadingStar) {
        std::string suffix = pattern.substr(1);
        return name.size() >= suffix.size()
               && name.compare(name.size() - suffix.size(), suffix.size(), suffix) == 0;
    }
    if (trailingStar) {
        std::string prefix = pattern.substr(0, pattern.size() - 1);
        return name.size() >= prefix.size()
               && name.compare(0, prefix.size(), prefix) == 0;
    }
    return name == pattern;
}

// Class name as the FlexSim library knows it (e.g. "Source", "Conveyor::StraightConveyor").
std::string classNameOf(treenode obj)
{
    treenode cls = classobject(obj);
    if (!cls) return "";
    return getname(cls);
}

// Build the parent path for an object — basic version: parent's name with "/"
// prefix, or "/" if directly under the model root. Sufficient for the AI to
// reference objects in follow-up calls.
std::string parentNameOf(treenode obj)
{
    if (!obj) return "";
    treenode up = obj->up;
    if (!up) return "/";
    // Detect "this is a direct child of the model root" by comparing the
    // parent's name to "MAIN:/project/model" — but easier: if parent name
    // is empty or it's the model itself, treat as root.
    std::string upName = getname(up);
    if (upName.empty() || up == model()) return "/";
    return "/" + upName;
}

// Emit one object entry per the chosen detail level. Filled into the
// `objects` JSON array.
nlohmann::json emitObjectEntry(treenode obj, const std::string& detail)
{
    nlohmann::json e;
    e["name"]   = getname(obj);
    e["class"]  = classNameOf(obj);
    e["parent"] = parentNameOf(obj);
    e["x"] = xloc(obj);
    e["y"] = yloc(obj);
    e["z"] = zloc(obj);

    if (detail == "structure") return e;

    // Connections (used by "connections" and "detailed").
    nlohmann::json outArr = nlohmann::json::array();
    int outCount = (int)nrop(obj);
    for (int i = 1; i <= outCount; ++i) {
        treenode dest = outobject(obj, i);
        if (dest) outArr.push_back(getname(dest));
    }
    nlohmann::json inArr = nlohmann::json::array();
    int inCount = (int)nrip(obj);
    for (int i = 1; i <= inCount; ++i) {
        treenode src = inobject(obj, i);
        if (src) inArr.push_back(getname(src));
    }
    nlohmann::json centerArr = nlohmann::json::array();
    int cpCount = (int)nrcp(obj);
    for (int i = 1; i <= cpCount; ++i) {
        treenode dest = centerobject(obj, i);
        if (dest) centerArr.push_back(getname(dest));
    }
    e["out"]    = std::move(outArr);
    e["in"]     = std::move(inArr);
    e["center"] = std::move(centerArr);

    if (detail == "connections") return e;

    // "detailed" — add size, rotation, color, labels.
    nlohmann::json sz;
    sz["sx"] = xsize(obj);
    sz["sy"] = ysize(obj);
    sz["sz"] = zsize(obj);
    e["size"] = std::move(sz);

    nlohmann::json rot;
    rot["rx"] = xrot(obj);
    rot["ry"] = yrot(obj);
    rot["rz"] = zrot(obj);
    e["rotation"] = std::move(rot);

    // Labels live under the object's ">labels" attribute subtree. For
    // the "detailed" level we emit the label NAMES only — value
    // extraction varies wildly by label type (number / string / treenode
    // / array) and the clean implementation lives in the deferred
    // get_object_properties + set_property work. The AI can get a
    // specific label's value via run_script + obj.LABELNAME until then.
    nlohmann::json labelNames = nlohmann::json::array();
    treenode labelsNode = obj->find(">labels");
    if (labelsNode) {
        int labelCount = (int)content(labelsNode);
        for (int i = 1; i <= labelCount; ++i) {
            treenode lbl = rank(labelsNode, i);
            if (!lbl) continue;
            std::string lblName = getname(lbl);
            if (lblName.empty()) continue;
            labelNames.push_back(lblName);
        }
    }
    e["label_names"] = std::move(labelNames);

    return e;
}

// Recursive walker. Visits `node` at current depth; descends if depth < maxDepth.
// `topLevel` flag controls whether we add `node` itself (true for root call,
// false when recursing into containers — we add the container's CHILDREN).
void walkAndCollect(treenode node,
                    int currentDepth,
                    int maxDepth,
                    const std::string& classFilter,
                    const std::string& nameFilter,
                    const std::string& detail,
                    nlohmann::json& outArr,
                    std::map<std::string, int>* classCounts)  // non-null when detail=summary
{
    if (!node) return;
    int subCount = (int)content(node);
    for (int i = 1; i <= subCount; ++i) {
        treenode child = rank(node, i);
        if (!child) continue;
        std::string childName = getname(child);
        if (isInternalName(childName)) continue;

        std::string childClass = classNameOf(child);

        bool classOk = classFilter.empty() || childClass == classFilter;
        bool nameOk  = matchesNameFilter(childName, nameFilter);
        bool match   = classOk && nameOk;

        if (match) {
            if (classCounts) {
                (*classCounts)[childClass.empty() ? std::string("(unknown)") : childClass] += 1;
            } else {
                outArr.push_back(emitObjectEntry(child, detail));
            }
        }

        // Recurse if there's more depth allowed.
        if (currentDepth + 1 < maxDepth) {
            walkAndCollect(child, currentDepth + 1, maxDepth,
                           classFilter, nameFilter, detail, outArr, classCounts);
        }
    }
}

} // namespace

// ============================================================
// get_model_structure
// ============================================================

std::string GetModelStructureTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "detail": {
      "type": "string",
      "enum": ["summary", "structure", "connections", "detailed"],
      "default": "structure",
      "description": "summary: counts by class. structure: per-object name + class + parent + position. connections: structure + A/S link arrays. detailed: connections + labels + size + rotation."
    },
    "class_filter": {
      "type": "string",
      "description": "Optional exact class name filter (e.g. 'Source', 'Conveyor::StraightConveyor')."
    },
    "name_filter": {
      "type": "string",
      "description": "Optional name pattern. Use * for wildcards: 'Source*' (prefix), '*Queue' (suffix), '*proc*' (contains), 'Source1' (exact)."
    },
    "parent_path": {
      "type": "string",
      "description": "Optional scope to a sub-tree. When set, walking starts from this object instead of the model root."
    },
    "max_depth": {
      "type": "integer",
      "minimum": 1,
      "default": 1,
      "description": "How many levels deep to walk. Default 1 = top-level only. Raise to drill into containers."
    }
  },
  "additionalProperties": false
})";
}

ToolResult GetModelStructureTool::run(std::string_view args_json)
{
    // Parse args on worker thread.
    nlohmann::json args = nlohmann::json::object();
    if (!args_json.empty() && args_json != "{}") {
        try {
            args = nlohmann::json::parse(args_json);
        } catch (const std::exception& e) {
            ToolResult r;
            r.ok = false;
            r.error_code = "bad_args_json";
            r.error_message = std::string("Couldn't parse args: ") + e.what();
            return r;
        }
    }

    std::string detail     = args.value("detail",       std::string("structure"));
    std::string classFlt   = args.value("class_filter", std::string());
    std::string nameFlt    = args.value("name_filter",  std::string());
    std::string parentPath = args.value("parent_path",  std::string());
    int maxDepth           = args.value("max_depth",    1);
    if (maxDepth < 1) maxDepth = 1;

    if (detail != "summary" && detail != "structure"
        && detail != "connections" && detail != "detailed")
    {
        ToolResult r;
        r.ok = false;
        r.error_code = "invalid_detail";
        r.error_message = "detail must be one of: summary, structure, connections, detailed.";
        return r;
    }

    // Main-thread work — read the tree, collect.
    struct ReadResult {
        bool ok;
        std::string error_code;
        std::string error_message;
        nlohmann::json json;
    };

    ReadResult result = mainthread::runAndWait([&]() -> ReadResult {
        ReadResult r{};

        // Pick the root for the walk.
        TreeNode* root = nullptr;
        if (parentPath.empty()) {
            root = model();
        } else {
            root = model()->find(parentPath.c_str());
            if (!objectexists(root)) {
                r.ok = false;
                r.error_code = "parent_not_found";
                r.error_message = "parent_path '" + parentPath + "' did not resolve.";
                return r;
            }
        }

        if (detail == "summary") {
            std::map<std::string, int> counts;
            nlohmann::json dummyArr = nlohmann::json::array();
            walkAndCollect(root, 0, maxDepth, classFlt, nameFlt, detail, dummyArr, &counts);
            nlohmann::json by = nlohmann::json::object();
            int total = 0;
            for (auto& kv : counts) {
                by[kv.first] = kv.second;
                total += kv.second;
            }
            nlohmann::json out;
            out["total_objects"] = total;
            out["by_class"]      = std::move(by);
            r.ok = true;
            r.json = std::move(out);
            return r;
        }

        // structure / connections / detailed all share the per-object emit path.
        nlohmann::json arr = nlohmann::json::array();
        walkAndCollect(root, 0, maxDepth, classFlt, nameFlt, detail, arr, nullptr);
        nlohmann::json out;
        out["objects"] = std::move(arr);
        r.ok = true;
        r.json = std::move(out);
        return r;
    });

    if (!result.ok) {
        ToolResult tr;
        tr.ok = false;
        tr.error_code = result.error_code;
        tr.error_message = result.error_message;
        return tr;
    }

    ToolResult tr;
    tr.ok = true;
    tr.result_json = result.json.dump();
    return tr;
}

// ============================================================
// get_object_properties (deferred stub)
// ============================================================

std::string GetObjectPropertiesTool::params_schema() const
{
    return R"({
  "type": "object",
  "properties": {
    "object_name": {
      "type": "string",
      "description": "Name or path of the object to inspect."
    }
  },
  "required": ["object_name"],
  "additionalProperties": false
})";
}

ToolResult GetObjectPropertiesTool::run(std::string_view /*args_json*/)
{
    return makeNotImplementedReply(name());
}

} // namespace ModelerAi::tools
