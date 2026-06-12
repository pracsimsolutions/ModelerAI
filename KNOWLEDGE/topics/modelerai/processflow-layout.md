---
title: ProcessFlow activity layout conventions
status: v1 — captured 2026-06-11 (modeler-provided dimensions)
---

# ProcessFlow activity layout

How to position activities in the ProcessFlowView so they render
cleanly.

## Activity block dimensions

Each activity renders as a fixed-size block in the view:

| Property | Value |
|---|---|
| Width   | **140** units |
| Height  | **20** units |

These are the visual dimensions, not the tree-storage geometry. When
positioning with `modelerai_add_activity {position: [x, y]}`, the `[x, y]`
is the **top-left corner** of the block.

## Flow direction

ProcessFlows read like text:

- **Left → right** for sequential flow
- **Top → bottom** for new rows (line wrap or parallel flows)

## Default spacing

**20 units** of gap between activities in any direction. So the
center-to-center distance between two adjacent activities is:

- Horizontal: `140 (block) + 20 (gap) = 160` units
- Vertical:   `20 (block) + 20 (gap) = 40` units

## Layout formulas

For a sequence `A → B → C → ...` placed left to right at row `y`:

```
A.position = [x0, y]
B.position = [x0 + 160, y]
C.position = [x0 + 320, y]
...
N.position = [x0 + (N-1) * 160, y]
```

For a second parallel flow below the first:

```
flow2_y = flow1_y + 40
```

For a flow that wraps (e.g. fits 6 activities per row, then continues):

```
i-th activity (0-indexed) in a wrap-W row:
  row = i / W   (integer division)
  col = i % W
  position = [x0 + col * 160, y0 + row * 40]
```

## Standard column origins

When laying out from scratch, a reasonable origin is `[0, 0]` for the
first row. The first activity drops at `[0, 0]`, next at `[160, 0]`,
etc.

## Sample build

A two-flow PF with three activities each:

```
Flow1 (y = 0):
  Source1       → position [0,    0]
  Delay1        → position [160,  0]
  CreateObject1 → position [320,  0]

Flow2 (y = 40):
  Source2       → position [0,    40]
  AssignLabels1 → position [160,  40]
  Sink2         → position [320,  40]
```

Connectors between adjacent same-row activities cross the 20-unit gap
cleanly. The 40-unit vertical separation between flows leaves room for
the connector arrowheads without crowding.

## See also

- [`modelerai_add_activity`](modelerai_add_activity.md) — takes
  `position:[x, y]` as an optional arg
- [`modelerai_connect_activities`](modelerai_connect_activities.md) —
  wires two existing activities (positions don't affect the connector
  semantics, only the visual routing)
