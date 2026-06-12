---
title: EventTriggeredSource activity — variable semantics
status: v2 — hand-curated 2026-06-11; SDT shape from screenshots
---

# EventTriggeredSource

**What it does:** Creates a token whenever any subscribed FlexSim event
fires. Used for reactive flows — "when Queue1 gets an entry, start a
token" — rather than scheduled / periodic arrivals.

## Variables

### `events` (dataType 0 → container of SDTs) — SEMANTIC

`events` itself is NOT an SDT — it's a plain container node. Its
**subnodes** are each one SDT representing one event listener
subscription. Any number of subscriptions; if any fires, a token is
created.

#### Per-subscription SDT shape (verified from screenshot)

Each child SDT under `events/` has roughly this structure:

```
<event subscription SDT>
├─ objRef               — pointer to the source object (e.g. /Source1)
├─ eventName            — string (e.g. "OnCreation", "OnEntry")
├─ labelNames           — sub-SDT (enumerates Event Data → label assignment)
│   ├─ <EventDataName1> — one row per Event Data slot for this (object, event)
│   │   ├─ Label Name or Value
│   │   ├─ Operation    — enum: "none" | "assign" | "match value"
│   │   ├─ <empty slot>
│   │   ├─ <empty slot>
│   │   └─ <numeric>    — 0.00 / 1.00 / ...
│   ├─ <EventDataName2>
│   │   └─ ...
│   └─ ...
├─ overridesReturnValue — number (0/1)
├─ eventBinding         — sub-SDT
│   ├─ selectedEventTitle
│   └─ object           — pointer to source (mirrors objRef)
├─ eventBindingTable    — sub-table of available events for this object's class
│                         (size varies: 19 for Source, 22 for Queue, etc.)
├─ requirements         — sub-node (conditions for the listener to fire)
├─ changeRule           — number (when in change-tracking mode — TBD)
├─ changeValue          — number (compare value for change rule — TBD)
└─ fireOnInitialValueMet — number (0/1) — fire immediately if state already matches at sim start
```

#### `labelNames` is per-(object, event) enumerated

The set of rows under `labelNames` is NOT free-form — it comes from the
event's "Event Data" enumeration on that specific object class. Examples
verified from screenshot:

| Object | Event | Event Data slots |
|---|---|---|
| Source | OnCreation | "Created Item", "Row Number" |
| Queue  | OnEntry    | "Entering Item", "Input Port" |

**TODO — full per-(object, event) Event Data enumeration.** Each
FixedResource class + event combination has its own slot list. We need
a sweep that walks the class library + event-binding tables and outputs
the master enumeration so the agent can know "for `Queue.OnExit`, the
slots are X, Y, Z". This is a separate research task tracked in the todo
list.

#### Per-row Operation enum (verified)

| Operation | Meaning |
|---|---|
| `none`        | Don't assign anything from this Event Data |
| `assign`      | Assign the Event Data value to a label on the spawned token |
| `match value` | Filter — only fire if this Event Data matches |

#### Writing event subscriptions

**TODO — no curated tool yet.** Current `set_activity_variable` cannot
add or edit these SDTs. The mechanism is going to be: copy/replace from
a library SDT template into `events/`, then write each field via SDT
helpers (`getsdtvalue` / `function_s(sdt, "setXxx", value)`). Tracked as
a separate todo: "SDT writer helper / curated tool".

Until that exists, modeler configures events via the FlexSim UI.

### `assignObjectTo` (default dataType 3 → COUPLING, but swappable) — SEMANTIC

Default node type is COUPLING (pointer-only), but the engine supports
swapping the entire node — copy/replace from a different library
template — to switch storage modes. The template's dataType is the
*default* shape; what the modeler ends up with depends on which mode
they configured. Selects WHERE the event's source-object reference gets
stored when a token is created.

**Storage modes:**

**Pointer mode** (default — node stays as COUPLING (3))
- Direct pointer to one model object. The label assignment target is
  always that one object.

**SDT mode** (most common in UI: store the source object on a token label)
- Copy/replace swaps the node to a dataType=0 SDT structure — see
  CreateObject's `assignTo` discussion for the same pattern.

**FlexScript-expression mode** (write to any model-side label)
- Copy/replace swaps the node to dataType=2 STRING with FlexScript content.
- Standard PF codeHeader; body returns a label node.
- Example: `return Model.find("Queue1").labels["lastTrigger"];`

### `tokenName` (dataType 2 → string, dual-mode) — SEMANTIC

Identical to [`InterArrivalSource.tokenName`](InterArrivalSource.md) —
plain literal name OR FlexScript expression returning a string. Same
codeHeader (full version with `Token token`).

### `enabled` (dataType 1 → number, 0/1) — SEMANTIC

Identical to [`InterArrivalSource.enabled`](InterArrivalSource.md) —
`1` produces, `0` silent, **read at sim start; runtime toggling has no
effect** until next reset.

### `listenerCoupling` (dataType 0 → SDT) — INTERNAL

Internal engine bookkeeping wired during reset. **Never set directly via
`set_activity_variable`** — it gets corrupted and event subscriptions
silently stop firing.

### Inherited / visual — DANGEROUS or USELESS

Same policy. **Never set `next` or `prev`.** See [Delay topic](Delay.md).

## Common patterns

```jsonc
// Token name driven by the event source
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "EventSource1",
  variable: "tokenName",
  value: "return \"From_\" + string(token.labels[\"sourceObj\"].value.name);"
}

// Disable for now — won't take effect until model reset
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "EventSource1",
  variable: "enabled", value: 0
}
```

**`events` subscriptions still need UI configuration** until the SDT
writer helper lands.

## Gotchas

- **`events` is a container of SDTs, not an SDT itself.** Don't try to
  call SDT helpers directly on `events` — call them on each child SDT.
- **`labelNames` row set varies per (object, event).** Don't assume a
  fixed schema — the slots come from FlexSim's per-class event-data
  enumeration. Until that's documented per class, can't safely add a
  subscription programmatically.
- **`listenerCoupling` is wired by the engine during reset.** Touching
  it from the agent will break event delivery silently.
- **`enabled = 0` is start-of-run only.** Same trap as InterArrivalSource.
- **`assignObjectTo` SDT mode unsupported.** Until the curated tool can
  do copy/replace, use FlexScript-expression mode for any non-token
  target.
