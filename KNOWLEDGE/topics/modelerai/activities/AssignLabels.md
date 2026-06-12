---
title: AssignLabels activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# AssignLabels

**What it does:** Sets one or more labels on a target (token, object, etc.)
as the token passes through. The target is determined by `assignTo`, and
the actual label key/value pairs live in a hidden `>labels` (or
`>assignments`) subnode under the activity.

## Variables

### `assignTo` (dataType 7 → SIMPLE — polymorphic) — SEMANTIC

DATATYPE_SIMPLE means the cell can hold a coupling pointer, a literal,
OR a FlexScript expression; the engine evaluates differently depending
on which form was assigned. For `assignTo`, this surfaces as three
distinct storage modes — it determines WHAT the labels get assigned to:

**Pointer mode** (coupling to a specific object)
- The node holds a direct treenode coupling to a model object
  (e.g. `Source1`, `Queue1`).
- Labels get assigned to that exact object every time, regardless of
  token state.

**FlexScript-expression mode** (string body returning an Object)
- The node compiles as FlexScript with the standard PF codeHeader:
  ```
  Object current = param(1);
  treenode activity = param(2);
  Token token = param(3);
  treenode processFlow = ownerobject(activity);
  ```
- The body must `return` an `Object` (or treenode that's an object).
- Examples:
  - `return token;` — assign to the token itself
  - `return token.labels["targetItem"].value;` — assign to whatever
    object the token's `targetItem` label points to
  - `return Model.find("Queue1");` — always assign to Queue1
  - Conditional / lookup logic before the return is fine — same shape
    as Delay's `delayTimeNode` FlexScript mode.

**Setting `assignTo` via the curated tool** — pick the value shape by intent:
- **Token (most common):** `{flexscript: "return token;"}` — marks the node
  flexscript and auto-prepends the codeHeader so `token` is in scope.
- **A specific existing model object:** `{model_object: "Queue1"}` — stores a
  coupling pointer. (Do NOT use `{ref:...}` for a model object; `ref` is for a
  sibling ACTIVITY, not a model object.)
- **Computed target:** `{flexscript: "return token.labels[\"targetItem\"].value;"}`.

### `>labels` subnode (hidden container) — WHERE THE ACTUAL LABEL KEY/VALUES LIVE

This is a **Table** at the hidden subnode `>labels`. Column 1 = label name,
column 2 = value (any cell can be a literal or a FlexScript expression). Write
it with the activity-table tools, passing `variable: ">labels"`:

```jsonc
// label "status" = literal string "processed"
modelerai_set_activity_table_cell { processflow, activity: "AssignLabels1",
  variable: ">labels", row: 1, col: 1, value: "status" }
modelerai_set_activity_table_cell { processflow, activity: "AssignLabels1",
  variable: ">labels", row: 1, col: 2, value: "processed" }

// label "entryTime" = current model time (FlexScript — evaluates per token)
modelerai_set_activity_table_cell { processflow, activity: "AssignLabels1",
  variable: ">labels", row: 2, col: 1, value: "entryTime" }
modelerai_set_activity_table_cell { processflow, activity: "AssignLabels1",
  variable: ">labels", row: 2, col: 2, value: { "flexscript": "return time();" } }
```

The cell tool auto-grows the table, so you can write any (row, col) directly.
**Label names cannot contain spaces** — use `entryTime`, not `entry time`.

### `includeDefaultLabels` (dataType 1 → number)

Unknown purpose for our use case — not important. Leave at default.

### Inherited / visual — DANGEROUS or USELESS

`container`, `lockedToMe`, `next`, `prev`, `fontName`, `fontSize`, etc.
See the Delay topic for the inherited-vars policy. **Same rules apply:
NEVER set `next`/`prev` — use `modelerai_connect_activities` instead.**

## Common patterns

```jsonc
// 1. Target the token itself (default for label assignment)
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "AssignLabels1",
  variable: "assignTo", value: { "flexscript": "return token;" }
}
// 2. Add the label rows (see the >labels section above for the table writes)
```

## Gotchas
- **`assignTo` for a model object: use `{model_object:"X"}`** (coupling
  pointer) — that's the clean shape. `{flexscript:"return Model.find(\"X\");"}`
  also works but is heavier. `{ref:...}` is WRONG here (it's for sibling
  activities).
- **Label names cannot contain spaces** — `entryTime`, not `entry time`.
- **`>labels` cells:** literal strings/numbers store as-is; pass
  `{flexscript:...}` for a value that must evaluate per token (e.g.
  `return time();`).
