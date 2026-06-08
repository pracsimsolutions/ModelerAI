// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// agent/user_rules.h — load the user-authored rules markdown file.
//
// The user maintains durable behavioral instructions at
// %APPDATA%/PRACSIM/ModelerAI/rules.md. We read it on every turn and inject
// the contents into the system prompt between the rules block and the KB
// block. Edits take effect on the next message — no restart, no reload
// button.
//
// If the file doesn't exist on first read, we create it from a template so
// the user has something to edit.

#pragma once

#include <string>

namespace ModelerAi::agent {

// Returns the file's contents (after creating from template if missing),
// trimmed. Returns empty string on hard I/O failure.
std::string loadUserRules();

// Absolute path to the rules file. Useful for the Settings UI's
// "Open my rules file" button.
std::string userRulesPath();

} // namespace ModelerAi::agent
