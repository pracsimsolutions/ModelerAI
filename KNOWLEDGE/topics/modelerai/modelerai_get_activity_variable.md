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
| anything else | `"unknown"` | best-effort string |

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
2. `modelerai_get_activity_info { processflow, activity }` → variable name list.
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
