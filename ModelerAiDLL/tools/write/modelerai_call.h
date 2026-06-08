// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/write/modelerai_call.h — single bridge tool to the modelerai_*
// command library. The library itself lives as C++ functions exported from
// the DLL and bound to FlexScript names via .fsx tree nodes (see
// ModelerAI.fsx around the modelerAiBridgeSend node). This tool turns an
// AI tool-call into an applicationcommand dispatch.
//
// Plan mode reads the inner `name` arg and reports "would have called <name>
// with <args>" so the per-command intent surfaces, instead of the opaque
// shape that run_script produces. Apply mode runs the call through the
// per-tool approval modal — the modal renders the inner name + args.

#pragma once

#include "tools/tool_base.h"

namespace ModelerAi::tools {

class ModelerAiCallTool : public ITool {
public:
    std::string_view name() const override        { return "modelerai_call"; }
    std::string_view description() const override {
        return "Invokes a ModelerAI library command. The library provides "
               "curated, tested FlexSim operations — prefer modelerai_call "
               "over run_script for any operation listed in the always-on "
               "modelerai_commands catalog. Each command has its own topic "
               "(read_topic('<command_name>')) with the parameter contract.";
    }
    std::string  params_schema() const override;
    Category     category() const override        { return Category::Mutating; }
    ToolResult   run(std::string_view args_json) override;
};

} // namespace ModelerAi::tools
