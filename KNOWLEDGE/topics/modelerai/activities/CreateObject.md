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

**Token-label mode** (most common: store the new object on a token
label)
- Requires a special node structure copied from the library template.
- Copy/replace from:
  `maintree().find("project/library/processflow/activities/CreateObject>variables/assignTo")`
- The second subnode of `assignTo` is named `stringValue` — set it to
  the label name you want to assert (e.g. `"createdItem"`).
- Then `assignType` (above) chooses overwrite vs append.

**Arbitrary-label mode** (write to any label on any model object)
- FlexScript-expression that returns a label node directly:
  ```
  return Model.find("Queue1").labels["createdItems"];
  ```
- Same standard PF codeHeader.

**TODO:** the curated `set_activity_variable` does NOT yet handle the
copy/replace pattern for token-label mode. For now, this needs manual
configuration in the FlexSim UI, OR the agent uses the
arbitrary-label-mode FlexScript form (which the tool can write as a
plain string).

### Inherited / visual — DANGEROUS or USELESS

Same policy. **Never set `next` or `prev`.** See [Delay topic](Delay.md).

## Common patterns

```jsonc
// Spawn 1 Box into Plane1 per token arrival, no assignment
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "objectRef", value: 1  // Box, assuming default FlowItemBin order
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "destination", value: "return Model.find(\"Plane1\");"
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "quantity", value: 1
}

// Spawn N tokens-worth of pallets, assign created item to a Queue1 label
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "objectRef", value: 4  // Pallet
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "quantity", value: "return token.labels[\"orderSize\"].value;"
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "assignTo", value: "return Model.find(\"Queue1\").labels[\"recent\"];"
}
modelerai_set_activity_variable {
  processflow: "MainFlow", activity: "CreateObject1",
  variable: "assignType", value: 1  // append
}
```

## Gotchas

- **`objectRef` is an INDEX, not a class name.** "Box" is `1` (in the
  default ordering), not `"Box"`. Customized FlowItemBin orderings will
  shift the mapping — verify before setting.
- **`assignTo` token-label mode needs a special node structure.** Setting
  the variable to a plain string in token-label mode will NOT work; the
  tool needs an extension to handle the copy-replace pattern, OR the
  agent uses the FlexScript-expression form returning a label node.
- **`createAt` controls parent-vs-place semantics** but isn't in the
  `>variables` sweep — its location in the tree is still TODO. Until
  enumerated, modify via UI.
- **`quantity` FlexScript header has an extra param** (`Object item =
  param(4);`) the other source-y vars don't — don't redeclare.
