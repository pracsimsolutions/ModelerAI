---
title: Delay activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# Delay

**What it does:** Holds the token for a duration, then releases it down the
next connector. The duration is determined by the `delayTimeNode` variable.

## Variables

### `delayTimeNode` (dataType 2 → string, BUT see "two modes" below) — SEMANTIC

The duration of the delay. Has **two distinct storage modes** the engine
toggles between based on the content set:

**Literal-number mode** (you set the value to something that parses as a
number)
- The node becomes a plain numeric node — the engine reads it directly.
- The number is interpreted in **the model's current time units**. If the
  model is configured in hours and `delayTimeNode = 1`, the delay is 1
  hour. If the model is in seconds, it's 1 second. There's no implicit
  unit; the modeler's model configuration decides.
- Example values: `30`, `0.5`, `120`.

**FlexScript-expression mode** (you set the value to anything that isn't
purely numeric)
- The node becomes a FlexScript node — the engine compiles the body and
  invokes it whenever it needs the duration.
- The body is wrapped with this codeHeader (auto-inserted; do NOT
  re-declare):
  ```
  Object current = param(1);
  treenode activity = param(2);
  Token token = param(3);
  treenode processFlow = ownerobject(activity);
  ```
- The body must `return` a single number (interpreted in model units, same
  as literal mode). You can write arbitrary FlexScript before the return.
- Example bodies:
  - Distribution: `return exponential(0, 30, 0);`
  - Token label read: `return token.labels["delay"].value;`
  - Conditional: `if (token.labels["urgent"]) return 5; return 60;`
  - With Object math: `Object src = Model.find("Source1"); return src.stats.output.value > 100 ? 30 : 60;`

**Setting via `modelerai_set_activity_variable`:**
The curated tool just writes whatever string you pass to the underlying
node. If you pass a numeric string (`"30"`), the engine *should* treat it
as literal-mode on next evaluation. If you pass a FlexScript body, the
engine *should* compile it as FlexScript. The exact mode-flip mechanism
(whether the tool needs to mark the node's dataType / nodeflags
explicitly) is **NOT yet verified** — TODO when we add a smoke test.

### `container`, `lockedToMe`, `next`, `prev` — DANGEROUS, DO NOT SET via the tool

- `container` and `lockedToMe` are visual stacking state. Setting them
  via `set_activity_variable` may break the activity's UI rendering.
- `next` and `prev` are connector wiring — the engine maintains them when
  the modeler draws a connector. **Always use
  `modelerai_connect_activities` to wire activities together**; never set
  `next` or `prev` directly. Direct sets bypass the auto-wiring logic and
  produce visually-broken / functionally-wrong PFs.
- TODO: tool should reject these variable names with a clear error
  ("use modelerai_connect_activities for wiring; container/lockedToMe are
  UI state and not safe to set via this tool").

### Other inherited variables (visual/cosmetic — safe but rarely useful)

`fontName`, `fontSize`, `fontStyle`, `fontColor`, `showLinks`,
`traceOption`, `traceRequirement`. The agent should skip these unless the
modeler explicitly asks for a visual change.

## Common patterns

```jsonc
// Fixed 30-time-unit delay
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Delay1",
  variable: "delayTimeNode", value: "30"
}

// Exponential distribution
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Delay1",
  variable: "delayTimeNode", value: "return exponential(0, 30, 0);"
}

// Token-label-driven (the token carries its own delay value)
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Delay1",
  variable: "delayTimeNode", value: "return token.labels[\"processTime\"].value;"
}
```

## Gotchas

- **Units come from the model, not the variable.** A modeler running in
  hours sees `30` as "30 hours". Always confirm the model's time unit
  before setting a literal — the wrong unit produces a delay that's off
  by 60× or 3600×.
- **Don't redeclare the codeHeader.** The header is auto-prepended on
  every FlexScript-mode evaluation; redeclaring `current` / `activity` /
  `token` / `processFlow` in your body is a compile error.
- **Negative or zero delays.** A 0 delay is a yield (like `Breathe`).
  A negative delay is undefined behaviour — don't pass one.
