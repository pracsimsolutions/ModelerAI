// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// settings/project_settings.h — per-.fsm settings stored in the model.
//
// Read/written from the in-model ModelerAI object's
// `variables/projectSettings` byteblock node. Travels with the .fsm
// when the user saves; Save-As preserves it; load brings it back.
//
// IMPORTANT — both calls touch the FlexSim SDK tree, so they MUST run
// on the FlexSim main thread. Worker threads should snapshot the
// resolved EffectiveSettings before dispatching.
//
// Returned shape is partial — only the fields the user has explicitly
// set at project scope appear in the JSON. Missing fields fall through
// to global (or hardcoded defaults). Empty model / missing in-model
// object returns an empty object (not an error).

#pragma once

#include "third_party/json.h"

namespace ModelerAi::settings {

// Read the projectSettings byteblock + parse as JSON. Always returns
// a JSON object (possibly empty); never throws.
nlohmann::json loadProjectSettings();

// Replace the projectSettings byteblock with a JSON serialization of
// `j`. Returns false if the in-model node isn't reachable (e.g. user
// hasn't run addmodelerai yet, or no model is loaded).
bool saveProjectSettings(const nlohmann::json& j);

// Merge `patch` into the existing project settings + save. Same
// pattern as patchGlobalSettings.
bool patchProjectSettings(const nlohmann::json& patch);

// True if the in-model ModelerAI object exists for the current
// model. When false, project settings can't be saved — the UI
// should grey out the "This project" scope toggle.
bool projectStorageAvailable();

} // namespace ModelerAi::settings
