---
title: modelerai_create_tracked_variable
status: SHELL — not yet implemented
---

# `modelerai_create_tracked_variable`

> ⚠️ **STATUS: SHELL.** This command is registered (so the AI can discover
> it via `modelerai_call`) but its body is not yet implemented. Calling it
> today returns `err:not_implemented` with a pointer at the TODO block
> above `ModelerAi_createTrackedVariable` in `ModelerAiDLL/commands/commands.cpp`.
>
> Implementation is blocked on FlexSim-specific context Josh is feeding
> back to Claude one command at a time. When the body lands, this stub
> gets rewritten with the real argument contract, return shape, and
> examples.

## Intent

Create a TrackedVariable tool with an initial value (and possibly Time-Weighted vs Sample type).

## Why it's needed

TrackedVariables are the simplest analytics primitive in FlexSim. Worth curating because creating one via `run_script` requires knowing the exact library template path, which the AI gets wrong.

## Status / next step

See the TODO block above `ModelerAi_createTrackedVariable` in
[`ModelerAiDLL/commands/commands.cpp`](../../../ModelerAiDLL/commands/commands.cpp)
for the open questions the implementation is waiting on.
