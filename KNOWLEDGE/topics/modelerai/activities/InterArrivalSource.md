---
title: InterArrivalSource activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# InterArrivalSource

**What it does:** Creates tokens at a recurring inter-arrival interval.
Most common token source in ProcessFlow models.

## Variables

### `interArrivalTime` (dataType 2 → string, dual-mode) — SEMANTIC

The time between successive token arrivals. **Dual storage mode**
(literal-number vs FlexScript-expression), same as
[`Delay.delayTimeNode`](Delay.md) — same model-units behaviour, return
a number, etc.

**One difference: this source's codeHeader has NO `Token token = param(3);`**.
The body runs BEFORE the next token is created — there's no token to
reference yet:

```
Object current = param(1);
treenode activity = param(2);
treenode processFlow = ownerobject(activity);
```

Example bodies:
- `30` (literal — every 30 model-time units)
- `return exponential(0, 30, 0);` (exponential mean 30)
- `return Table("ArrivalIntervals")[1][1];` (table-driven; e.g. one
  value per shift period)

### `arrivalAtTimeZero` (dataType 1 → number, 0/1) — SEMANTIC

Controls when the **first** token arrives:
- `1` → first token at sim time 0, no initial delay
- `0` → first token after the first sampled `interArrivalTime`

Common pattern: set to `1` for "start with one in the system"; set to
`0` for the more realistic "first arrival is itself stochastic".

### `tokenName` (dataType 2 → string, dual-mode) — SEMANTIC

The name applied to each created token. Same dual-mode storage
(plain-text literal vs FlexScript expression). **This one's codeHeader
includes `Token token`** because the token exists by the time it's
named:

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
treenode processFlow = ownerobject(activity);
```

Example values:
- `Order` (literal — every token is named "Order")
- `return "Order_" + string(token.id);` (auto-numbered)
- `return Table("CustomerNames")[duniform(1, 100, 0)][1];` (sampled)

### `enabled` (dataType 1 → number, 0/1) — SEMANTIC

`1` produces tokens; `0` silent. **Read at sim start — runtime toggling
has no effect** until the next model reset. If you want a source that can
be turned off mid-run, use an EventTriggeredSource or wrap with
`Acquire` semantics instead.

### Inherited / visual — DANGEROUS or USELESS

Same policy as Delay — see [Delay topic](Delay.md). **NEVER set
`next`/`prev`** (use `modelerai_connect_activities`).

## Common patterns

```jsonc
// Exponential interarrival, no initial token, default name
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "interArrivalTime", value: "return exponential(0, 30, 0);"
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "arrivalAtTimeZero", value: 0
}

// Constant 60-unit interval, first token at t=0
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "interArrivalTime", value: "60"
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "arrivalAtTimeZero", value: 1
}

// Auto-numbered token names
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "tokenName", value: "return \"Order_\" + string(token.id);"
}
```

## Gotchas

- **Different codeHeader for `interArrivalTime` vs `tokenName`.**
  `interArrivalTime` runs before the token exists, so its header omits
  `Token token`. `tokenName` runs after, so it has the token. Mixing
  them up will give "undefined `token`" errors.
- **Units come from the model.** Literal `30` is "30 model-time units" —
  hours, minutes, or seconds depending on the model setup. Same trap as
  Delay.
- **`enabled = 0` is not a runtime kill switch.** It's start-of-run only.
  Don't expect to toggle it mid-simulation via `set_activity_variable`.
