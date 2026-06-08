// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// settings/global_settings.h — user-level settings persistence.
//
// One JSON file at %APPDATA%\PRACSIM\ModelerAI\settings.json. Plaintext
// (not encrypted) — these are preferences, not secrets. Atomic write
// via MoveFileExA REPLACE_EXISTING.

#pragma once

#include "third_party/json.h"
#include <string>

namespace ModelerAi::settings {

// Returns the JSON shape on disk. If the file is missing, returns an
// empty object (the resolver will fall through to defaults). Parse
// failures also return empty + log a warning — never throw.
nlohmann::json loadGlobalSettings();

// Atomic write. Returns true on success.
bool saveGlobalSettings(const nlohmann::json& j);

// Convenience: merge `patch` (a JSON object containing ONLY the fields
// the caller wants to update) into the existing global settings and save.
// Fields not in `patch` are preserved.
bool patchGlobalSettings(const nlohmann::json& patch);

} // namespace ModelerAi::settings
