/**Custom Code*/
// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// commands/antipattern.h — runtime detector for known-bad FlexScript forms
// the AI keeps hallucinating despite prompt directives. Header-only.
//
// Used by:
//   tools/write/run_script.cpp           — checks arbitrary FlexScript bodies
//   commands/commands.cpp                — checks PM expression bodies
//
// Rationale: text directives in the system prompt advise the model toward
// the modern API but compete with training-data patterns for attention.
// As conversations grow and the prompt absorbs more rules, the prompt's
// pull weakens and the model reverts. A runtime check at the tool boundary
// is deterministic — the AI can't bypass it, and the structured error
// gives the model an immediate, contextual correction that one
// round-trip resolves.
//
// To add a new pattern: append to kAntiPatterns. Each entry is a regex +
// a remediation message naming both the deprecated form and the modern
// alternative.

#pragma once

#include <regex>
#include <string>

namespace ModelerAi {

struct AntiPattern {
    const char* name;          // human-readable label, used as the antipattern field in the JSON response
    const char* regex;         // ECMAScript-syntax regex
    const char* remediation;   // error_message text — names WHAT is wrong AND WHAT to use instead
};

inline const AntiPattern* getAntiPatterns(int& outCount)
{
    static const AntiPattern kPatterns[] = {
        {
            "createObject",
            R"(\bcreateObject\s*\()",
            "Your script uses `createObject(...)` which doesn't exist in FlexSim 2026 — "
            "your training data is showing. Use the curated tool `modelerai_create_object` "
            "(preferred — handles position, parent, validation), or `Object.create(\"ClassName\")` "
            "for raw FlexScript."
        },
        {
            "createInstance/instantiate/makeObject/newObject",
            R"(\b(createInstance|instantiate|makeObject|newObject|spawnObject|createNewObject)\s*\()",
            "`createInstance`, `instantiate`, `makeObject`, `newObject`, `spawnObject`, "
            "`createNewObject` — NONE of these exist in FlexSim 2026. They are training-data "
            "hallucinations. The ACTUAL APIs for creating things are: "
            "(1) `modelerai_create_object(\"ClassName\", ...)` — curated, validated, preferred. "
            "(2) `Object.create(\"ClassName\")` — raw FlexScript for visible 3D objects (Source, "
            "Queue, Processor, etc.). (3) `Tools.create(\"ToolType\", \"name\")` — for Tools "
            "(Group, GlobalTable, StatisticsCollector, etc.). Before retrying, call `search_kb` "
            "with the class or Tool name you want to create so you see the real signature."
        },
        {
            "connectPorts/connectobjects",
            R"(\b(connectPorts|connectobjects)\s*\()",
            "Your script uses `connectPorts(...)` or `connectobjects(...)` — neither exists "
            "in FlexSim 2026. Use the curated tool `modelerai_create_port_connection` "
            "(preferred — verifies the connection actually took effect), or "
            "`contextdragconnection(src, dst, 'A')` for raw FlexScript."
        },
        {
            "setloc/xloc family",
            R"(\b(setloc|xloc|yloc|zloc)\s*\()",
            "The `setloc/xloc/yloc/zloc` free-function form is legacy. Use the modern "
            "accessor: `obj.setLocation(x,y,z)` to write, `obj.location.x` (or .y/.z) to read."
        },
        {
            "setsize/xsize family",
            R"(\b(setsize|xsize|ysize|zsize)\s*\()",
            "The `setsize/xsize/...` free-function form is legacy. Use `obj.setSize(x,y,z)` "
            "or `obj.size.x` (.y/.z)."
        },
        {
            "setrot/xrot family",
            R"(\b(setrot|xrot|yrot|zrot)\s*\()",
            "The `setrot/xrot/...` free-function form is legacy. Use `obj.setRotation(x,y,z)` "
            "or `obj.rotation.x` (.y/.z)."
        },
        {
            "getname/setname",
            R"(\b(getname|setname)\s*\()",
            "`getname()` and `setname()` are legacy. Use `obj.name` (read or write — it's a property)."
        },
        {
            "destroyobject",
            R"(\bdestroyobject\s*\()",
            "`destroyobject(obj)` is legacy. Use `obj.destroy()` or `treenode.destroy()` "
            "(the curated `modelerai_delete_object` is preferred for top-level deletes)."
        },
        // NOTE (.1000086): the getoutput/getinput antipattern was REMOVED. Those
        // are valid fsvisible functions that work on a TREENODE (e.g. the result
        // of Model.find()), whereas the "modern" `obj.stats.input.value` accessor
        // requires an Object — on a Model.find() treenode it raises a compile
        // error ("ObjectPorts does not support property value"). The old rule
        // rejected getinput/getoutput and steered the agent straight into that
        // error when authoring PM/expression bodies. getinput(Model.find("X"))
        // is the correct, cast-free form; leave it allowed.
        {
            "while-time-step loop",
            // Match `while (... time() ...)` whose body (within the same statement-ish window)
            // contains step(). The [\s\S]{0,200} cap keeps the search bounded.
            R"(while\s*\([^)]*\btime\s*\([^)]*\)[^)]*\)[\s\S]{0,200}?\bstep\s*\()",
            "The `while (time() < N) step();` pattern overshoots by exactly one event "
            "(step() is atomic — it can jump the clock far past the target). Use the curated "
            "tool `modelerai_run_to_time(target)` (preferred — adds a temp stop time and waits "
            "for the stop trigger, halts EXACTLY at target) or `runtotime(target)` for raw "
            "FlexScript (blocks until sim time hits target)."
        },
        {
            "infinite while loop",
            // Match `while (1)` / `while (true)` / `while ( true )`. These
            // unambiguously infinite forms freeze FlexSim hard — there's no
            // try/catch and no engine-side script timeout. If the loop body
            // doesn't explicitly `break` or `return`, the engine deadlocks
            // until the modeler force-kills the process.
            R"(\bwhile\s*\(\s*(1|true)\s*\))",
            "FlexScript has no try/catch and no engine-side timeout: an "
            "infinite `while (1)` / `while (true)` loop freezes FlexSim — "
            "the modeler has to kill the process. Use a bounded for-loop "
            "with a sanity cap instead, e.g. "
            "`for (int i = 0; i < 10000 && condition; i++) { ... }`. "
            "If you truly need to iterate until a destructive condition (e.g. "
            "clearing subnodes), cap at 4096: "
            "`for (int i = 0; i < 4096 && x.subnodes.length; i++) x.subnodes[1].destroy();`"
        },
        {
            "inobject/outobject/centerobject",
            R"(\b(inobject|outobject|centerobject)\s*\()",
            "The free-function port accessors `inobject/outobject/centerobject` are legacy. "
            "Use the modern array accessors: `obj.inObjects[i]`, `obj.outObjects[i]`, "
            "`obj.centerObjects[i]`. `nrop(obj)` and `nrcp(obj)` for counts also have "
            "accessor forms: `obj.outObjects.length`, `obj.centerObjects.length`."
        },
    };
    outCount = static_cast<int>(sizeof(kPatterns) / sizeof(kPatterns[0]));
    return kPatterns;
}

// Scans `body` for a known antipattern. On match, fills outName +
// outRemediation and returns true. Returns false if no antipattern hits.
// Patterns are checked in declaration order — first match wins.
inline bool scanAntiPatterns(const std::string& body,
                             std::string& outName,
                             std::string& outRemediation)
{
    int count = 0;
    const AntiPattern* patterns = getAntiPatterns(count);
    for (int i = 0; i < count; ++i) {
        try {
            std::regex re(patterns[i].regex, std::regex::ECMAScript);
            if (std::regex_search(body, re)) {
                outName        = patterns[i].name;
                outRemediation = patterns[i].remediation;
                return true;
            }
        } catch (...) {
            // Malformed regex — skip, don't crash the whole scan.
        }
    }
    return false;
}

} // namespace ModelerAi
