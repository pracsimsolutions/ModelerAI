// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "tree/condense.h"
#include "module.h"        // pulls in FlexsimDefs.h + allobjects.h
#include "bridge/log.h"    // consolePrint

#include <cmath>
#include <cstdio>
#include <sstream>
#include <string>
#include <vector>

namespace ModelerAi::tree {

namespace {

// Format a double — drop trailing zeros, drop the decimal point if it
// trails. Keeps the outline readable for typical FlexSim integer-ish
// positions (5, 10, 15) without losing precision when they aren't.
std::string fmtCoord(double v)
{
    char buf[32];
    snprintf(buf, sizeof(buf), "%g", v);
    return std::string(buf);
}

bool nameIsInternal(const std::string& nm)
{
    return !nm.empty() && nm[0] == '>';
}

std::string classNameOf(treenode obj)
{
    treenode cls = classobject(obj);
    if (!cls) return "";
    return getname(cls);
}

// Get the Model.find()-compatible path for an object. This is what the AI
// must use when calling any tool that resolves an object — it includes:
//   - parent chain when nested (e.g. "Plane1/Source1")
//   - ~N suffix when there are duplicate names (e.g. "Source1~2")
// Falls back to the bare name if nodetomodelpath_cstr fails for any reason.
std::string findPathOf(treenode obj)
{
    if (!obj) return "";
    try {
        const char* p = nodetomodelpath_cstr(obj, 1);
        if (p && *p) return p;
    } catch (...) {}
    return getname(obj);
}

void emitObject(std::ostringstream& os, treenode obj, int depth)
{
    if (!obj) return;
    if (depth > 5) return;  // sanity cap on recursive nesting

    std::string nm = getname(obj);
    if (nameIsInternal(nm)) return;

    std::string cls = classNameOf(obj);
    std::string path = findPathOf(obj);

    // Indent by depth for readability of nested objects (e.g. things
    // inside a Plane). The PATH the AI must use for Model.find() is
    // already in the path string, so depth is purely visual.
    for (int i = 0; i <= depth; ++i) os << "  ";
    os << path;
    if (!cls.empty()) os << " (" << cls << ")";

    double x = xloc(obj);
    double y = yloc(obj);
    double z = zloc(obj);
    if (x != 0.0 || y != 0.0 || z != 0.0) {
        os << " @[" << fmtCoord(x) << ", " << fmtCoord(y) << ", " << fmtCoord(z) << "]";
    }
    os << "\n";

    int outCount = (int)nrop(obj);
    for (int i = 1; i <= outCount; ++i) {
        treenode dest = outobject(obj, i);
        if (dest) {
            for (int j = 0; j <= depth + 1; ++j) os << "  ";
            os << "output[" << (i - 1) << "]\xE2\x86\x92 " << findPathOf(dest) << "\n";
            // \xE2\x86\x92 is UTF-8 for U+2192 RIGHTWARDS ARROW
        }
    }
    int centerCount = (int)nrcp(obj);
    for (int i = 1; i <= centerCount; ++i) {
        treenode dest = centerobject(obj, i);
        if (dest) {
            for (int j = 0; j <= depth + 1; ++j) os << "  ";
            os << "center[" << (i - 1) << "]\xE2\x86\x92 " << findPathOf(dest) << "\n";
        }
    }

    // Recurse into child objects (subnodes that are themselves Objects —
    // i.e., have a class). This catches things placed inside a Plane,
    // VisualTool, etc. Skips attribute subnodes (>variables, >labels, etc.)
    // and non-Object subnodes.
    int nChildren = (int)content(obj);
    for (int i = 1; i <= nChildren; ++i) {
        treenode child = obj->subnodes[i];
        if (!child) continue;
        std::string childNm = getname(child);
        if (nameIsInternal(childNm)) continue;
        if (classNameOf(child).empty()) continue;  // not an Object — skip
        emitObject(os, child, depth + 1);
    }
}

} // namespace

namespace {

// Emit a flat list of children's names under `container`, skipping
// internal `>`-prefixed attribute nodes. Used for Tools subtrees
// (Groups, GlobalTables, etc.) where we just want the names.
void emitToolGroup(std::ostringstream& os, treenode container, const char* heading)
{
    if (!container) return;
    int n = content(container);
    if (n <= 0) return;

    std::vector<std::string> names;
    for (int i = 1; i <= n; ++i) {
        treenode child = container->subnodes[i];
        if (!child) continue;
        std::string nm = getname(child);
        if (nameIsInternal(nm)) continue;
        if (nm.empty()) continue;
        names.push_back(std::move(nm));
    }
    if (names.empty()) return;

    os << "  " << heading << ": ";
    for (size_t i = 0; i < names.size(); ++i) {
        if (i) os << ", ";
        os << names[i];
    }
    os << "\n";
}

// Best-effort accessor for the model's run state. FlexSim stores this
// at /project/exec/runstate; falls back to "unknown" on access failure
// rather than crashing the condense.
std::string currentRunState()
{
    try {
        treenode rs = node("/project/exec/runstate", nullptr);
        if (rs) {
            int v = (int)rs->value;
            switch (v) {
                case 0: return "stopped";
                case 1: return "running";
                case 2: return "paused";
                default: return "state=" + std::to_string(v);
            }
        }
    } catch (...) {}
    return "unknown";
}

std::string currentSimTime()
{
    try {
        double t = time();
        char buf[32];
        snprintf(buf, sizeof(buf), "%g", t);
        return buf;
    } catch (...) {}
    return "?";
}

} // namespace

// ----------------------------------------------------------------------------
// Cache state. The expensive part of condenseActiveModel() is the recursive
// tree walk + per-object nodetomodelpath_cstr lookups. That part only
// changes when something in the tree is added/removed/renamed/reparented.
// Sim-state (run_state + sim time) changes constantly during a run but is
// trivial to read — we always recompute it fresh and only cache the walk
// output.
//
// Cache invalidation is COOPERATIVE: every mutating modelerai_* command
// calls invalidateCondenseCache() before returning. run_script does the
// same (since arbitrary FlexScript can touch anything). Read-only tools
// (inspect_connections, get_*, list_*) leave the cache alone — the next
// turn reuses the previous walk for free (~200-500ms savings on
// non-trivial models).
//
// Thread safety: both functions only run on the FlexSim main thread
// (condenseActiveModel is called via mainthread::runAndWait from the agent
// worker; invalidate is called from the tool dispatch which is also
// main-thread). No mutex needed.
// ----------------------------------------------------------------------------
namespace {
std::string g_cachedModelBlock;  // simulation/model/tools — sans sim_state header
bool        g_cacheDirty = true; // start dirty so first turn always walks
}

void invalidateCondenseCache()
{
    g_cacheDirty = true;
}

namespace {
// Walk the model + Tools subtrees and produce the model-block string.
// This is the EXPENSIVE work that gets cached.
std::string buildModelBlock()
{
    std::ostringstream os;
    treenode m = model();
    if (!m) return "model: (null)\n";

    int n = content(m);
    int emitted = 0;
    if (n > 0) {
        os << "model:\n";
        for (int i = 1; i <= n; ++i) {
            treenode child = m->subnodes[i];
            if (!child) continue;
            std::string nm = getname(child);
            if (nameIsInternal(nm)) continue;
            emitObject(os, child, 0);
            ++emitted;
        }
        if (emitted == 0) {
            os.str("");
            os << "model: (empty)\n";
        }
    } else {
        os << "model: (empty)\n";
    }

    try {
        treenode toolsRoot = node("/Tools", nullptr);
        if (toolsRoot) {
            std::ostringstream toolsOs;
            int beforeLen = (int)toolsOs.tellp();
            emitToolGroup(toolsOs, node("/Tools/Groups",                nullptr), "Groups");
            emitToolGroup(toolsOs, node("/Tools/GlobalTables",          nullptr), "GlobalTables");
            emitToolGroup(toolsOs, node("/Tools/StatisticsCollectors",  nullptr), "StatisticsCollectors");
            emitToolGroup(toolsOs, node("/Tools/PerformanceMeasures",   nullptr), "PerformanceMeasures");
            emitToolGroup(toolsOs, node("/Tools/TimeTables",            nullptr), "TimeTables");
            emitToolGroup(toolsOs, node("/Tools/MTBFMTTR",              nullptr), "MTBFMTTR");
            emitToolGroup(toolsOs, node("/Tools/UserEvents",            nullptr), "UserEvents");
            emitToolGroup(toolsOs, node("/Tools/Lists",                 nullptr), "Lists");
            emitToolGroup(toolsOs, node("/Tools/TrackedVariables",      nullptr), "TrackedVariables");
            emitToolGroup(toolsOs, node("/Tools/ParameterTables",       nullptr), "ParameterTables");
            std::string toolsStr = toolsOs.str();
            if ((int)toolsStr.size() > beforeLen) {
                os << "\ntools:\n" << toolsStr;
            }
        }
    } catch (...) {}

    return os.str();
}
} // namespace

std::string condenseActiveModel()
{
    treenode m = model();
    if (!m) {
        bridge::consolePrint("[ModelerAI] condenseActiveModel: model() is null\n");
        return "model: (empty)";
    }

    // Sim state — always fresh. Cheap (2 SDK calls).
    std::ostringstream os;
    os << "simulation: state=" << currentRunState()
       << " time=" << currentSimTime() << "\n\n";

    // Model + Tools block — reuse cache when clean. The walk is
    // expensive (recursive tree descent + per-node path lookups); the
    // cache turns ~200-500ms into ~0.
    if (g_cacheDirty || g_cachedModelBlock.empty()) {
        bridge::consolePrint("[ModelerAI] condenseActiveModel: cache dirty, re-walking model()\n");
        g_cachedModelBlock = buildModelBlock();
        g_cacheDirty = false;
    } else {
        bridge::consolePrint("[ModelerAI] condenseActiveModel: cache hit, skipping walk\n");
    }
    os << g_cachedModelBlock;
    return os.str();
}

} // namespace ModelerAi::tree
