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

**TODO:** the curated tool currently takes the value as a plain string
and doesn't differentiate the three modes. Setting a string via
`set_activity_variable` will likely produce a FlexScript-mode node. For
pointer mode, the agent would need to use the `{ref:"ObjectName"}` shape
(if supported by the tool's coupling write path) — needs verification.

### `>labels` subnode (hidden container) — WHERE THE ACTUAL LABEL KEY/VALUES LIVE

The `>variables` walk doesn't surface this; it's a separate hidden
container under the activity. The key/value pairs the modeler enters in
the AssignLabels UI grid end up here.

**TODO:** we have not yet enumerated the `>labels` subnode structure. The
curated `set_activity_variable` cannot write into `>labels` directly — a
new tool (or extension) is needed to add/edit individual label rows. A
later session should walk a configured AssignLabels instance and
document the shape:
- One subnode per label-to-assign?
- Each row holding `name` + `value`?
- Is `value` a literal or a FlexScript expression with the same header?

### `includeDefaultLabels` (dataType 1 → number)

Unknown purpose for our use case — not important. Leave at default.

### Inherited / visual — DANGEROUS or USELESS

`container`, `lockedToMe`, `next`, `prev`, `fontName`, `fontSize`, etc.
See the Delay topic for the inherited-vars policy. **Same rules apply:
NEVER set `next`/`prev` — use `modelerai_connect_activities` instead.**

## Common patterns

```jsonc
// Assign labels to the token itself (most common case)
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "AssignLabels1",
  variable: "assignTo", value: "return token;"
}
// Then the label rows themselves — TODO, no tool yet.
```

## Gotchas

- **Can't yet add label rows via curated tools.** Setting `assignTo` works
  via FlexScript-mode strings, but the actual key/value pairs require
  hand-editing the activity in the FlexSim UI until a `>labels` writer
  tool is added.
- **Pointer vs FlexScript mode.** If the modeler wants a hard-pinned
  target object, FlexScript-mode (`return Model.find("X");`) is the
  practical workaround until coupling-mode set is supported.
