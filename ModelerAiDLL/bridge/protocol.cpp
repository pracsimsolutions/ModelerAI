// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "bridge/protocol.h"

namespace ModelerAi::bridge {

namespace {

struct NameEntry {
    BridgeMsgType    type;
    std::string_view name;
};

constexpr NameEntry kNames[] = {
    // JS -> DLL
    { BridgeMsgType::UserMessage,     "user_message"      },
    { BridgeMsgType::CancelTurn,      "cancel_turn"       },
    { BridgeMsgType::ProviderSwitch,  "provider_switch"   },
    { BridgeMsgType::SaveApiKey,      "save_api_key"      },
    { BridgeMsgType::ForgetApiKey,    "forget_api_key"    },
    { BridgeMsgType::LoadHistory,     "load_history"      },
    { BridgeMsgType::ClearHistory,    "clear_history"     },
    { BridgeMsgType::GetProviderInfo, "get_provider_info" },
    { BridgeMsgType::ViewerReady,     "viewer_ready"      },
    { BridgeMsgType::ModeChange,      "mode_change"       },
    { BridgeMsgType::TestProvider,    "test_provider"     },
    { BridgeMsgType::EffortChange,    "effort_change"     },
    { BridgeMsgType::ListSessions,       "list_sessions"     },
    { BridgeMsgType::DeleteSession,      "delete_session"    },
    { BridgeMsgType::DeleteAllSessions,  "delete_all_sessions" },
    { BridgeMsgType::AddCustomProvider,    "add_custom_provider"    },
    { BridgeMsgType::RemoveCustomProvider, "remove_custom_provider" },
    { BridgeMsgType::ToolApprovalResponse,    "tool_approval_response"    },
    { BridgeMsgType::AskUserQuestionResponse, "ask_user_question_response" },
    { BridgeMsgType::SetSetting,              "set_setting"               },
    { BridgeMsgType::RequestSettings,         "request_settings"          },
    { BridgeMsgType::CompleteOnboarding,      "complete_onboarding"       },
    { BridgeMsgType::ResetSettings,           "reset_settings"            },
    { BridgeMsgType::ExportConversation,      "export_conversation"       },
    // DLL -> JS
    { BridgeMsgType::UserMessageBroadcast, "user_message_broadcast" },
    { BridgeMsgType::AssistantChunk,  "assistant_chunk"   },
    { BridgeMsgType::AssistantDone,   "assistant_done"    },
    { BridgeMsgType::ToolCallStart,   "tool_call_start"   },
    { BridgeMsgType::ToolCallDone,    "tool_call_done"    },
    { BridgeMsgType::StateUpdate,     "state_update"      },
    { BridgeMsgType::Error,           "error"             },
    // Sentinel
    { BridgeMsgType::Unknown,         "unknown"           },
};

} // namespace

std::string_view typeName(BridgeMsgType t)
{
    for (const auto& e : kNames) {
        if (e.type == t) return e.name;
    }
    return "unknown";
}

BridgeMsgType parseType(std::string_view name)
{
    for (const auto& e : kNames) {
        if (e.name == name) return e.type;
    }
    return BridgeMsgType::Unknown;
}

} // namespace ModelerAi::bridge
