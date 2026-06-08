---
title: modelerai_run_to_end
status: v1
---

# `modelerai_run_to_end`

Run the model until the event queue drains naturally. No target sim time — the model plays to completion (every Source's `ArrivalSchedule` runs out, every item reaches the Sink, etc.).

## When to use

- The modeler asks "run it to completion" or "play it out" without specifying a target time.
- You set up a finite Source (e.g., `ArrivalSchedule` with N rows) and want to see the throughput when all those items have been processed.
- You're not sure how long the model needs to run, and waiting for natural exhaustion is cheaper than guessing a target.

Don't use this for **unbounded** models (a Source with `InterArrivalTime` that never stops producing). Those will run until the wall-clock timeout fires or the watchdog catches a stall — `events_drained` will never happen.

## Arguments

```json
{ "timeout_seconds": 300 }
```

All fields optional. Also accepts a bare positional number for the timeout:

```
600
```

| Field | Default | Purpose |
|---|---|---|
| `timeout_seconds` | `300` | Real-world budget. Pass a larger value for slow models; pass `-1` to disable (not recommended). |

## Returns

```json
{
  "ok":             true,
  "completed":      true,
  "user_stopped":   false,
  "timed_out":      false,
  "stalled":        false,
  "final_sim_time": 487.3,
  "run_state":      "Stopped",
  "reason":         "events_drained"
}
```

### Exit reasons

| `reason` | `completed` | Meaning |
|---|---|---|
| `events_drained` | `true` | **Normal terminal state.** Queue emptied; model played to completion. |
| `user_stopped` | `false` | The modeler clicked Stop in the viewer. |
| `wall_timeout` | `false` | `timeout_seconds` elapsed before the queue drained. |
| `stalled` | `false` | Sim time didn't advance by 5 sec in any 5 wall-sec window. Likely a runaway trigger or recursive event loop. |
| `earlier_stop_fired` | `false` | A user-defined stop time fired before the queue drained. |

When `stalled: true`, the response includes a `stalled_note` with remediation. When `timed_out: true`, the response includes `timeout_seconds` + `timeout_note`.

## Examples

**Play out a model with a finite ArrivalSchedule.**
```json
{}
```
Defaults: 300s wall-clock timeout. Returns `events_drained` when the last item exits the Sink.

**Allow a long-running playout.**
```json
{ "timeout_seconds": 1800 }
```
30 wall-minutes before the timeout fires.

## Errors

| Code | Cause |
|---|---|
| `go_failed` | `go(1)` threw — the model isn't in a runnable state (corrupt state, mid-edit). Reset and retry. |

## See also

- [`modelerai_run_to_time`](modelerai_run_to_time.md) — when a specific target sim time matters.
- [`modelerai_run_until`](modelerai_run_until.md) — when a specific condition (PM, label, expression) is the stopping point.
- [`modelerai_get_model_summary`](modelerai_get_model_summary.md) — call after `run_to_end` to read throughput and stuck-objects.
