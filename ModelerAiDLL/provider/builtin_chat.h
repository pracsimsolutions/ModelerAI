// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/builtin_chat.h — built-in OpenAI Chat Completions adapter for
// curated providers that ship in-box (OpenAI itself + xAI Grok).
//
// One instance per provider; holds a curated multi-model list, a fixed
// keystore id, and an env-var name to fall back to when the keystore is
// empty. Wire-format work is delegated to provider/openai_chat_helpers.

#pragma once

#include "provider/provider.h"
#include <mutex>

namespace ModelerAi::provider {

// Static configuration for one built-in OpenAI-compat entry. Filled once at
// bootstrap; immutable after construction.
struct BuiltinChatConfig {
    std::string            id;             // stable slug, e.g. "openai", "xai"
    std::string            display_name;   // visible label, e.g. "OpenAI (GPT)"
    std::string            base_url;       // full base, e.g. "https://api.openai.com/v1"
    std::string            env_var;        // e.g. "OPENAI_API_KEY"
    std::vector<ModelInfo> models;         // curated list, version-pinned
    std::string            default_model;  // initial active model id (must be in `models`)
};

class BuiltinChatCompletions : public IProvider {
public:
    explicit BuiltinChatCompletions(BuiltinChatConfig cfg);

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

    EffortLevel effort() const override;
    void set_effort(EffortLevel level) override;

private:
    BuiltinChatConfig          cfg_;
    mutable std::mutex         stateMutex_;
    std::string                active_model_id_;
    EffortLevel                effort_ = EffortLevel::Off;
};

} // namespace ModelerAi::provider
