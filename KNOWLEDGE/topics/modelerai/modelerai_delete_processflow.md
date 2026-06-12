# modelerai_delete_processflow

Finds a ProcessFlow by name in the `/Tools/ProcessFlow` storage list,
destroys the ProcessFlow node, and removes its toolbox coupling entry.

**Safety:** only destroys nodes that live inside the `/Tools/ProcessFlow`
storage list — will not accidentally destroy arbitrary model objects even if a
name collision exists elsewhere.

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
{ "ok": true, "name": "MyFlow", "kind": "unknown", "removed": true }
```

`kind` comes from a best-effort detection and may be `"unknown"`.

## Errors

| error_code | Meaning |
|---|---|
| `not_found` | No ProcessFlow with that name in /Tools/ProcessFlow |
| `missing_name` | No name arg provided |
| `delete_pf_failed` | FlexScript execution threw an exception |

## Example

```jsonc
modelerai_call({ name: "modelerai_delete_processflow", args: { name: "TestFlow" } })
```
