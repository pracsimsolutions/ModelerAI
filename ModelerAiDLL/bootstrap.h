// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// bootstrap.h — DLL-init wiring + provider registry.
//
// Holds:
//   - The single Agent instance.
//   - The provider registry: native Anthropic + zero-or-more user-configured
//     OpenAI-compatible custom providers loaded from custom_providers.json.
//   - The currently-active provider pointer (the one the agent uses).

#pragma once

#include "provider/openai_compatible.h"

#include <string>
#include <string_view>
#include <vector>

namespace ModelerAi::agent {
    class Agent;
}
namespace ModelerAi::provider {
    class IProvider;
}
namespace ModelerAi::keystore {
    class Keystore;
}
namespace ModelerAi::tools {
    class ToolRegistry;
}
namespace ModelerAi::kb {
    class KbIndex;
}

namespace ModelerAi::bootstrap {

// The process-wide DPAPI keystore. Loaded once at initialize(); all
// adapters resolve API keys through it before falling back to env var
// (Anthropic) or in-config plaintext (custom providers, for one-time
// migration from older custom_providers.json files).
keystore::Keystore* keystore();

// The process-wide tool registry. Populated at initialize() with the
// built-in test tool (echo) and any future tools the user wires in.
tools::ToolRegistry* toolRegistry();

// The process-wide knowledge-base index. Loaded once at initialize() from
// KNOWLEDGE/INDEX.json. Returns a valid (possibly empty) instance even if
// the JSON file is missing — caller should always check loaded() / size().
kb::KbIndex* kbIndex();

void initialize();
void teardown();

// The single Agent for this DLL instance.
agent::Agent* agent();

// All registered providers (Anthropic first, then custom in insertion order).
std::vector<provider::IProvider*> providers();

// The provider whose model is currently being used for chat.
provider::IProvider* activeProvider();

// Locate a provider by its id() string. Returns nullptr if none matches.
provider::IProvider* findProvider(std::string_view id);

// Locate the provider that hosts the given model id. Used by provider_switch
// to identify which adapter owns a model the user picked from the unified
// picker. Returns nullptr if no registered provider serves that model.
provider::IProvider* findProviderForModel(std::string_view modelId);

// Make `p` the active provider and update the agent's pointer. No-op if p
// isn't in the registry.
bool setActiveProvider(provider::IProvider* p);

// Add a new user-configured custom provider. Persists to disk and returns
// the new instance (still owned by the registry). Returns nullptr on
// validation failure (e.g. duplicate id, missing base_url).
provider::IProvider* addCustomProvider(provider::CustomProviderConfig cfg);

// Remove a custom provider by id. Built-in Anthropic cannot be removed.
// If the removed provider was active, falls back to the first remaining.
// Persists to disk.
bool removeCustomProvider(std::string_view id);

} // namespace ModelerAi::bootstrap
