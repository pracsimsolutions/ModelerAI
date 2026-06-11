---
title: modelerai_run_to_end
status: v2 — step-loop, required safety cap, no target time, no condition
---

# `modelerai_run_to_end`

Run the model one event at a time (`step()` loop) until the event queue drains naturally — meaning the model has nothing left to do.

## When NOT to use this

- "Run to sim time X" / "run for X seconds" / "advance to time X" → use **`modelerai_run_to_time`**.
- "Run until <some condition>" → use **`modelerai_run_until`**.

## When to use

- "Play the model to completion" / "let it run until nothing's left" / "run until the source stops producing and everything finishes."
- The modeler has an `ArrivalSchedule` or finite source and wants to see the final state.

**`step()` ignores user-defined stop times.** Stop-time entries are silently blown through. Use `modelerai_run_to_time` if stop times matter.

## Arguments

```json
{
  "safety_sim_seconds": 259200
}
```

`safety_sim_seconds` is **required.** Pick a budget the run shouldn't exceed (e.g. `259200` = 3 days). There's no default — omitting returns `missing_safety_cap`. The cap is a safety net for cases where the source is infinite and the queue never actually drains.

The model is **reset automatically** before the loop starts.

## Returns

```json
{
  "ok":                   true,
  "reason":               "events_drained",
  "final_sim_time":       9821.5,
  "steps_taken":          18432,
  "safety_sim_seconds":   259200
}
```

### Exit reasons

| `reason` | Meaning |
|---|---|
| `events_drained` | `eventqty() == 0`. Normal terminal state — model played to completion. |
| `safety_capped` | Sim time exceeded `safety_sim_seconds`. Either raise the cap or check that the source/event queue actually drains (an infinite Source will never end). |

## Examples

**Standard play-to-completion run with a 3-day cap.**
```json
{ "safety_sim_seconds": 259200 }
```

**Tighter cap if the modeler knows the run is short.**
```json
{ "safety_sim_seconds": 28800 }
```

## Errors

| Code | Cause |
|---|---|
| `missing_args` | Args weren't a JSON object. |
| `missing_safety_cap` | `safety_sim_seconds` field missing. |
| `bad_safety_cap` | `safety_sim_seconds` ≤ 0. |
| `run_to_end_failed` | The step-loop threw. Check the System Console. |

## See also

- [`modelerai_run_to_time`](modelerai_run_to_time.md) — for "run to sim time X". Honors stop times; faster (`go()` + message-pump waits).
- [`modelerai_run_until`](modelerai_run_until.md) — for "run until <condition>" with a FlexScript expression.
