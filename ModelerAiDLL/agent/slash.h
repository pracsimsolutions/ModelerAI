// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/slash.h — slash command dispatcher.
//
// Detection happens in the DLL (preserves the dumb-viewer rule). When the
// bridge sees user text starting with '/', it routes here instead of the
// LLM. The dispatcher emits an assistant_chunk + assistant_done envelope
// pair so the viewer renders the response identically to a real reply.

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace ModelerAi::agent {

class Agent;

// One arg "choice" — a tabbable option offered after the command name.
// Used for commands like `/help <command>` where the second token has a
// known finite set of values.
struct ArgChoice {
    std::string value;        // what gets inserted into the input
    std::string label;        // visible label in the popover (may differ)
    std::string description;  // shown next to the label
};

// Description of a single argument the command accepts. v0.1 supports at
// most one arg per command. Future commands with multi-arg syntax can
// extend this without breaking the wire format.
struct ArgSpec {
    std::string            placeholder;  // hint, e.g. "command", "model"
    std::vector<ArgChoice> choices;      // empty -> free-text
};

// One entry in the autocomplete catalog the viewer renders when the user
// types '/'. The primary `name` is the canonical command; `aliases` are
// alternative spellings that all resolve to the same handler.
struct CommandInfo {
    std::string              name;         // primary, no leading slash
    std::vector<std::string> aliases;      // also no leading slash
    std::string              description;  // one-line shown next to the name in the popover
    std::string              details;      // longer description shown when highlighted
    std::string              usage;        // hint shown when the command is selected
    bool                     has_args = false;
    ArgSpec                  args;         // valid when has_args == true
};

// Static catalog. Stable for the process lifetime in v0.1. Future v0.2
// may extend this with user-authored entries discovered from the
// KNOWLEDGE/skills/ folder.
const std::vector<CommandInfo>& commandCatalog();

// Returns true iff the text is a slash command (starts with '/').
bool isSlashCommand(std::string_view text);

// Dispatch the slash command. Pushes assistant_chunk / assistant_done /
// state_update envelopes as needed via the agent's push channel.
// Returns true if the command was recognized; false to indicate "unknown
// command" (caller should still complete the turn with a help hint).
bool dispatchSlash(Agent* agent,
                   const std::string& turnId,
                   std::string_view text);

} // namespace ModelerAi::agent
