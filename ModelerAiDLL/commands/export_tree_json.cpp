// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/export_tree_json.cpp — modelerai_export_tree_json.
//
// Walks a tree subtree (selected node, treenode arg, or string path) and
// emits an annotated JSON dump. For DATA_OBJECT nodes the walk includes
// BOTH the model-tree subnodes (`subnodes`) AND the per-object attribute
// tree (`attributes`) — the latter is where >superclasses, >variables,
// >behaviour, etc. live and is invisible to a plain subnodes-only walker.
//
// Two output modes:
//   "file"   (default): writes to <modeldir>/Examples/exports/<name>.json,
//                       returns the absolute path.
//   "inline":           returns the JSON payload directly. No file written.
//                       Intended for AI-callable use via modelerai_call.
//
// Design doc: docs/specs/2026-06-01-tree-json-exporter.md.
//
// Args (all optional except target — defaults shown):
//   target                  : path string | treenode | null (use selection)
//   output                  : "file" | "inline"             (default "inline")
//   max_depth               : int >= 0, 0 = unlimited       (default 0)
//   max_nodes               : int >= 0, 0 = unlimited       (default 0)
//   include_subnodes        : bool                          (default true)
//   include_attributes      : bool                          (default true)
//   include_bundle_data     : bool                          (default false)
//   include_byteblock_value : bool                          (default true)
//   include_float_value     : bool                          (default true)
//   include_coupling_target : bool                          (default true)
//   max_string_length       : int >= 0, 0 = unlimited       (default 4096)
//   skip_names              : array of node names to prune  (default [])
//
//   Hard internal cap of 64 recursion levels prevents stack overflow even
//   when max_depth=0; we only crash on truly malformed cyclic trees.
//
//   When a limit fires, the root payload gains a `truncated` object with
//   counters: { reason, node_count, nodes_skipped, depth_clipped }.
//
// Return: Variant string. Path on file-mode success, JSON payload on
// inline-mode success, or "ERROR: <reason>" on any failure.

#include "module.h"
#include "commands/export_tree_json.h"
#include "third_party/json.h"

#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <system_error>
#include <unordered_set>

namespace {

// One result buffer per thread — the Variant returns a const char* that
// must outlive the return. Matches the convention in commands.cpp.
thread_local std::string g_resultBuffer;

Variant returnRawString(const std::string& s)
{
    g_resultBuffer = s;
    return Variant(g_resultBuffer.c_str());
}

Variant returnErr(const std::string& msg)
{
    return returnRawString("ERROR: " + msg);
}

// Build a node's full tree path. Empty string on failure.
std::string nodePath(TreeNode* n)
{
    if (!n) return "";
    try {
        const char* p = nodetomodelpath_cstr(n, 1);
        if (p) return p;
    } catch (...) {}
    return "";
}

struct WalkOptions {
    int  max_depth               = 0;     // 0 = unlimited (capped at kHardDepthCap)
    int  max_nodes               = 0;     // 0 = unlimited
    bool include_subnodes        = true;
    bool include_attributes      = true;
    bool include_bundle_data     = false;
    bool include_byteblock_value = true;
    bool include_float_value     = true;
    bool include_coupling_target = true;
    int  max_string_length       = 4096;  // 0 = unlimited
    std::unordered_set<std::string> skip_names;
};

// Hard ceiling on recursion regardless of caller's max_depth — protects the
// FlexSim stack against malformed/cyclic trees. Real trees are nowhere near
// this depth.
constexpr int kHardDepthCap = 64;

struct WalkState {
    const WalkOptions& opts;
    int  node_count    = 0;   // nodes successfully emitted
    int  nodes_skipped = 0;   // pruned by max_nodes / skip_names
    int  depth_clipped = 0;   // recursions cut short by max_depth
    bool over_budget   = false;

    explicit WalkState(const WalkOptions& o) : opts(o) {}

    bool nodeBudgetExhausted() const {
        return opts.max_nodes > 0 && node_count >= opts.max_nodes;
    }
};

// Fall back to the model-tree selection. selectedobject(target) wants a
// treenode (the view/window the selection lives in); model() is the
// standard root for "what's selected in the modeling tree." If nothing
// is selected there, return nullptr — caller surfaces an error.
TreeNode* fallbackSelection()
{
    try {
        TreeNode* s = selectedobject(model());
        if (s) return s;
    } catch (...) {}
    return nullptr;
}

// Helper: get the structured args JSON from param(1) if it parses as an
// object. Returns nullptr if not present / not an object.
std::unique_ptr<nlohmann::json> tryParseArgs(const Variant& targetArg)
{
    if (targetArg.type != VariantType::String) return nullptr;
    std::string s = std::string(targetArg);
    if (s.empty() || s.front() != '{') return nullptr;
    try {
        auto j = nlohmann::json::parse(s);
        if (j.is_object()) return std::make_unique<nlohmann::json>(std::move(j));
    } catch (...) {}
    return nullptr;
}

// Resolve param(1) into the root node to export. Sets outErr and returns
// nullptr on failure. Supports treenode reference, string path, and the
// modelerai_call structured form (a JSON object stringified into param(1)).
TreeNode* resolveTarget(const Variant& targetArg, std::string& outErr)
{
    if (targetArg.type == VariantType::TreeNode) {
        TreeNode* n = static_cast<TreeNode*>(targetArg);
        if (!n) { outErr = "node reference is null"; return nullptr; }
        return n;
    }

    if (targetArg.type == VariantType::String) {
        std::string s = std::string(targetArg);

        if (auto args = tryParseArgs(targetArg)) {
            if (args->contains("target")) {
                if ((*args)["target"].is_string()) {
                    std::string p = (*args)["target"].get<std::string>();
                    if (p.empty()) {
                        TreeNode* sel = fallbackSelection();
                        if (!sel) { outErr = "no node selected and target was empty"; return nullptr; }
                        return sel;
                    }
                    TreeNode* n = node(p.c_str(), nullptr);
                    if (!n) { outErr = "node not found at path '" + p + "'"; return nullptr; }
                    return n;
                }
                if ((*args)["target"].is_null()) {
                    TreeNode* sel = fallbackSelection();
                    if (!sel) { outErr = "no node selected and target was null"; return nullptr; }
                    return sel;
                }
            }
            // Object form with no target → use selection (lets callers pass
            // {output:"inline", max_depth:3} and rely on selection).
            TreeNode* sel = fallbackSelection();
            if (!sel) { outErr = "no node selected and args object had no target"; return nullptr; }
            return sel;
        }

        if (s.empty()) {
            TreeNode* sel = fallbackSelection();
            if (!sel) { outErr = "no node selected and no node/path argument given"; return nullptr; }
            return sel;
        }

        TreeNode* n = node(s.c_str(), nullptr);
        if (!n) { outErr = "node not found at path '" + s + "'"; return nullptr; }
        return n;
    }

    if (targetArg.type == VariantType::Null) {
        TreeNode* sel = fallbackSelection();
        if (!sel) { outErr = "no node selected and no node/path argument given"; return nullptr; }
        return sel;
    }

    outErr = "unsupported argument type — pass a treenode or a path string";
    return nullptr;
}

// Output mode. Positional param(2) wins; otherwise check for "output" in
// the JSON object form passed via param(1).
std::string resolveOutputMode(const Variant& targetArg, const Variant& outputArg)
{
    if (outputArg.type == VariantType::String) {
        std::string s = std::string(outputArg);
        if (!s.empty()) return s;
    }
    if (auto args = tryParseArgs(targetArg)) {
        if (args->contains("output") && (*args)["output"].is_string())
            return (*args)["output"].get<std::string>();
    }
    // Default inline (.1000058): the agent wants the JSON returned, and "file"
    // fails on an unsaved model ("could not resolve model directory"). Pass
    // output:"file" explicitly to write to disk instead.
    return "inline";
}

// Pull walker flags from the structured args JSON if present. Leaves
// WalkOptions defaults in place for anything not specified.
WalkOptions resolveWalkOptions(const Variant& targetArg)
{
    WalkOptions opts;
    auto args = tryParseArgs(targetArg);
    if (!args) return opts;

    auto& j = *args;
    auto readInt = [&](const char* k, int& out) {
        if (j.contains(k) && j[k].is_number_integer()) {
            int v = j[k].get<int>();
            if (v >= 0) out = v;
        }
    };
    auto readBool = [&](const char* k, bool& out) {
        if (j.contains(k) && j[k].is_boolean()) out = j[k].get<bool>();
    };

    readInt ("max_depth",               opts.max_depth);
    readInt ("max_nodes",               opts.max_nodes);
    readBool("include_subnodes",        opts.include_subnodes);
    readBool("include_attributes",      opts.include_attributes);
    readBool("include_bundle_data",     opts.include_bundle_data);
    readBool("include_byteblock_value", opts.include_byteblock_value);
    readBool("include_float_value",     opts.include_float_value);
    readBool("include_coupling_target", opts.include_coupling_target);
    readInt ("max_string_length",       opts.max_string_length);

    if (j.contains("skip_names") && j["skip_names"].is_array()) {
        for (const auto& v : j["skip_names"]) {
            if (v.is_string()) opts.skip_names.insert(v.get<std::string>());
        }
    }
    return opts;
}

// Replace Windows-illegal filename chars (<>:"/\|?*) and control chars
// with a single space; collapse runs of spaces; trim edges. Underscores
// are never inserted — the original name is preserved as faithfully as
// the filesystem allows. Empty result on degenerate input.
std::string sanitizeFilename(const std::string& raw)
{
    static const char* illegal = "<>:\"/\\|?*";
    std::string buf;
    buf.reserve(raw.size());
    for (unsigned char c : raw) {
        if (c < 0x20 || std::strchr(illegal, c)) buf += ' ';
        else                                     buf += static_cast<char>(c);
    }
    std::string collapsed;
    collapsed.reserve(buf.size());
    bool prevSpace = false;
    for (char c : buf) {
        if (c == ' ') {
            if (!prevSpace) collapsed += c;
            prevSpace = true;
        } else {
            collapsed += c;
            prevSpace = false;
        }
    }
    size_t start = collapsed.find_first_not_of(' ');
    size_t end   = collapsed.find_last_not_of(' ');
    if (start == std::string::npos) return "";
    return collapsed.substr(start, end - start + 1);
}

// Truncate `s` to max_string_length, appending an ellipsis marker that
// records the original length. max=0 → no limit.
std::string clipString(const std::string& s, int max)
{
    if (max <= 0 || (int)s.size() <= max) return s;
    return s.substr(0, max) + "…[truncated " + std::to_string(s.size() - max) + " chars]";
}

// Recursive walker. Builds a JSON object for `n` and its descendants
// into `out`. Couplings emit target path and skip recursion; everything
// else recurses into subnodes (and DATA_OBJECT attribute trees) in tree
// order, respecting state.opts.
void walk(TreeNode* n, nlohmann::json& out, WalkState& state, int depth)
{
    if (!n) { out = nullptr; return; }

    // Node-budget gate: stop emitting full nodes once max_nodes is hit.
    if (state.nodeBudgetExhausted()) {
        out["name"]      = "…[budget exhausted]";
        out["truncated"] = true;
        ++state.nodes_skipped;
        state.over_budget = true;
        return;
    }
    ++state.node_count;

    const char* nameC = nullptr;
    try { nameC = n->getName(); } catch (...) {}
    std::string name = nameC ? nameC : "";

    out["name"] = name;
    {
        std::string p = nodePath(n);
        if (!p.empty()) out["path"] = p;
    }

    int dt = 0;
    try { dt = n->dataType; } catch (...) {}
    out["dataType"] = dt;

    // Kind detection — first match wins.
    if (dt == DATA_POINTERCOUPLING) {
        out["kind"] = "coupling";
        if (state.opts.include_coupling_target) {
            TreeNode* partner = nullptr;
            try {
                CouplingDataType* c = n->object<CouplingDataType>();
                if (c) partner = c->partner().get();
            } catch (...) {}
            if (partner) {
                std::string pp = nodePath(partner);
                if (!pp.empty()) out["target"] = pp;
                const char* pname = nullptr;
                try { pname = partner->getName(); } catch (...) {}
                if (pname && *pname) out["target_name"] = std::string(pname);
            } else {
                out["target"] = nullptr;
            }
        }
        // Couplings do not recurse — they are edges, not containers.
        return;
    }

    if (dt == DATA_OBJECT) {
        out["kind"] = "object";
        try {
            ObjectDataType* odt = n->object<ObjectDataType>();
            if (odt) {
                const char* cf = odt->getClassFactory();
                if (cf && *cf) out["class"] = std::string(cf);
            }
        } catch (...) {}
    } else if (dt == DATA_SIMPLE) {
        out["kind"] = "sdt";
        try {
            SimpleDataType* sdt = n->object<SimpleDataType>();
            if (sdt) {
                const char* cf = sdt->getClassFactory();
                if (cf && *cf) out["class"] = std::string(cf);
            }
        } catch (...) {}
    } else if (!name.empty() && name.front() == '>') {
        out["kind"] = "attribute";
    } else if (dt == DATA_FLOAT) {
        out["kind"] = "data";
        if (state.opts.include_float_value) {
            try {
                double v = static_cast<double>(n->value);
                if (v == static_cast<double>(static_cast<long long>(v))
                    && v >= -9.2e18 && v <= 9.2e18) {
                    out["value"] = static_cast<long long>(v);
                } else {
                    out["value"] = v;
                }
            } catch (...) {}
        }
    } else if (dt == DATA_BYTEBLOCK) {
        out["kind"] = "data";
        if (state.opts.include_byteblock_value) {
            try {
                std::string s = std::string(getnodestr(n));
                out["value"] = clipString(s, state.opts.max_string_length);
            } catch (...) {}
        }
    } else if (dt == DATA_BUNDLE) {
        out["kind"]       = "data";
        out["value_kind"] = "bundle";
        // Bundles are table-data blobs (e.g. GlobalTable rows). Dumping
        // them inline can balloon the output dramatically. Default is
        // "marker only"; opt-in flag is reserved for a future enumerator.
        if (state.opts.include_bundle_data) {
            out["bundle_data_note"] = "include_bundle_data=true is reserved; "
                                       "no bundle enumerator wired yet.";
        }
    } else {
        out["kind"] = "container";
    }

    // Depth limit applies BEFORE we descend into children. Hard cap always
    // applies; caller's max_depth applies when > 0.
    int nextDepth = depth + 1;
    bool depthExceeded =
        (nextDepth > kHardDepthCap) ||
        (state.opts.max_depth > 0 && nextDepth > state.opts.max_depth);

    if (depthExceeded) {
        out["truncated"] = "max_depth";
        ++state.depth_clipped;
        return;
    }

    auto walkChildren = [&](TreeNode* root, const char* outKey, int n_children) {
        nlohmann::json children = nlohmann::json::array();
        for (int i = 1; i <= n_children; ++i) {
            TreeNode* child = nullptr;
            try { child = root->subnodes[i]; } catch (...) {}
            if (!child) continue;
            if (state.nodeBudgetExhausted()) {
                state.over_budget = true;
                ++state.nodes_skipped;
                continue;
            }
            const char* cnameC = nullptr;
            try { cnameC = child->getName(); } catch (...) {}
            std::string cname = cnameC ? cnameC : "";
            if (!cname.empty() && state.opts.skip_names.count(cname)) {
                ++state.nodes_skipped;
                continue;
            }
            nlohmann::json childJson;
            walk(child, childJson, state, nextDepth);
            children.push_back(std::move(childJson));
        }
        if (!children.empty()) out[outKey] = std::move(children);
    };

    if (state.opts.include_subnodes) {
        int n_children = 0;
        try { n_children = n->subnodes.length; } catch (...) {}
        if (n_children > 0) walkChildren(n, "subnodes", n_children);
    }

    // DATA_OBJECT nodes carry a separate attribute tree (the "object tree")
    // that holds >variables, >superclasses, >classes, >behaviour, etc.
    // The model-tree subnode walk above never sees it. ObjectDataType's
    // engine-exported getObjectTree() returns its root; SDTs return null
    // from their override so this never expands for non-object nodes.
    if (dt == DATA_OBJECT && state.opts.include_attributes) {
        try {
            ObjectDataType* odt = n->object<ObjectDataType>();
            if (odt) {
                TreeNode* attrRoot = odt->getObjectTree();
                if (attrRoot) {
                    int n_attrs = 0;
                    try { n_attrs = attrRoot->subnodes.length; } catch (...) {}
                    if (n_attrs > 0) walkChildren(attrRoot, "attributes", n_attrs);
                }
            }
        } catch (...) {}
    }
}

} // anonymous namespace

modelerai_export Variant ModelerAi_ExportTreeJson(FLEXSIMINTERFACE)
{
    try {
        Variant targetArg = param(1);
        Variant outputArg = param(2);

        std::string errMsg;
        TreeNode* root = resolveTarget(targetArg, errMsg);
        if (!root) return returnErr(errMsg);

        std::string outputMode = resolveOutputMode(targetArg, outputArg);
        if (outputMode != "file" && outputMode != "inline") {
            return returnErr("invalid output mode '" + outputMode
                             + "' — expected 'file' or 'inline'");
        }

        WalkOptions opts = resolveWalkOptions(targetArg);
        WalkState   state(opts);

        nlohmann::json tree;
        walk(root, tree, state, 0);

        if (state.over_budget || state.depth_clipped > 0 || state.nodes_skipped > 0) {
            nlohmann::json trunc;
            if (state.over_budget) trunc["reason"] = "max_nodes";
            else if (state.depth_clipped > 0) trunc["reason"] = "max_depth";
            else trunc["reason"] = "skip_names";
            trunc["node_count"]    = state.node_count;
            trunc["nodes_skipped"] = state.nodes_skipped;
            trunc["depth_clipped"] = state.depth_clipped;
            tree["truncated"] = std::move(trunc);
        }

        std::string payload = tree.dump(2);

        if (outputMode == "inline") {
            return returnRawString(payload);
        }

        // file mode: <modeldir>/Examples/exports/<sanitized-name>.json
        std::string rootName;
        try {
            const char* n = root->getName();
            if (n) rootName = n;
        } catch (...) {}

        std::string fname = sanitizeFilename(rootName);
        if (fname.empty()) {
            int rnk = 0;
            try { rnk = root->rank; } catch (...) {}
            fname = "node_" + std::to_string(rnk);
        }
        fname += ".json";

        std::string dir;
        try {
            const char* md = modeldir_cstr();
            if (md) dir = md;
        } catch (...) {}
        if (dir.empty()) {
            return returnErr("could not resolve model directory (model not saved?)");
        }

        std::filesystem::path outDir = std::filesystem::path(dir) / "Examples" / "exports";
        std::error_code ec;
        std::filesystem::create_directories(outDir, ec);
        if (ec) {
            return returnErr("failed to create '" + outDir.string() + "': " + ec.message());
        }
        std::filesystem::path outPath = outDir / fname;

        std::ofstream f(outPath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!f.is_open()) {
            return returnErr("failed to open '" + outPath.string() + "' for writing");
        }
        f.write(payload.data(), static_cast<std::streamsize>(payload.size()));
        f.close();
        if (f.fail()) {
            return returnErr("write to '" + outPath.string() + "' failed");
        }

        return returnRawString(outPath.string());
    } catch (const std::exception& e) {
        return returnErr(std::string("exception: ") + (e.what() ? e.what() : "unknown"));
    } catch (...) {
        return returnErr("exception: non-std");
    }
}
