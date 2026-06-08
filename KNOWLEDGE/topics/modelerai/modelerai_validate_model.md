---
title: modelerai_validate_model
status: SHELL — not yet implemented
---

# `modelerai_validate_model`

> ⚠️ **STATUS: SHELL.** This command is registered (so the AI can discover
> it via `modelerai_call`) but its body is not yet implemented. Calling it
> today returns `err:not_implemented` with a pointer at the TODO block
> above `ModelerAi_validateModel` in `ModelerAiDLL/commands/commands.cpp`.
>
> Implementation is blocked on FlexSim-specific context Josh is feeding
> back to Claude one command at a time. When the body lands, this stub
> gets rewritten with the real argument contract, return shape, and
> examples.

## Intent

Run a configurable set of read-only sanity checks across the active model — orphans, disconnected sources/sinks, undefined process times, infinite-capacity warnings, etc. Returns a structured warning report.

## Why it's needed

'Is my model fundamentally broken before I run it?' is a question users ask informally; surfacing it as a curated check list lets the AI proactively suggest fixes before a wasted run. Read-only so no approval needed.

## Status / next step

See the TODO block above `ModelerAi_validateModel` in
[`ModelerAiDLL/commands/commands.cpp`](../../../ModelerAiDLL/commands/commands.cpp)
for the open questions the implementation is waiting on.
