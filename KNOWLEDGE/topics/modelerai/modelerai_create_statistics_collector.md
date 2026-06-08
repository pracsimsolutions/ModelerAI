---
title: modelerai_create_statistics_collector
status: SHELL — not yet implemented
---

# `modelerai_create_statistics_collector`

> ⚠️ **STATUS: SHELL.** This command is registered (so the AI can discover
> it via `modelerai_call`) but its body is not yet implemented. Calling it
> today returns `err:not_implemented` with a pointer at the TODO block
> above `ModelerAi_createStatisticsCollector` in `ModelerAiDLL/commands/commands.cpp`.
>
> Implementation is blocked on FlexSim-specific context Josh is feeding
> back to Claude one command at a time. When the body lands, this stub
> gets rewritten with the real argument contract, return shape, and
> examples.

## Intent

Create a StatisticsCollector tool — given an event source (object + event name), a list of column expressions, and optional when/where filters.

## Why it's needed

Stats collectors are how nearly every dashboard chart gets its data. The current surface has no curated path, and the GUI's tree shape is non-trivial to reproduce via `run_script`. Explicitly called out as a gap in the agent's standing prompt.

## Status / next step

See the TODO block above `ModelerAi_createStatisticsCollector` in
[`ModelerAiDLL/commands/commands.cpp`](../../../ModelerAiDLL/commands/commands.cpp)
for the open questions the implementation is waiting on.
