// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "settings/effective_settings.h"
#include "settings/global_settings.h"
#include "settings/project_settings.h"

namespace ModelerAi::settings {

nlohmann::json toJson(const EffectiveSettings& s)
{
    nlohmann::json j;
    j["theme"]                 = s.theme;
    j["send_on_enter"]         = s.send_on_enter;
    j["show_debug_info"]       = s.show_debug_info;
    j["session_strategy"]      = s.session_strategy;
    j["manual_session_key"]    = s.manual_session_key;
    j["save_as_behavior"]      = s.save_as_behavior;
    j["active_provider_id"]    = s.active_provider_id;
    j["active_model_id"]       = s.active_model_id;
    j["effort"]                = s.effort;
    j["mode"]                  = s.mode;
    nlohmann::json pols = nlohmann::json::object();
    for (const auto& kv : s.tool_policies) pols[kv.first] = kv.second;
    j["tool_policies"]            = std::move(pols);
    j["onboarding_completed_version"] = s.onboarding_completed_version;
    j["onboarding_completed_at_ms"]   = s.onboarding_completed_at_ms;
    j["cost_warn_usd"]            = s.cost_warn_usd;
    j["cost_stop_usd"]            = s.cost_stop_usd;
    j["export_dir"]               = s.export_dir;
    j["max_tool_iterations"]      = s.max_tool_iterations;
    return j;
}

void overlayFromJson(const nlohmann::json& j, EffectiveSettings& out)
{
    if (!j.is_object()) return;

    auto str = [&](const char* k, std::string& field) {
        if (j.contains(k) && j[k].is_string()) field = j[k].get<std::string>();
    };
    auto boolean = [&](const char* k, bool& field) {
        if (j.contains(k) && j[k].is_boolean()) field = j[k].get<bool>();
    };

    str("theme",              out.theme);
    boolean("send_on_enter",  out.send_on_enter);
    boolean("show_debug_info", out.show_debug_info);
    str("session_strategy",   out.session_strategy);
    str("manual_session_key", out.manual_session_key);
    str("save_as_behavior",   out.save_as_behavior);
    str("active_provider_id", out.active_provider_id);
    str("active_model_id",    out.active_model_id);
    str("effort",             out.effort);
    str("mode",               out.mode);
    str("onboarding_completed_version", out.onboarding_completed_version);
    if (j.contains("onboarding_completed_at_ms") && j["onboarding_completed_at_ms"].is_number_integer()) {
        out.onboarding_completed_at_ms = j["onboarding_completed_at_ms"].get<long long>();
    }
    if (j.contains("cost_warn_usd") && j["cost_warn_usd"].is_number()) {
        out.cost_warn_usd = j["cost_warn_usd"].get<double>();
    }
    if (j.contains("cost_stop_usd") && j["cost_stop_usd"].is_number()) {
        out.cost_stop_usd = j["cost_stop_usd"].get<double>();
    }
    str("export_dir", out.export_dir);
    if (j.contains("max_tool_iterations") && j["max_tool_iterations"].is_number_integer()) {
        int v = j["max_tool_iterations"].get<int>();
        if (v < 1)   v = 1;
        if (v > 500) v = 500;
        out.max_tool_iterations = v;
    }

    if (j.contains("tool_policies") && j["tool_policies"].is_object()) {
        // Merge — don't replace. Later layers can add policies without
        // wiping earlier ones; only direct overlaps are overwritten.
        for (auto it = j["tool_policies"].begin();
             it != j["tool_policies"].end(); ++it)
        {
            if (it.value().is_string()) {
                out.tool_policies[it.key()] = it.value().get<std::string>();
            }
        }
    }
}

EffectiveSettings resolveEffectiveSettings()
{
    EffectiveSettings out;                       // hardcoded defaults
    overlayFromJson(loadGlobalSettings(),  out); // global wins over defaults
    overlayFromJson(loadProjectSettings(), out); // project wins over global
    return out;
}

} // namespace ModelerAi::settings
