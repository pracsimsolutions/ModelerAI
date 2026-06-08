// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/write/run_script.h — escape-hatch FlexScript executor.
//
// Stays in the tool surface as the fallback for anything the curated
// modelerai_* command library doesn't cover. Prefer modelerai_call when a
// curated command fits.

#pragma once

#include "tools/tool_base.h"

namespace ModelerAi::tools {

class RunScriptTool : public ITool {
public:
    std::string_view name() const override        { return "run_script"; }
    std::string_view description() const override {
        return "Executes an arbitrary FlexScript snippet against the active "
               "model. The MOST POWERFUL and MOST DANGEROUS tool — only use "
               "this when no modelerai_* library command fits. Wrap multi-"
               "statement scripts in `{ ... }` and end with `return` to "
               "surface a result.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::Mutating; }
    ToolResult   run(std::string_view args_json) override;
};

} // namespace ModelerAi::tools
