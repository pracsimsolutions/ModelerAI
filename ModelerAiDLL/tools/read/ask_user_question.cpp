// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.

#include "tools/read/ask_user_question.h"

namespace ModelerAi::tools {

std::string AskUserQuestionTool::params_schema() const
{
    // Two-arg schema: a question string, plus an options array.
    // Options can be plain strings ("Yes", "No") or {value, label,
    // description} objects for richer rendering. Both shapes flow
    // through verbatim to the viewer's question bubble; the LLM
    // gets back {answer: <chosen value>}.
    return R"({
  "type": "object",
  "properties": {
    "question": {
      "type": "string",
      "description": "The question to ask the user. Be concise and specific."
    },
    "options": {
      "type": "array",
      "description": "Multiple-choice options. Each is either a plain string OR an object {value, label, description}. Omit / leave empty to ask for free-text input.",
      "items": {
        "oneOf": [
          { "type": "string" },
          {
            "type": "object",
            "properties": {
              "value":       { "type": "string" },
              "label":       { "type": "string" },
              "description": { "type": "string" }
            },
            "required": ["value"]
          }
        ]
      }
    }
  },
  "required": ["question"]
})";
}

ToolResult AskUserQuestionTool::run(std::string_view /*args_json*/)
{
    // The dispatch layer special-cases ask_user_question and never calls
    // this. If we land here, something routed around the special case;
    // surface that as a tool error rather than crashing.
    ToolResult r;
    r.ok            = false;
    r.error_code    = "internal_routing_error";
    r.error_message = "ask_user_question must be dispatched via the "
                      "InteractionBroker path, not the synchronous ITool::run path. "
                      "This is a framework bug — check agent.cpp.";
    return r;
}

} // namespace ModelerAi::tools
