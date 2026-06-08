// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// paths.h — OS path resolution for ModelerAI runtime files.
//
// Everything ModelerAI writes to disk lives under %APPDATA%/PRACSIM/ModelerAI/.
// The directory is created on first call to appDataDir() and cached for the
// process lifetime.

#pragma once

#include <string>

namespace ModelerAi::paths {

// %APPDATA%/PRACSIM/ModelerAI/. Always ends in a backslash. Both PRACSIM
// and ModelerAI subdirs are created if missing (idempotent — already-exists
// is not an error). Returns empty string if SHGetKnownFolderPath fails.
std::string appDataDir();

// appDataDir() + "bridge.log". The MODELER_AI_BRIDGE_LOG=1 trace log.
std::string bridgeLogPath();

// appDataDir() + "diag.log". Always-on diagnostic log for the remote
// subsystem (thread-safe; survives worker-thread context where pt()
// is unreliable). Tail with `Get-Content -Wait`.
std::string diagLogPath();

// Directory containing ModelerAI.dll. Used by the KB loader to find
// KNOWLEDGE/topics/*.md relative to the installed module. Always ends
// in a backslash.
std::string moduleInstallDir();

// appDataDir() + "custom_providers.json". User-configured custom
// OpenAI-compatible providers persist here.
std::string customProvidersPath();

} // namespace ModelerAi::paths
