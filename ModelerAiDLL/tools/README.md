# tools/ — Tool implementations

Tools are how the model interacts with FlexSim. Each tool has a name, a description (shown to the model so it can pick the right one), a JSON-schema for its parameters, and a `run(args) → ToolResult` implementation.

> **AI changes do not integrate with FlexSim's native undo stack.** See [`docs/architecture/2026-05-24-undo-research.md`](../../docs/architecture/2026-05-24-undo-research.md) for the snapshot-based rewind plan that replaces aggregated undo. Tool bodies just call FlexSim SDK functions directly; the rewind/cleanup layer lives above them.

Categories follow the design doc (`../docs/spec/README.md`):

| Folder | Category | Properties |
|---|---|---|
| `read/` | A | Auto-execute, no confirmation. Read-only state inspection. |
| `write/` | B | Plan mode intercepts as "would-do"; Apply runs through per-tool approval modal. |
| `filesystem/` | C | Writes to disk. Surface intent before running. |
| `lifecycle/` | D | Model open/save/close. Modal confirmation required. |
| `runtime/` | E | Runtime / output introspection. Read-only against simulation results. |

## Current files

- `tool_base.h` — `ITool` interface + `ToolResult` + `makeNotImplementedReply` helper
- `tool_registry.{cpp,h}` — name → ITool* registry, populated at DLL init
- `read/echo_tool.{cpp,h}` — framework-exercise tool
- `read/ask_user_question.{cpp,h}` — pause-and-wait primitive backed by the InteractionBroker
- `read/flexsim_readonly_stubs.{cpp,h}` — 6 read-only FlexSim tool stubs (schemas + dispatch; bodies stubbed)
- `write/flexsim_mutating_stubs.{cpp,h}` — 6 mutating FlexSim tool stubs (same pattern)

## Adding a new tool

1. Pick the right category folder.
2. New `.cpp/.h` pair that implements `ITool`. Set `name()`, `description()`, `params_schema()`, `category()`, and `run()`.
3. Register the tool in `bootstrap.cpp` near the other `registerTool()` calls.
4. Add both files to `module.vcxproj` + `module.vcxproj.filters` so MSVC picks them up.
5. Optionally drop a matching KB topic at `KNOWLEDGE/topics/<tool-slug>.md` explaining when the model should reach for the tool.

## Tool result shape

Every tool returns the same `ToolResult` struct so the agent can serialize it uniformly back to the provider. Errors are still results (with `ok == false`) — they're not exceptions, because the model needs to see and reason about them.

## Main-thread marshaling

Tool bodies that touch the FlexSim model tree (SDK calls or FlexScript `exec()`) MUST run on the FlexSim main thread. The tool registry is currently called from the agent's worker thread. A main-thread task queue is the keystone scaffolding for any tool body beyond the trivial in-memory ones — see the walkthrough in [`docs/architecture/2026-05-24-undo-research.md`](../../docs/architecture/2026-05-24-undo-research.md) for the pattern.
