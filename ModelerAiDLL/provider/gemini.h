// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// provider/gemini.h — Google Gemini streamGenerateContent adapter.
//
// Speaks Google's own wire format (NOT OpenAI-compat) so we can surface
// Gemini's thinkingBudget knob and its 2M-token context window. Curated
// model list ships in models(); user picks via the unified model picker.

#pragma once

#include "provider/provider.h"
#include <mutex>

namespace ModelerAi::provider {

class Gemini : public IProvider {
public:
    Gemini();

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
    mutable std::mutex stateMutex_;
    std::string        active_model_id_;
    EffortLevel        effort_ = EffortLevel::Off;
};

} // namespace ModelerAi::provider
