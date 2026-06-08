---
title: modelerai_run_until
status: v1 — step()-driven; ignores user-defined stop times
---

# `modelerai_run_until`

Run the model **one event at a time** (`step()` loop) until a condition becomes true. Two condition shapes — a FlexScript expression OR a structured PerformanceMeasure check.

## When to use

- The modeler asks "run until 100 items reach the sink" / "until average wait time exceeds 10s" / "until OperatorA is busy".
- You need to check a condition that depends on running state (a stat, a label value, a PM result) — something you can't precompute as a sim-time target.
- You want fine-grained control: every event boundary is a checkpoint.

**Important:** `step()` ignores user-defined stop times. If the modeler has stop times in the model, this tool will blow through them silently. Use [`modelerai_run_to_time`](modelerai_run_to_time.md) when stop times matter.

## Arguments — two condition shapes

**Shape A: raw FlexScript expression.** Most flexible.
```json
{
  "expression": "Model.find(\"Sink1\").stats.input.value >= 100",
  "timeout_seconds": 300
}
```
The expression is wrapped as `return (<expr>) ? 1 : 0;` and evaluated after each step. Non-zero result = condition met. If the expression throws, that step's evaluation is treated as false (soft-fail).

**Shape B: structured PerformanceMeasure check.** Safer when a PM already exists.
```json
{
  "pm": "SinkThroughput",
  "op": ">=",
  "value": 100,
  "timeout_seconds": 300
}
```
`op` ∈ `">="`, `">"`, `"<="`, `"<"`, `"=="`, `"!="`.
The PM is resolved once at start; if it doesn't exist, the call errors with `pm_not_found` before any stepping.

| Field | Default | Purpose |
|---|---|---|
| `timeout_seconds` | `300` | Real-world budget. Pass a larger value for slow models; pass `-1` to disable. |

**You must supply EXACTLY ONE shape — not both.** Mixing them returns `bad_condition_shape`.

## Returns

```json
{
  "ok":                    true,
  "completed":             true,
  "user_stopped":          false,
  "timed_out":             false,
  "stalled":               false,
  "final_sim_time":        432.1,
  "steps_taken":           4811,
  "final_condition_value": 100,
  "reason":                "condition_met"
}
```

| Field | Meaning |
|---|---|
| `completed` | `true` only when `reason == "condition_met"`. |
| `final_condition_value` | The last evaluated value of the condition. For Shape A: the expression's numeric result (1.0 for true, 0.0 for false, or whatever the expression returned). For Shape B: the final PM value. |
| `steps_taken` | Number of `step()` calls fired during the loop. |

### Exit reasons

| `reason` | `completed` | Meaning |
|---|---|---|
| `condition_met` | `true` | **Normal terminal state.** Condition evaluated truthy. |
| `events_drained_before_condition` | `false` | Event queue emptied before the condition turned true. Model finished without reaching the target. |
| `user_stopped` | `false` | Modeler clicked Stop in the viewer. |
| `wall_timeout` | `false` | `timeout_seconds` elapsed. |
| `stalled` | `false` | Sim time didn't advance by 5 sec in any 5 wall-sec window. |

## Examples

**Run until the sink has processed 100 items (Shape A).**
```json
{ "expression": "Model.find(\"Sink1\").stats.input.value >= 100" }
```

**Run until average wait time in Queue1 exceeds 10 seconds (Shape A).**
```json
{ "expression": "Model.find(\"Queue1\").stats.staytime.average() > 10" }
```

**Run until a PerformanceMeasure crosses a threshold (Shape B).**
```json
{ "pm": "SinkThroughput", "op": ">=", "value": 100 }
```

**Run until a label value is reached on a specific object (Shape A).**
```json
{ "expression": "Model.find(\"Tracker\").labels[\"alert_count\"].value >= 5" }
```

## Errors

| Code | Cause |
|---|---|
| `missing_condition` | Neither `expression` nor `pm`+`op`+`value` supplied. |
| `bad_condition_shape` | Both shapes supplied in one call. Pick one. |
| `bad_operator` | `op` is not in `>=`, `>`, `<=`, `<`, `==`, `!=`. |
| `pm_not_found` | Shape B: `pm` doesn't resolve to a PerformanceMeasure. Call `modelerai_list_performance_measures` to see what exists. |
| `expression_eval_failed` | Shape A: the expression failed to evaluate on EVERY step. Likely a typo, missing object reference, or wrong field name. The error includes the offending expression. |
| `step_failed` | `step()` threw mid-loop. Model state may be invalid. |

## Sim-time-progress watchdog

Same safety net as `run_to_end` and `run_to_time`: if sim time fails to advance by at least 5 sec across any 5 wall-second window, the call exits with `stalled: true`. Common cause is a runaway trigger body that the modeler added recently.

When `stalled` fires, the response includes a `stalled_note` pointing at likely culprits (most recently-set triggers and FlexScript-valued properties).

## See also

- [`modelerai_run_to_time`](modelerai_run_to_time.md) — when a sim-time target matters and you want stop times honored.
- [`modelerai_run_to_end`](modelerai_run_to_end.md) — when "play to completion" is the goal.
- [`modelerai_create_performance_measure`](modelerai_create_performance_measure.md) — Shape B requires a registered PM.
