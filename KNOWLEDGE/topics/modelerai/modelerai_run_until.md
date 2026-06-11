---
title: modelerai_run_until
status: v2 — step-loop, FlexScript condition only, required safety cap
---

# `modelerai_run_until`

Run the model one event at a time (`step()` loop) until a FlexScript condition becomes true.

## When NOT to use this

- "Run to sim time 10000" / "run for 10000 seconds" / "advance to time X" → use **`modelerai_run_to_time`** instead. Do not synthesize `condition: "time() >= 10000"` here; that's much slower and ignores stop times.
- "Run until the queue is empty" / "run until events drain" → use **`modelerai_run_to_end`** instead.

## When to use

- The modeler asks "run until 100 items reach the sink" / "until Queue1 has 5 items" / "until OperatorA is busy".
- The exit criterion is a state condition you can express as a FlexScript expression, NOT a sim-time target.

**`step()` ignores user-defined stop times.** Stop-time entries in the model are silently blown through. Use `modelerai_run_to_time` if stop times matter.

## Arguments

```json
{
  "condition":           "Model.find(\"Sink1\").stats.input.value >= 100",
  "safety_sim_seconds":  259200
}
```

Both fields **required.**

| Field | Type | Purpose |
|---|---|---|
| `condition` | string | FlexScript expression. Wrapped at compile site as `if ((<your expression>)) { … }`. Should evaluate to truthy when the modeler wants to stop. |
| `safety_sim_seconds` | number | Sim-time cap. The loop bails if `time() > safety_sim_seconds`, returning `reason: "safety_capped"`. Pick a budget the run shouldn't exceed (e.g. `259200` = 3 days). NO default — omitting returns `missing_safety_cap`. |

The model is **reset automatically** before the loop starts.

## Returns

```json
{
  "ok":                    true,
  "reason":                "condition_met",
  "final_sim_time":        432.1,
  "steps_taken":           4811,
  "safety_sim_seconds":    259200,
  "condition_value":       true
}
```

### Exit reasons

| `reason` | Meaning |
|---|---|
| `condition_met` | The FlexScript condition turned truthy. Normal exit. |
| `events_drained` | `eventqty() == 0` before the condition turned true. Model finished without reaching target. |
| `safety_capped` | Sim time exceeded `safety_sim_seconds` before the condition turned true. Either raise the cap or check that the condition is achievable. |

## Examples

**Run until the sink has processed 100 items, with a 3-day safety cap.**
```json
{
  "condition": "Model.find(\"Sink1\").stats.input.value >= 100",
  "safety_sim_seconds": 259200
}
```

**Run until average wait time in Queue1 exceeds 10 seconds.**
```json
{
  "condition": "Model.find(\"Queue1\").stats.staytime.average() > 10",
  "safety_sim_seconds": 259200
}
```

**Run until a label crosses a threshold.**
```json
{
  "condition": "Model.find(\"Tracker\").labels[\"alert_count\"].value >= 5",
  "safety_sim_seconds": 28800
}
```

## Errors

| Code | Cause |
|---|---|
| `missing_args` | Args weren't a JSON object. |
| `missing_condition` | `condition` field missing or empty. |
| `missing_safety_cap` | `safety_sim_seconds` field missing. The cap is required. |
| `bad_safety_cap` | `safety_sim_seconds` ≤ 0. |
| `run_until_failed` | The step-loop threw — most likely a typo in the condition expression. The error message includes the offending expression. |
| `loop_returned_wrong_type` | The condition compiled but didn't evaluate to a number on the test. Check the expression syntax. |

## See also

- [`modelerai_run_to_time`](modelerai_run_to_time.md) — for sim-time targets like "advance to time 10000". Faster (`go()` + message-pump waits, not `step()`), honors user stop times.
- [`modelerai_run_to_end`](modelerai_run_to_end.md) — for "run to completion" without a condition.
