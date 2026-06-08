// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/openai_compatible.h — generic OpenAI Chat Completions adapter.
//
// One C++ class handles every provider that speaks the OpenAI wire format:
// OpenAI itself, DeepSeek, Mistral, Groq, Together, Fireworks, OpenRouter,
// xAI Grok, Cerebras, Ollama (local), LM Studio (local), llama.cpp server,
// LiteLLM proxy, anything else conforming to the standard.
//
// One instance == one model on one endpoint. User adds multiple instances
// via the Settings → Providers UI; bootstrap creates one C++ object per
// custom_providers.json entry and registers them all in the provider
// registry.
//
// What this adapter does NOT support yet (intentionally deferred):
// - Tool / function-call translation (no tools in v0.2).
// - Image input (multimodal varies wildly across compat providers; v0.3+).
// - Extended-thinking / reasoning_effort (Anthropic-only knob for now).

#pragma once

#include "provider/provider.h"
#include <mutex>

namespace ModelerAi::provider {

// Persistent config for one custom provider entry. Lives in
// custom_providers.json alongside others; one OpenAiCompatible
// instance is created per record.
struct CustomProviderConfig {
    std::string id;             // stable slug used in provider_switch envelopes (auto-generated if missing)
    std::string display_name;   // visible label, e.g. "Local Ollama (Llama 3.2)"
    std::string base_url;       // includes /v1, e.g. "http://localhost:11434/v1"
    std::string api_key;        // pass-through bearer token; pre-shipping plaintext (DPAPI keystore lands later)
    std::string model_id;       // the wire-format model identifier
    std::string model_display;  // friendly model label; falls back to model_id

    int    context_tokens   = 32000;
    bool   supports_tools   = false;
    bool   supports_vision  = false;

    // Per-Mtok USD pricing for cost telemetry. User-supplied; default zeros
    // mean "no cost shown" (correct for local Ollama / LM Studio).
    double input_usd_per_mtok        = 0.0;
    double output_usd_per_mtok       = 0.0;
    double cache_write_usd_per_mtok  = 0.0;
    double cache_read_usd_per_mtok   = 0.0;
};

class OpenAiCompatible : public IProvider {
public:
    explicit OpenAiCompatible(CustomProviderConfig cfg);

    std::string id() const override;
    std::string display_name() const override;
    std::vector<ModelInfo> models() const override;
    bool has_api_key() const override;

    void stream_turn(
        const agent::Conversation&         conv,
        const std::vector<agent::ToolDef>& tools,
        StreamCallback                     onChunk,
        ToolCallCallback                   onToolCall,
        DoneCallback                       onDone,
        ErrorCallback                      onError,
        CancelToken                        cancel = nullptr) override;

    TestKeyResult test_key() override;

    std::string active_model_id() const override;
    bool set_active_model(std::string_view id) override;

    EffortLevel effort() const override;           // always Off — not supported
    void set_effort(EffortLevel level) override;   // no-op

    const CustomProviderConfig& config() const { return cfg_; }

private:
    mutable std::mutex stateMutex_;
    CustomProviderConfig cfg_;
};

} // namespace ModelerAi::provider
