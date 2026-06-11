---
title: modelerai_run_to_time
status: v4 — worker-thread polling via runAndWait; cancel + 10-min hard timeout
---

# `modelerai_run_to_time`

Run the model from sim time 0 to a target sim time. Uses `go()` (full speed, not `step()`), honors user-defined stop times along the way, detects sim-time stalls, and cleans up after itself.

## When to use this — broadly

This is the **default tool for "advance the model to a specific point in time."** Any phrasing that implies a sim-time target maps here:

- "Run to sim time 10000"
- "Run for 10000 seconds"
- "Advance to time X"
- "Run the model to 10000"
- "Get the model to sim time X"
- "Skip ahead to time X"

**Do NOT use `modelerai_run_until` with `condition: "time() >= N"`.** It works but is much slower (step-loop) and ignores stop times.

## When NOT to use

- "Run until <state condition>" (queue length, sink count, label value, PM threshold) → use `modelerai_run_until`.
- "Run to completion" / "play until events drain" → use `modelerai_run_to_end`.

## How it works

The dispatcher (`modelerai_call`) takes a special path for this command. The wait runs on the worker thread, with each tiny main-thread incursion gated by `runAndWait`:

1. Worker parses `target_sim_time` from args.
2. Worker calls `runAndWait` for **install + go()**: builds the temp stop (`getmodelunit(STOP_TIME_NODE)` + `createcopy` + `function_s("setModelTime")`), `stoptime(0, 0)`, `resetmodel(1)`, `runspeed(INT_MAX)`, `go()`. Returns quickly (`go()` is async).
3. Worker enters the **poll loop**: sleep 100ms, then `runAndWait` to read `time()` / `eventqty()` / `getrunstate()`. Decides whether to keep going, advance past a user stop (`Model.nextStopTime` + `go()`), break for drained/stalled/target_reached. Cancel + 10-min hard timeout checked every tick.
4. Loop exits → worker calls `runAndWait` for **cleanup**: optional `stop(1)` (cancel/timeout paths), `applicationcommand("switchRunning", 0)`, find temp stop by `dateString` marker and `destroy()`, `stoptime(0, 0)`.
5. Worker builds result JSON and returns.

**Two architectures we tried first and abandoned:**

- **Sync C++ on main thread with `MsgWaitForMultipleObjects` + message pump.** Sim never advanced (`time()==0` every poll). The main thread was busy in our wait code; the FlexSim scheduler didn't get to run.
- **C++20 coroutine with `co_await Delay::realTime(.1)`.** Sim DID run to target, but the coroutine never resumed after its first yield. Suspect: under `runspeed(INT_MAX)` FlexSim is processing events so aggressively it never serves the wall-clock Delay timer, and when sim halts at the temp stop, FlexSim may stop servicing queued wall-clock timers entirely. `SetEvent` never fired; worker hung forever.

The current architecture (worker-thread polling via `runAndWait`) works because the main thread is FlexSim's to own for the 100ms stretches between our incursions, sim advances normally, and the worker stays cancelable.

The model is reset to t=0 every call — this tool always starts fresh.

The export `ModelerAi_runToTime` registered in `ModelerAI.fsx` is a stub that returns `error_code: "invoke_via_modelerai_call"` if anyone calls it through `applicationcommand` directly. The dispatcher path is the only way to invoke this correctly.

## Arguments

```json
{
  "target_sim_time": 10000
}
```

Also accepted: bare number (`10000`), positional array (`[10000]`), or any of these JSON keys for the target: `target_sim_time` / `target` / `time` / `sim_time`.

| Field | Type | Required | Purpose |
|---|---|---|---|
| `target_sim_time` | number > 0 | yes | Absolute sim-clock seconds. The model resets to 0 then runs to this point. |

## Returns

```json
{
  "ok":              true,
  "completed":       true,
  "reason":          "target_reached",
  "target_sim_time": 10000,
  "final_sim_time":  10000
}
```

### Exit reasons

| `reason` | `completed` | Meaning |
|---|---|---|
| `target_reached` | `true` | Sim time hit the target. Normal exit. |
| `events_drained` | `false` | `eventqty() == 0` before reaching target. The model ran out of work — an ArrivalSchedule finished, every flowitem reached a Sink, etc. Inspect with `modelerai_get_model_summary`. |
| `stall_detected` | `false` | Sim time advanced less than 1 sim-second in the last 100ms wall-clock poll. Almost always a runaway trigger body or zero-time event loop. |
| `canceled` | `false` | The chat's Stop button was pressed (or the agent decided to bail). Cleanup includes `stop(1)`. |
| `wall_timeout` | `false` | Hard 10-minute wall-clock cap reached. Sim may still be running in FlexSim — check the toolbar; the response advises a manual stop. Use this as a signal that the run is far slower than expected. |

## Examples

**Advance to sim time 10000.**
```json
{ "target_sim_time": 10000 }
```

**Bare number.**
```json
10000
```

**Two-step inspection workflow.**
1. `modelerai_run_to_time({"target_sim_time": 3600})` — get to 1 hour
2. `modelerai_get_object_stats({"object": "Sink1"})` — read what happened

## Errors

| Code | Cause |
|---|---|
| `missing_target` | No numeric target supplied. |
| `bad_target` | `target_sim_time` ≤ 0. |
| `install_failed` | The temp-stop install FlexScript threw (almost always a model in a broken state — try `modelerai_reset_model` first). |

## Interaction with user-defined stop times

User-managed stop times (the ones visible in Toolbox → ModelDateTimes → Stop Times) ARE honored. When the run hits one before reaching `target_sim_time`, the polling loop detects it (`time() == Model.nextStopTime` + `getrunstate() == 0`), reads the next stop time, and calls `go()` again to continue past it. Repeat until the target is reached.

This means: a user stop time at 5000 won't stop a `run_to_time(10000)` call. It WILL stop a run if `target_sim_time > stop_time` and the loop is configured to halt there (currently it always continues).

## See also

- [`modelerai_run_until`](modelerai_run_until.md) — for state-condition-based exits.
- [`modelerai_run_to_end`](modelerai_run_to_end.md) — for "play until events drain."
- [`modelerai_add_stop_time`](modelerai_add_stop_time.md) — to set up persistent stop times before a run.
