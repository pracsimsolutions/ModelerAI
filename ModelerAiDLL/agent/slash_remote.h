// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/slash_remote.h — slash subcommand handlers for `/remote *`.

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::agent {

class Agent;

// Returns true if the args were a recognized subcommand (on/off/status/
// rotate). The handler pushes assistant_chunk + assistant_done +
// state_update envelopes via the agent's push path.
bool dispatchRemote(Agent* agent,
                    const std::string& turnId,
                    std::string_view argsAfterSlashRemote);

} // namespace ModelerAi::agent
