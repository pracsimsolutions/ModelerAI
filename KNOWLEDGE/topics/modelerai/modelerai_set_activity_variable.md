# modelerai_set_activity_variable

Sets a variable on an activity within a named ProcessFlow. Covers all four
value kinds used by ProcessFlow activities: numeric, string, activity reference
(same PF), and ProcessFlow reference (flowRef pattern for sub-flows /
Person flows).

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

`"ref"` looks up the target by name within the **same** ProcessFlow.

`"ref_pf"` looks up the target by name under any `Tools/Toolbox/ProcessFlow`
folder — the canonical pattern for setting `flowRef` on a CreatePerson or
RunSubFlow activity.

## Returns

```jsonc
{ "ok": true, "activity": "Source1", "variable": "arrivalSpacing",
  "value_kind": "number", "processflow": "MainFlow" }
```

## Errors

| error_code | Meaning |
|---|---|
| `processflow_not_found` | No PF with that name under Tools/Toolbox/ProcessFlow |
| `activity_not_found` | Activity not found inside the named ProcessFlow |
| `ref_not_found` | `value.ref` activity not found in the same ProcessFlow |
| `ref_pf_not_found` | `value.ref_pf` ProcessFlow not found under the toolbox |
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
