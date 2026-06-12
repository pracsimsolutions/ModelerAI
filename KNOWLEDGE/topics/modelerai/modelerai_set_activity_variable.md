# modelerai_set_activity_variable

Sets a variable on an activity within a named ProcessFlow. Covers the value
kinds used by ProcessFlow activities: numeric, string, activity reference
(same PF), ProcessFlow reference (flowRef pattern for sub-flows / Person flows),
model-object coupling, and flexscript.

## Args

```jsonc
{
  "processflow": "MainFlow",        // required — PF name
  "activity":    "Source1",         // required — activity name within that PF
  "variable":    "arrivalSpacing",  // required — variable name on the activity
  "value":       0                  // required — see Value kinds below
}
```

### Value kinds

| `value` shape | FlexScript equivalent | `value_kind` returned |
|---|---|---|
| `30` (number) | `setvarnum(activity, varName, 30)` | `"number"` |
| `"assembly"` (string) | `getvarnode(activity, varName).value = "assembly"` | `"string"` |
| `{ "ref": "OtherActivity" }` | `getvarnode(activity, varName).value = otherActivity` | `"ref"` |
| `{ "ref_pf": "OtherFlow" }` | `getvarnode(activity, varName).value = otherFlow` | `"ref_pf"` |
| `{ "model_object": "Queue1" }` | couples the var node to the model object `Queue1` | `"model_object"` |
| `{ "flexscript": "return ...;" }` | marks the node flexscript and prepends the codeHeader | `"flexscript"` |

`"ref"` looks up the target by name within the **same** ProcessFlow.

`"ref_pf"` looks up the target by name in the `/Tools/ProcessFlow` storage list
— the canonical pattern for setting `flowRef` on a CreatePerson or
RunSubFlow activity.

`"model_object"` stores a coupling pointer to a specific existing model object
(resolved by name). Use this for destination / objectRef / assignTo style
variables instead of passing a raw string name.

`"flexscript"` marks the variable node as flexscript and auto-prepends the
appropriate `codeHeader`. An optional `header` field on the value object picks
the header variant: `"standard"` (default), `"pre_token"`,
`"create_object_quantity"`, or `"schedule_label"`. Example:
`{ "flexscript": "return token.quantity;", "header": "create_object_quantity" }`.

## Returns

```jsonc
{ "ok": true, "activity": "Source1", "variable": "arrivalSpacing",
  "value_kind": "number", "processflow": "MainFlow" }
```

## Errors

| error_code | Meaning |
|---|---|
| `processflow_not_found` | No PF with that name in /Tools/ProcessFlow |
| `activity_not_found` | Activity not found inside the named ProcessFlow |
| `ref_not_found` | `value.ref` activity not found in the same ProcessFlow |
| `ref_pf_not_found` | `value.ref_pf` ProcessFlow not found in /Tools/ProcessFlow |
| `missing_args` | Required field missing |
| `bad_value` | `value` is not a number, string, or recognized object shape |

## Examples

```jsonc
// Set numeric inter-arrival time
modelerai_call({ name: "modelerai_set_activity_variable",
  args: { processflow: "MainFlow", activity: "Source1",
          variable: "arrivalSpacing", value: 30 } })

// Set string label
modelerai_call({ name: "modelerai_set_activity_variable",
  args: { processflow: "MainFlow", activity: "Assign1",
          variable: "labelName", value: "priority" } })

// Point a RunSubFlow at another activity in the same PF
modelerai_call({ name: "modelerai_set_activity_variable",
  args: { processflow: "MainFlow", activity: "RunSubFlow1",
          variable: "subFlow", value: { "ref": "SubFlowEntry" } } })

// flowRef pattern — point a CreatePerson at a separate Person flow
modelerai_call({ name: "modelerai_set_activity_variable",
  args: { processflow: "PersonSource", activity: "CreatePerson1",
          variable: "flowRef", value: { "ref_pf": "MainPersonFlow" } } })
```
