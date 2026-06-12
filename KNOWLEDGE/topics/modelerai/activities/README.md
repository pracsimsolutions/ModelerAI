---
title: ProcessFlow activity deep-dive index
status: live — batch 1 complete; more activities to follow
---

# ProcessFlow activity deep-dive

Per-class hand-curated docs describing each activity's variables in
depth: storage modes (literal vs FlexScript-expression vs coupling),
codeHeaders, value semantics, gotchas, and example `set_activity_variable`
calls.

This is the **semantic layer** on top of the structural data in:
- [processflow-activity-catalog.md](../processflow-activity-catalog.md) — which activity class exists and what it does in one sentence
- [processflow-activity-variables.md](../processflow-activity-variables.md) — every variable on every class with its raw dataType

When the agent needs to actually *set* a variable, it should read the
per-class doc here. The catalog tells WHICH class; the variables file
tells WHAT exists; this directory tells HOW to set it correctly.

## How to read these docs

Each activity doc has the same shape:

1. **What it does** — one paragraph.
2. **Variables** — each variable gets its own subsection:
   - DataType + kind (NUMBER / STRING / COUPLING / SIMPLE / container)
   - **SEMANTIC** vs **DANGEROUS** vs **USELESS** vs **INTERNAL** marker:
     - **SEMANTIC** — modeler-facing, set this via the curated tool
     - **DANGEROUS** — `next`/`prev`/`container`/`lockedToMe` etc. — never set directly; use specialized tools (`connect_activities` for wiring)
     - **USELESS** — visual cosmetics (`fontSize`, `traceOption`) — rarely worth touching
     - **INTERNAL** — engine bookkeeping (`listenerCoupling`, `currentArrival`, `lastTime`) — never set
   - Storage modes if polymorphic (SIMPLE / STRING-with-flexscript-toggle)
   - CodeHeader if the variable accepts a FlexScript body
3. **Common patterns** — concrete JSON examples the agent can mirror.
4. **Gotchas** — surprising / failure-mode notes.

## Batch 1 (complete)

These activities are documented end-to-end:

- [Delay](Delay.md)
- [AssignLabels](AssignLabels.md)
- [CustomCode](CustomCode.md)
- [InterArrivalSource](InterArrivalSource.md)
- [CreateObject](CreateObject.md)
- [EventTriggeredSource](EventTriggeredSource.md) — includes the `events`-of-SDTs shape
- [ScheduleSource](ScheduleSource.md) — note `arrivals` is a **Table** (not an SDT)

## CodeHeader reference (cross-class)

Most FlexScript-expression-mode variables on PF activities use one of
these headers. The exact header per-variable is documented in the
relevant activity doc.

### Standard PF codeHeader (Delay, AssignLabels, CustomCode, CreateObject.destination/quantity, EventTriggeredSource.assignObjectTo, etc.)

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
treenode processFlow = ownerobject(activity);
```

### Pre-token codeHeader (InterArrivalSource.interArrivalTime — runs before the token exists)

```
Object current = param(1);
treenode activity = param(2);
treenode processFlow = ownerobject(activity);
```

### CreateObject.quantity codeHeader (extra `item` param)

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
Object item = param(4);
treenode processFlow = ownerobject(activity);
```

### ScheduleSource label cells codeHeader (richer: row, repeat, token-index, labelName)

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
int rowNumber = param(4);
int repeatCount = param(5);
int tokenIndex = param(6);
string labelName = param(7);
treenode processFlow = ownerobject(activity);
```

## Universal don'ts

- **Never set `next` or `prev`** on any activity — use `modelerai_connect_activities` to wire flows.
- **Never set `container` or `lockedToMe`** — UI state only.
- **`fontName`/`fontSize`/`fontStyle`/`fontColor`/`showLinks`/`traceOption`/`traceRequirement`** — visual cosmetics; rarely worth changing.

## Open follow-ups affecting batch 1

- **SDT writer** — `set_activity_variable` cannot add/edit SDT entries. Blocks: EventTriggeredSource's `events` rows, CreateObject's `assignTo` token-label mode.
- **Table writer** — same blocker for ScheduleSource's `arrivals` cells (different API: Table not SDT).
- **Event Data enumeration** — for each (FixedResource class, event), what Event Data slots does the per-event `labelNames` SDT expose? Required to safely add an EventTriggeredSource subscription via the agent.
