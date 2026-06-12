# modelerai_get_activity_variable

Reads a single variable on an activity in a named ProcessFlow. Symmetric
with [`modelerai_set_activity_variable`](modelerai_set_activity_variable.md).
Read-only.

The variable's *kind* is inferred from its underlying node datatype:

| Node datatype | Returned `kind` | `value` shape |
|---|---|---|
| `DATATYPE_NUMBER` (1) | `"number"` | `42.0` |
| `DATATYPE_STRING` (2) | `"string"` | `"some expression"` |
| `DATATYPE_NODE`   (3) → sibling activity in this PF | `"ref"` | `{ "ref": "OtherActivity" }` |
| `DATATYPE_NODE`   (3) → another PF | `"ref_pf"` | `{ "ref_pf": "OtherFlow" }` |
| `DATATYPE_NODE`   (3) → some other treenode | `"node"` | `"/path/to/node"` |
| `7` (SIMPLE — polymorphic; e.g. `assignTo`, `objectRef`, `destination`) | `"node"` (or best-effort) | `"/path/to/node"` (best-effort) |
| anything else | `"unknown"` | best-effort string |

dataType-7 SIMPLE is the most common polymorphic PF variable type. Reading it
returns kind `"node"` (or a best-effort representation) since the underlying
value may be a coupling, an expression, or a node. To **set** a SIMPLE variable,
use `modelerai_set_activity_variable` with `{ "model_object": "Queue1" }`,
`{ "flexscript": "return ...;" }`, or `{ "ref": "OtherActivity" }` depending on
what the variable should point at.

## Args

```jsonc
{
  "processflow": "MainFlow",        // required
  "activity":    "Source1",         // required — name within the PF
  "variable":    "arrivalSpacing"   // required — variable name
}
```

## Returns

```jsonc
// Numeric
{ "ok": true, "processflow": "MainFlow", "activity": "Source1",
  "variable": "arrivalSpacing", "kind": "number", "value": 30.0 }

// String
{ "ok": true, "processflow": "MainFlow", "activity": "Delay1",
  "variable": "label", "kind": "string", "value": "assembly" }

// Reference to a sibling activity in the same PF
{ "ok": true, "processflow": "MainFlow", "activity": "RunSubFlow1",
  "variable": "subFlow", "kind": "ref",
  "value": { "ref": "SubFlowActivity" } }

// Reference to another ProcessFlow
{ "ok": true, "processflow": "PersonSource", "activity": "CreatePerson1",
  "variable": "flowRef", "kind": "ref_pf",
  "value": { "ref_pf": "MainPersonFlow" } }
```

## Errors

| error_code | meaning |
|---|---|
| `processflow_not_found` | no PF named `<processflow>` at `/Tools/ProcessFlow/` |
| `activity_not_found` | PF exists but contains no activity by that name |
| `variable_not_found` | activity exists but `getvarnode(activity, "<name>")` returned null |
| `missing_args` / `bad_args_json` | malformed input |

## Discovery flow

1. `modelerai_list_activities { processflow }` → names + classes.
2. Read the KB topic
   [`processflow-activity-variables.md`](processflow-activity-variables.md) for
   the valid variable names of that activity class. (`get_activity_info` does
   NOT return a variable list.)
3. `modelerai_get_activity_variable { processflow, activity, variable }` →
   one value at a time, with kind.

## Example

```jsonc
modelerai_call({
  name: "modelerai_get_activity_variable",
  args: { processflow: "MainFlow", activity: "Source1",
          variable: "arrivalSpacing" }
})
```
