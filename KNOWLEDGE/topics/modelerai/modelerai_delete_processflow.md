# modelerai_delete_processflow

Finds a ProcessFlow by name under any `Tools/Toolbox/ProcessFlow/*` folder,
destroys the ProcessFlow node, and removes its toolbox coupling entry.

**Safety:** only destroys nodes that live inside the
`Tools/Toolbox/ProcessFlow` subtree — will not accidentally destroy arbitrary
model objects even if a name collision exists elsewhere.

**v1 limitation:** does not chase RunSubFlow activity references before
deleting. If other ProcessFlows call this one via a RunSubFlow activity, those
references will dangle. FlexSim surfaces dangling references at run time.
Warn the modeler when deleting a `sub_flow` that may be referenced elsewhere.

## Args

```jsonc
{ "name": "MyFlow" }
// or bare string:
"MyFlow"
```

## Returns

```jsonc
{ "ok": true, "name": "MyFlow", "kind": "sub_flow", "removed": true }
```

## Errors

| error_code | Meaning |
|---|---|
| `not_found` | No ProcessFlow with that name under Tools/Toolbox/ProcessFlow |
| `missing_name` | No name arg provided |
| `delete_pf_failed` | FlexScript execution threw an exception |

## Example

```jsonc
modelerai_call({ name: "modelerai_delete_processflow", args: { name: "TestFlow" } })
```
