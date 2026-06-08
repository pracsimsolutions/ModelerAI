// Copyright (c) 2026 Practical Simulation Solutions.
// Licensed under the MIT License — see LICENSE.txt at the repo root.
//
// tools/tool_base.h — ITool interface + ToolResult type.
//
// Every tool that the model can call implements this interface. The agent's
// tool_registry maps tool names to ITool* instances. Each tool exposes its
// schema (so the provider can advertise it to the model) and a synchronous
// run(args) → result. Long-running tools should still return synchronously
// for v1; we'll revisit async tools when a use case forces the issue.

#pragma once

#include <string>
#include <string_view>

namespace ModelerAi::tools {

// ---------------------------------------------------------------------------
// Category — matches the design doc's A/B/C/D/E split. Drives confirmation
// prompts, mode-based filtering (Ask hides Mutating), and per-tool policy
// defaults. AI-made changes are NOT integrated with FlexSim's native undo
// stack — see docs/architecture/2026-05-24-undo-research.md for the
// snapshot-based rewind plan that replaces it.
// ---------------------------------------------------------------------------
enum class Category {
    ReadOnly,       // A — no confirmation, auto-execute
    Mutating,       // B — Plan intercepts; Apply runs through per-tool approval modal
    Filesystem,     // C — writes to disk, surface intent before running
    Lifecycle,      // D — model open/save/close, modal confirmation required
    RuntimeReadOnly // E — runtime / output introspection
};

// ---------------------------------------------------------------------------
// What every tool returns. ok == false is still a result — the model needs
// to see and reason about errors, so they are NOT exceptions.
// ---------------------------------------------------------------------------
struct ToolResult {
    bool        ok = true;
    std::string result_json;  // serialized response body; empty on bare-success
    std::string error_message; // user/model-visible message when ok == false
    std::string error_code;    // machine-tag, e.g. "object_not_found"
};

// ---------------------------------------------------------------------------
// The interface every tool implements.
// ---------------------------------------------------------------------------
class ITool {
public:
    virtual ~ITool() = default;

    /** Unique name. Used in the provider's tool-call wire format. */
    virtual std::string_view name() const = 0;

    /** Human-readable description. The model reads this to decide whether
     *  to call the tool, so be precise about scope, side effects, and
     *  preconditions. */
    virtual std::string_view description() const = 0;

    /** JSON Schema (Draft-07 subset) describing the tool's parameters.
     *  Returned as a string so adapters can embed it without re-parsing. */
    virtual std::string params_schema() const = 0;

    /** Category — drives undo envelope + confirmation behavior. */
    virtual Category category() const = 0;

    /** Run the tool. args_json is the raw JSON the model passed.
     *  Implementation parses, validates against its own schema, runs, and
     *  returns a ToolResult. Throws are caught by the agent and reported as
     *  results with ok == false. */
    virtual ToolResult run(std::string_view args_json) = 0;
};

// Shared "this tool's body is registered but not implemented yet" reply.
// Used by all FlexSim-domain stub tools so the agent loop keeps going and
// the model can adjust its plan instead of treating the call as an error.
inline ToolResult makeNotImplementedReply(std::string_view toolName)
{
    ToolResult r;
    r.ok = true;
    r.result_json = std::string("{\"not_implemented\":true,\"tool\":\"")
                  + std::string(toolName)
                  + "\",\"note\":\"This tool's framework is registered but "
                    "its body is not implemented yet. The user owns the "
                    "FlexSim domain bodies; they'll fill this in when "
                    "authoring the tool surface.\"}";
    return r;
}

} // namespace ModelerAi::tools
