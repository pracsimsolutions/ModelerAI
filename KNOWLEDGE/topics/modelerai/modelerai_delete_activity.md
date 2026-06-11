# modelerai_delete_activity

Finds a named activity inside a named ProcessFlow and destroys it.

Both `activity` and `processflow` are required because activity names are only
unique within a ProcessFlow, not globally across the model.

**v1 limitation:** does not explicitly clean up connectors that pointed at the
deleted activity. FlexSim's connector destruction follows the activity destroy
in practice, but if dangling connectors remain they will surface at model
run time or in validation.

## Args

```jsonc
{ "processflow": "MainFlow", "activity": "Delay1" }
```

## Returns

```jsonc
{ "ok": true, "name": "Delay1", "processflow": "MainFlow", "removed": true }
```

## Errors

| error_code | Meaning |
|---|---|
| `processflow_not_found` | No PF with that name under Tools/Toolbox/ProcessFlow |
| `activity_not_found` | No activity with that name inside the named ProcessFlow |
| `missing_args` | `processflow` or `activity` not provided |
| `delete_activity_failed` | FlexScript execution threw an exception |

## Example

```jsonc
modelerai_call({
  name: "modelerai_delete_activity",
  args: { processflow: "MainFlow", activity: "TempDelay" }
})
```
