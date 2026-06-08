// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/read/ask_user_question.h
//
// Framework tool — lets the LLM pause and ask the user a multiple-choice
// (or free-text) question mid-turn. Modeled on the AskUserQuestion tool
// in Claude Code's superpowers skill catalog, which the user singled out
// as a workflow they want here.
//
// The dispatch layer special-cases this tool by name (see agent.cpp's
// runAskUserQuestion) because it doesn't fit the synchronous ITool::run
// contract — it needs to push a UI envelope and block on user input via
// the InteractionBroker. The class still implements ITool so the registry
// + tools[] catalog include it in the provider request; the run() method
// is technically unreachable from the normal path.

#pragma once

#include "tools/tool_base.h"

namespace ModelerAi::tools {

class AskUserQuestionTool : public ITool {
public:
    std::string_view name() const override        { return "ask_user_question"; }
    std::string_view description() const override {
        return "Pause and ask the user a question mid-conversation. Use this when "
               "you need user input to proceed (clarification, picking between "
               "options, confirming an assumption). The user's answer is returned "
               "as the tool result. Prefer this over guessing or assuming.";
    }
    std::string  params_schema() const override;
    Category     category() const override         { return Category::ReadOnly; }

    // Unreachable in normal dispatch — runAskUserQuestion in agent.cpp
    // handles this tool. If the registry ever resolves it, fall back to
    // a synthetic "not supported via this path" error so we don't crash.
    ToolResult   run(std::string_view args_json) override;
};

} // namespace ModelerAi::tools
