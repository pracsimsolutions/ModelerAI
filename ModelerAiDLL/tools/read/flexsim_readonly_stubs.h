// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/read/flexsim_readonly_stubs.h — read-only FlexSim tools.
//
// Two tools live here:
//   get_model_structure    — implemented; the AI's main "look at the model" surface.
//   get_object_properties  — stub (deferred, paired with set_property).
//
// Earlier stubs (get_active_model_info, list_objects, get_label_value,
// get_class_doc, eval_script_readonly) were dropped per the tool-surface
// design — folded into get_model_structure (with filters + detail levels)
// or into run_script (eval_script_readonly is redundant with the escape
// hatch).

#pragma once

#include "tools/tool_base.h"

namespace ModelerAi::tools {

// ---- get_model_structure ----
// Unified model-inspection tool. Four detail levels:
//   "summary":     counts by class (cheapest, for orienting on large models)
//   "structure":   per-object name + class + parent + position
//   "connections": structure + A/S link arrays (in/out/center)
//   "detailed":    connections + labels + size + rotation + color
// Plus filters: class_filter, name_filter (substring + wildcard), parent_path.
// Plus max_depth (default 1 = top-level only).
class GetModelStructureTool : public ITool {
public:
    std::string_view name() const override        { return "get_model_structure"; }
    std::string_view description() const override {
        return "Returns the FlexSim model's structure at a chosen detail level. "
               "Detail levels (cheapest first): 'summary' = class counts; "
               "'structure' = per-object name + class + parent + position; "
               "'connections' = structure + A/S port connections; "
               "'detailed' = connections + labels + size + rotation + color. "
               "Filter with class_filter (exact class name match), name_filter "
               "(substring match, supports * for prefix/suffix/contains), and "
               "parent_path (scope to a subtree). max_depth (default 1) "
               "controls how deep to walk under each starting node.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

// ---- get_object_properties (deferred stub) ----
// Paired with set_property — both need the picklist + property-family
// mapping work. See tool-surface.html for the deferral note.
class GetObjectPropertiesTool : public ITool {
public:
    std::string_view name() const override        { return "get_object_properties"; }
    std::string_view description() const override {
        return "Returns all readable properties of a named object: position, "
               "size, rotation, connection counts, and class-specific tunables. "
               "(Body not yet implemented — paired with set_property; deferred. "
               "For now use get_model_structure(detail: 'detailed') for breadth-"
               "level inspection, or run_script + obj.getProperty() for specific "
               "values.)";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::ReadOnly; }
    ToolResult   run(std::string_view args_json) override;
};

} // namespace ModelerAi::tools
