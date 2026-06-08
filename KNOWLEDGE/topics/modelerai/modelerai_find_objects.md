---
title: modelerai_find_objects
status: SHELL — not yet implemented
---

# `modelerai_find_objects`

> ⚠️ **STATUS: SHELL.** This command is registered (so the AI can discover
> it via `modelerai_call`) but its body is not yet implemented. Calling it
> today returns `err:not_implemented` with a pointer at the TODO block
> above `ModelerAi_findObjects` in `ModelerAiDLL/commands/commands.cpp`.
>
> Implementation is blocked on FlexSim-specific context Josh is feeding
> back to Claude one command at a time. When the body lands, this stub
> gets rewritten with the real argument contract, return shape, and
> examples.

## Intent

Search the active model for objects matching name pattern, class, group membership, or parent path. Returns a JSON array of `{path, name, class}` matches.

## Why it's needed

Discovery is currently faked with `run_script` + `treenodewalker` loops, which is fragile and slow. A curated finder is read-only (no approval needed) and removes a whole class of hallucination.

## Status / next step

See the TODO block above `ModelerAi_findObjects` in
[`ModelerAiDLL/commands/commands.cpp`](../../../ModelerAiDLL/commands/commands.cpp)
for the open questions the implementation is waiting on.
