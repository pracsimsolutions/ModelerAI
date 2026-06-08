// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/prompt.h — system-prompt assembly.
//
// All persona / rules text lives in compiled C++ (see prompt.cpp). The
// agent calls this every turn with a fresh condensed-model snapshot, the
// KB block, and the user-authored rules. The output is split into two
// pieces — a STABLE prefix (suitable for Anthropic prompt caching) and a
// VARIABLE suffix (the current model state, which changes per turn).

#pragma once

#include "agent/agent.h"   // for AgentMode

#include <string>
#include <string_view>

namespace ModelerAi::agent {

struct SystemPromptParts {
    std::string stable_prefix;   // persona + rules + user_rules + KB
    std::string variable_suffix; // condensed model state + mode hint
};

// Inputs to system-prompt assembly. All optional — empty strings produce
// the corresponding section being omitted. catalogIndex is the compact
// always-on FlexScript symbol surface emitted by tools/build-kb.js (~1.5K
// tokens, lists every class by module + every command group with samples).
// Full per-group / per-module tables are NOT in the prefix — the LLM
// fetches them on demand via list_commands / list_classes tools.
struct SystemPromptInputs {
    std::string_view condensedModel;
    std::string_view kbBlock;           // hand-written topic body (legacy "example-greeting")
    std::string_view catalogIndex;      // KNOWLEDGE/catalog/index.md content
    std::string_view userRules;
    AgentMode        mode;
};

// Build the prompt as two parts. Provider adapters that support prompt
// caching (e.g. Anthropic) emit them as a two-block array with cache_control
// on the prefix. Adapters that don't can just concatenate.
//
// The mode hint lives in the VARIABLE suffix on purpose: switching mode
// shouldn't invalidate the (much larger) persona+rules+KB cache. The
// mode hint is small (~50-100 tokens) so paying for it on every turn is
// cheap vs the savings of keeping the stable prefix cached across mode
// flips.
SystemPromptParts buildSystemPrompt(const SystemPromptInputs& in);

// Convenience: returns prefix + "\n\n" + suffix. Kept for any caller that
// still wants a single string.
std::string assembleSystemPrompt(const SystemPromptInputs& in);

} // namespace ModelerAi::agent
