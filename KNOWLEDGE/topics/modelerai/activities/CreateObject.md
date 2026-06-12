---
title: CreateObject activity — variable semantics
status: v1 — hand-curated 2026-06-11 (batch 1 of activity deep-dive)
---

# CreateObject

**What it does:** Spawns one or more FlexSim objects (flow items) into
the model on token arrival. Used to bring new items into the world from
a process flow — e.g. an order activity creating a Box at a warehouse
pick face.

## Variables

### `objectRef` (dataType 1 → number) — SEMANTIC

**An index into `/Tools/FlowItemBin/`** — the number selects which
flow-item class to instantiate. NOT a treenode pointer.

The indexing follows the order of children under `/Tools/FlowItemBin/`.
A clean model's default ordering:

| Index | Class |
|---|---|
| 1 | Box |
| 2 | Cylinder |
| 3 | Sphere |
| 4 | Pallet |
| 5 | Tote |
| 6 | Man (Operator) |
| 7 | Woman (Operator) |
| 8 | Truck (Operator) |
| 9 | Test Tube |
| 10 | Pill Bottle |
| 11 | Clipboard |

The order can be customized in the modeler's FlowItemBin, so don't
hard-code the indices for production work — verify against the actual
`/Tools/FlowItemBin/` contents first.

### `quantity` (dataType 1 → number, dual-mode) — SEMANTIC

Number of objects to spawn per token arrival. **Dual storage mode**
(plain number OR FlexScript expression). Extended codeHeader includes
`Object item = param(4);` — the newly-created item context:

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
Object item = param(4);
treenode processFlow = ownerobject(activity);
```

Example values:
- `1` (literal — spawn one per arrival)
- `return token.labels["batchSize"].value;` (driven by a token label)

### `destination` (dataType 2 → string, dual-mode) — SEMANTIC

WHERE the new object is placed. Storage modes:

**Pointer mode:** direct treenode coupling to a container object.

**FlexScript-expression mode:** standard PF codeHeader; body must
`return` an Object/treenode (the parent container):

```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
treenode processFlow = ownerobject(activity);
```

Example bodies:
- `return Model.find("Plane1");` — drop into a plane
- `return current;` — into whatever Object the activity's PF is attached to
- `return token.labels["loc"].value;` — token-driven destination

**Related: `createAt` (TODO — not yet enumerated in the >variables sweep
but documented as a sibling control)**
- `0` → "Create In" — object becomes a child of `destination`
- `1` → "Create At" — object is placed AT the location of `destination`
  but at the model root, not as a child

Need to walk a configured CreateObject instance to confirm where
`createAt` lives (nested container? hidden subnode?).

### `assignType` (dataType 1 → number, 0/1) — SEMANTIC

Controls how `assignTo` writes to its target:
- `0` → **Overwrite** the existing value
- `1` → **Append / "Insert at Front of"** — the existing value becomes a
  list with the new item prepended

### `assignTo` (dataType 7 → SIMPLE — polymorphic) — SEMANTIC, but unusual

DATATYPE_SIMPLE: can hold a coupling pointer, a literal, OR a FlexScript
expression depending on what's assigned. Selects WHERE the new-object
reference gets stored. Two distinct usage patterns exposed in the UI:

**Token-label mode** (most common: store the new object on a token label)
- Use the dedicated tool — it does the library-template copy/replace +
  `stringValue` + `assignType` internally:
  ```jsonc
  modelerai_set_create_object_target_label {
    processflow: "MainFlow", activity: "CreateObject1",
    label_name: "createdItem",   // the token label the new object's ref goes into
    append: false                // false=overwrite (default), true=append to a list
  }
  ```

**Arbitrary-label mode** (write to any label on any model object)
- `set_activity_variable` on `assignTo` with a FlexScript body returning a
  label node:
  ```jsonc
  modelerai_set_activity_variable {
    processflow: "MainFlow", activity: "CreateObject1", variable: "assignTo",
    value: { "flexscript": "return Model.find(\"Queue1\").labels[\"createdItems\"];" }
  }
  ```

### "Assign Labels to Created Objects" — a label table on the NEW OBJECTS

Distinct from `assignTo` (which stores the object reference onto the token).
This is a Name/Value table that puts labels **on each created item**. If
`quantity > 1`, the label is applied to ALL created items in the batch.

It's a table at the hidden subnode `>labels` (same name as AssignLabels):
col 1 = label name, col 2 = value. Write it with the activity-table tools,
passing `variable: ">labels"` — no new tooling needed:
```jsonc
// give every created item a label "batch" = its position in the batch
modelerai_set_activity_table_cell { processflow, activity: "CreateObject1",
  variable: ">labels", row: 1, col: 1, value: "batch" }
modelerai_set_activity_table_cell { processflow, activity: "CreateObject1",
  variable: ">labels", row: 1, col: 2,
  value: { "flexscript": "return createdrank;", "header": "create_object_label" } }
```

**Value-cell codeHeader** — use `header: "create_object_label"` when a value
cell is FlexScript. It binds:
```
Object current = param(1);
treenode activity = param(2);
Token token = param(3);
Object item = param(4);       // the item being labeled
Variant assignTo = item;      // convenience alias
int createdrank = param(5);   // 1-based index of this item within the batch
string labelName = param(6);  // the label name (col 1 of this row)
treenode processFlow = ownerobject(activity);
```
Example (label each created item with its batch position):
```jsonc
modelerai_set_activity_table_cell {
  processflow, activity: "CreateObject1", variable: "<labels-table>",
  row: 1, col: 2,
  value: { "flexscript": "return createdrank;", "header": "create_object_label" }
}
```

### Inherited / visual — DANGEROUS or USELESS

Same policy. **Never set `next` or `prev`.** See [Delay topic](Delay.md).

## Common patterns

```jsonc
// Spawn 1 Box into Plane1 per token arrival, no assignment
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "objectRef", value: 1  // Box (index into Tools/FlowItemBin)
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "destination", value: { "model_object": "Plane1" }  // coupling pointer
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "quantity", value: 1
}

// Spawn token-driven count of pallets, store created item on a token label
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "objectRef", value: 4  // Pallet
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "quantity",
  value: { "flexscript": "return token.labels[\"orderSize\"].value;",
           "header": "create_object_quantity" }  // quantity body has the extra `item` param
}
modelerai_set_create_object_target_label {
  processflow: "MainFlow", activity: "CreateObject1",
  label_name: "createdItem", append: false
}
```

## Gotchas

- **`objectRef` is an INDEX, not a class name.** "Box" is `1` (in the
  default ordering), not `"Box"`. Customized FlowItemBin orderings will
  shift the mapping — verify before setting.
- **`assignTo` token-label mode:** use `modelerai_set_create_object_target_label`
  (handles the library-template copy/replace + stringValue + assignType for
  you). For arbitrary model-object labels, use `set_activity_variable` on
  `assignTo` with `{flexscript:"return Model.find(\"X\").labels[\"y\"];"}`.
- **`createAt` controls parent-vs-place semantics** but isn't in the
  `>variables` sweep — its location in the tree is still TODO. Until
  enumerated, modify via UI.
- **`quantity` FlexScript header has an extra param** (`Object item =
  param(4);`) the other source-y vars don't — don't redeclare.
