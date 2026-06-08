// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "tools/tool_registry.h"

namespace ModelerAi::tools {

void ToolRegistry::registerTool(std::unique_ptr<ITool> t)
{
    if (!t) return;
    std::lock_guard<std::mutex> lk(mutex_);
    std::string targetName(t->name());
    // Replace any existing entry with the same name (last-write-wins so
    // tests / hot-reloads of a tool can override an earlier registration).
    for (auto& existing : tools_) {
        if (existing && existing->name() == targetName) {
            existing = std::move(t);
            return;
        }
    }
    tools_.push_back(std::move(t));
}

ITool* ToolRegistry::find(std::string_view name) const
{
    std::lock_guard<std::mutex> lk(mutex_);
    for (const auto& t : tools_) {
        if (t && t->name() == name) return t.get();
    }
    return nullptr;
}

std::vector<agent::ToolDef> ToolRegistry::toolDefs() const
{
    std::lock_guard<std::mutex> lk(mutex_);
    std::vector<agent::ToolDef> out;
    out.reserve(tools_.size());
    for (const auto& t : tools_) {
        if (!t) continue;
        agent::ToolDef d;
        d.name               = t->name();
        d.description        = t->description();
        d.params_schema_json = t->params_schema();
        out.push_back(std::move(d));
    }
    return out;
}

int ToolRegistry::size() const
{
    std::lock_guard<std::mutex> lk(mutex_);
    return (int)tools_.size();
}

} // namespace ModelerAi::tools
