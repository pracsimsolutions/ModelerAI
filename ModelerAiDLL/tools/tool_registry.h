// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/tool_registry.h — name → ITool* registry, populated at DLL init.
//
// One ToolRegistry per DLL instance, owned by bootstrap. The agent reads
// from it on every turn to build the list of ToolDefs sent to the provider,
// and to look up an implementation when a provider emits a tool_use call.
//
// Per the project memory feedback-user-owns-kb-and-tool-content, the
// registry is wiring only. The TOOLS themselves (the C++ classes that
// implement ITool::run with FlexSim SDK calls) land later when the user
// is hands-on to provide the domain-specific implementations. For v0.3
// only one trivial test tool ships — `echo` — so the framework is
// exercisable without touching FlexSim.

#pragma once

#include "agent/message.h"   // for ToolDef
#include "tools/tool_base.h"

#include <memory>
#include <mutex>
#include <string>
#include <string_view>
#include <vector>

namespace ModelerAi::tools {

class ToolRegistry {
public:
    // Add `t` to the registry. Takes ownership. If a tool with the same
    // name already exists, the new one replaces it.
    void registerTool(std::unique_ptr<ITool> t);

    // Look up by canonical name. Returns nullptr if not registered.
    ITool* find(std::string_view name) const;

    // Snapshot of every registered tool as ToolDefs the provider adapter
    // can serialize. Built fresh each call (cheap for a handful of tools).
    std::vector<agent::ToolDef> toolDefs() const;

    // How many tools are registered. Mostly for /tools and debug.
    int size() const;

private:
    mutable std::mutex                   mutex_;
    std::vector<std::unique_ptr<ITool>>  tools_;
};

} // namespace ModelerAi::tools
