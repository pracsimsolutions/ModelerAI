// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// settings/effective_settings.h — resolved per-turn settings.
//
// Three sources, applied in this order (later wins):
//   1. Hardcoded defaults (this struct's field initializers).
//   2. Global file: %APPDATA%\PRACSIM\ModelerAI\settings.json.
//   3. Project node: in-model ModelerAI/variables/projectSettings.
//
// VS-Code-style precedence: "this project is special, do it this way"
// overrides "my usual defaults." The viewer's Settings UI decides which
// scope each field can be written at.

#pragma once

#include "third_party/json.h"

#include <string>
#include <unordered_map>

namespace ModelerAi::settings {

struct EffectiveSettings {
    // ---- General (user-level by convention) ----
    std::string theme               = "system";      // light / dark / system
    bool        send_on_enter       = true;
    bool        show_debug_info     = false;

    // ---- Session scope (user-level by convention) ----
    std::string session_strategy    = "per-file";    // per-file / per-folder / manual
    std::string manual_session_key;                  // only used when strategy == "manual"

    // ---- Save-As branching behavior (user-level by convention) ----
    // What to do when the active model changes to a different .fsm dir
    // but the in-model session_id matches one we've already seen at the
    // old location (i.e., the user just did a Save-As):
    //   "copy"  — duplicate the conversation under a new GUID on the
    //             new model; the two diverge from here (DEFAULT — best
    //             for the "scenario branching" workflow)
    //   "fresh" — new model starts with an empty chat
    //   "share" — legacy: both files keep writing to the same on-disk
    //             session file (cross-contamination)
    std::string save_as_behavior    = "copy";

    // ---- Model selection (user-level default + project override) ----
    // Default to the cheapest model (Haiku). Users with bigger budgets
    // can swap mid-session via the model picker in the chat header.
    std::string active_provider_id  = "anthropic";
    std::string active_model_id     = "claude-haiku-4-5";
    std::string effort              = "off";         // off / low / medium / high

    // ---- Per-project by convention ----
    std::string mode                = "ask";         // ask / plan / apply

    // Per-tool runtime policy: tool name -> ask / auto_allow / always_deny.
    // Sparse — only tools the user has explicitly decided on appear here;
    // missing entries default to "ask".
    std::unordered_map<std::string, std::string> tool_policies;

    // Most-recently-completed onboarding wizard version. Empty / older
    // than kCurrentOnboardingVersion -> wizard auto-launches on next
    // viewer load. Acts as a "first-run guide" suggestion (not a gate);
    // ModelerAI is MIT-licensed and has no legal-acceptance wall.
    std::string onboarding_completed_version;
    long long   onboarding_completed_at_ms = 0;

    // ---- Cost ceiling (user-level by convention) ----
    // Soft warning + hard stop on cumulative session cost. Zero means
    // "disabled" for that threshold. Checked in Agent::startTurn before
    // dispatching the worker — exceeding the stop threshold rejects the
    // turn with a clear error envelope; exceeding the warn threshold
    // pushes a state_update the viewer renders as an inline banner but
    // still lets the turn proceed.
    double cost_warn_usd            = 0.0;
    double cost_stop_usd            = 0.0;

    // ---- Export (user-level by convention) ----
    // Folder /export writes conversation markdown files to. Empty falls
    // back to %APPDATA%\PRACSIM\ModelerAI\exports\. Trailing slash is
    // tolerated either way; the slash command normalizes.
    std::string export_dir;

    // ---- Agent loop cap (user-level by convention) ----
    // Maximum number of LLM ↔ tool ping-pong rounds within a single turn.
    // Each round = one Anthropic API call: model emits tool calls, we
    // run them, send results back, model continues. Hitting this cap
    // aborts the turn with a structured error that tells the user to
    // raise the limit here in settings. Default 25; range [1, 500].
    int         max_tool_iterations = 25;
};

// Version of the first-run onboarding wizard. Bump when the wizard
// gains a new step the user must walk (e.g. enabling tools, importing
// KB, etc.). Format: YYYY-MM-DD.
constexpr const char* kCurrentOnboardingVersion = "2026-05-24";

// Serialize the FULL settings struct to JSON. Used for the global
// settings file (which is always complete).
nlohmann::json toJson(const EffectiveSettings& s);

// Overlay the fields PRESENT in `j` onto `out`. Missing fields are NOT
// touched (so the project layer can be partial — only the overrides
// the user has explicitly set at project scope appear in the JSON).
// Tool policies merge key-by-key.
void overlayFromJson(const nlohmann::json& j, EffectiveSettings& out);

// Resolve: defaults → global → project, returning the effective view.
// Must be called from the FlexSim main thread (project layer touches
// the SDK). Worker threads should snapshot this once and read their
// captured copy.
EffectiveSettings resolveEffectiveSettings();

} // namespace ModelerAi::settings
