# agent/ — Turn loop, history, tool dispatch

The agent owns one model turn end-to-end. It reads the user message + prior conversation, asks the KB layer for relevant topics, asks the tool registry for available tools, hands all that to an `IProvider*` to stream a response, and dispatches any tool calls the model makes.

> **AI changes do not integrate with FlexSim's native undo stack.** No `beginaggregatedundo / endaggregatedundo` envelope wraps mutating tool calls. The rewind strategy is snapshot-based — see [`docs/architecture/2026-05-24-undo-research.md`](../../docs/architecture/2026-05-24-undo-research.md) for the plan that will land alongside the first real mutating tool body.

## Current files

- `message.h` — canonical `Message` / `Conversation` / `ToolDef` / `ToolCallRecord` types
- `agent.cpp / .h` — turn state machine (mode-aware tool filtering, free-quota gate, cost-ceiling gate, cancel-aware worker, session-id persistence)
- `prompt.cpp / .h` — split system-prompt assembler (stable prefix for prompt caching + variable suffix)
- `session_store.cpp / .h` — DPAPI-encrypted on-disk conversation persistence
- `slash.cpp / .h` — slash-command dispatcher (`/help`, `/clear`, `/cost`, `/model`, `/effort`, `/export`, `/tools`, etc.)
- `user_rules.cpp / .h` — user-authored standing rules loaded into the system prompt
- `user_interaction.cpp / .h` — `InteractionBroker` primitive (worker-thread CV blocking that powers `ask_user_question` + the approval modal)

## Turn flow

1. Bridge receives `user_message` envelope → agent gets the text (FlexSim main thread).
2. Bridge calls `tree::condenseActiveModel()` to produce a compact model outline (main thread; SDK is single-threaded).
3. Agent appends a `User` message to a snapshot copy of the conversation.
4. Agent asks `kb::buildKbBlockForTopic` for relevant KB topics; result becomes a "Relevant knowledge" block in the stable system-prompt prefix.
5. Agent asks `tool_registry` for the full `ToolDef` list. Ask mode filters mutating tools out; Plan + Apply send everything.
6. Agent spawns a detached worker thread and returns control to the FlexSim main thread.
7. Worker calls `provider->stream_turn(conv, tools, onChunk, onToolCall, onDone, onError, cancel)`.
8. For each chunk → emit `assistant_chunk` envelope back to the viewer.
9. For each tool call → look up the impl via `tool_registry`, dispatch through the per-mode policy (Plan intercepts mutating; Apply runs through the approval modal), append the result, and loop the provider call.
10. On done → append the assistant message to persistent history, increment counters, push `assistant_done` + `session_stats` + `license_status` + `session_save_status`, save to disk.

## Plan vs Apply

Mode-aware tool dispatch lives in `agent.cpp`:

- **Ask**: mutating tools are filtered out of `toolDefs` before send. The LLM never knows they exist this turn.
- **Plan**: mutating calls are intercepted in the dispatcher; the agent returns a synthetic `{plan_mode: true, would_have_called: ...}` result. The model describes its plan in text without touching the model.
- **Apply**: mutating calls hit `runWithApproval()` — the InteractionBroker blocks the worker, the viewer pops a modal, the user clicks Approve / Deny / Always-allow, the result is the user's decision.

## Cancel + teardown

`Agent::cancelCurrentTurn()` sets an atomic the WinHTTP read loop polls between chunks (sub-100ms latency in practice). `bootstrap::teardown()` signals cancel and briefly polls `turnInFlight()` before freeing the agent, to avoid a UAF in the detached worker.
