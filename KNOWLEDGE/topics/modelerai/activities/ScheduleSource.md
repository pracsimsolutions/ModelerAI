---
title: ScheduleSource activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# ScheduleSource

**What it does:** Creates tokens according to a fixed time schedule
held in a table. Each row specifies a time, name, and quantity; rows
may also carry label assignments in extra columns.

## Variables

### `arrivals` (dataType 0 → **Table**, NOT an SDT) — SEMANTIC

This one is special: despite sharing the dataType-0 code with SDT-shaped
variables, `arrivals` is a **Table**, accessible via the standard
FlexScript Table API:

```flexscript
Table table = getvarnode(Model.find("Tools/ProcessFlow/MainFlow/Source1"), "arrivals");
table[r][c];                 // read
table[r][c] = "exponential(0, 60, 0)";  // write
```

#### Column shape

| Column | Header (fixed) | Role |
|---|---|---|
| 1 | `Time`     | When this arrival fires (model time units) |
| 2 | `Name`     | Token name |
| 3 | `Quantity` | How many tokens to create at this time |
| 4..N | `<labelName>` | Each extra column becomes a label on the created tokens; the column header IS the label name |

The first three columns are required. Any number of additional columns
may be added — each one's header becomes a label key, and the cell
value becomes that label's value (literal or expression).

#### Cell value modes

Any cell can hold either a literal value or a FlexScript expression.

**Time / Name / Quantity cells** — TODO: codeHeader for these cells is
not yet documented. Probably similar to InterArrivalSource's
`interArrivalTime` (no `Token token`, since the token isn't created
yet). Verify before writing FlexScript into these cells.

**Label cells** (columns 4+) — Use this **extended codeHeader** when a
label cell contains FlexScript (it's distinct from the Delay/AssignLabels
header — additional row/repeat/index/labelName params):

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
int rowNumber = param(4);    // The arrival entry row number
int repeatCount = param(5);  // Times the arrivals table has repeated (first iter = 0)
int tokenIndex = param(6);   // Index of the created token within the arrival entry
string labelName = param(7); // Column header (= label being set)
treenode processFlow = ownerobject(activity);
```

So a label cell can compute its value based on which row fired, which
loop iteration, which sub-token in the batch, and which column it's
assigning — useful for "first token of each arrival gets priority=high,
rest get priority=normal" patterns.

### `repeatSchedule` (dataType 1 → number, 0/1) — SEMANTIC

Toggle the looping behaviour:
- `0` → fire each row once, then stop.
- `1` → after the last row, restart from row 1; keep looping forever (or
  until capped by `repeatCount` — TBD whether `repeatCount` actually
  exists on this class as a cap, or is purely runtime read-only).

### `offsetTime` (dataType 1 → number) — SEMANTIC

A constant added to every row's `Time` value. **Applied ONCE** (not
re-added per loop pass).

Example: with `offsetTime = 20` and an arrivals table containing
`Time=10` and `Time=20` in two rows:
- First arrival at sim time `30` (10 + 20)
- Second arrival at sim time `40` (20 + 20)
- If `repeatSchedule = 1`, the third arrival continues with the loop's
  natural period — the offset isn't compounded.

### `repeatCount` (dataType 1 → number) — INTERNAL state

Read-only runtime counter — how many times the arrivals table has
restarted. **Do not set via `set_activity_variable`.**

### `currentArrival` (dataType 1 → number) — INTERNAL state

Read-only runtime cursor for the current row being processed.
**Do not set.**

### `lastTime` (dataType 1 → number) — INTERNAL state

Read-only runtime — sim time of the last arrival fired. **Do not set.**

### `enabled` (dataType 1 → number, 0/1) — SEMANTIC

Identical to [`InterArrivalSource.enabled`](InterArrivalSource.md) —
`1` produces, `0` silent, **read at sim start; runtime toggling has no
effect** until next reset.

### Inherited / visual — DANGEROUS or USELESS

Same policy. **Never set `next` or `prev`.** See [Delay topic](Delay.md).

## Common patterns

```jsonc
// Toggle repeating mode + add a 60-second offset
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "repeatSchedule", value: 1
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "Source1",
  variable: "offsetTime", value: 60
}
```

**Populating the arrivals table** (Time / Name / Quantity / labels)
currently requires FlexScript using the Table API directly — no curated
tool yet. The setvar tool can't reach individual cells. Tracked as a
TODO ("SDT/Table writer helper") and likely lands together with the SDT
helper for `events` / `assignTo` rows.

Until then, the agent can write a `run_script` (last resort) that
manipulates the table:
```flexscript
Table t = getvarnode(node("/Tools/ProcessFlow/MainFlow/Source1", model()), "arrivals");
t.setSize(3, 3);                   // 3 rows, default columns
t[1][1] = "0";  t[1][2] = "Order"; t[1][3] = "1";
t[2][1] = "10"; t[2][2] = "Order"; t[2][3] = "1";
t[3][1] = "20"; t[3][2] = "Order"; t[3][3] = "1";
```

## Gotchas

- **`arrivals` is a Table, not an SDT.** Cells are addressed by
  `[row][col]`, and the column header for columns 4+ becomes the
  label name on the spawned tokens. SDT helpers (`getsdtvalue` etc.)
  do NOT apply here.
- **`offsetTime` applies once at the start of the schedule**, not on
  every loop pass. Don't expect it to add to repeated arrivals.
- **Label cells have a richer codeHeader** than label expressions
  elsewhere — `rowNumber`, `repeatCount`, `tokenIndex`, and `labelName`
  are available. Don't redeclare them.
- **Internal counters (`repeatCount`, `currentArrival`, `lastTime`)**
  are read-only runtime. Setting them via `set_activity_variable`
  desyncs the source from its schedule.
- **`enabled = 0` is start-of-run only.** Same trap as other sources.
