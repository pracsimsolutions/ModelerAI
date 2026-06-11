# modelerai_get_activity_info

Returns descriptive info about a single activity in a named ProcessFlow:
its class, full tree path, and the list of variable names defined on it.
Read-only.

Reads from `/Tools/ProcessFlow/<name>` storage — same path discipline as
every other PF tool.

## Args

```jsonc
{
  "processflow": "MainFlow",   // required
  "activity":    "Source1"     // required
}
```

## Returns

```jsonc
{
  "ok": true,
  "processflow": "MainFlow",
  "activity": "Source1",
  "class": "InterArrivalSource",
  "path": "/Tools/ProcessFlow/MainFlow/Source1",
  "variables": [
    "arrivalSpacing",
    "flowItemClass",
    "labelData",
    "..."
  ]
}
```

`variables` is enumerated from the activity's `variables` subnode container.
An activity with no such container reports an empty array.

## Errors

| error_code | meaning |
|---|---|
| `processflow_not_found` | no PF named `<processflow>` at `/Tools/ProcessFlow/` |
| `activity_not_found` | PF exists but contains no activity by that name |
| `missing_args` / `bad_args_json` | malformed input |

## Example

```jsonc
modelerai_call({ name: "modelerai_get_activity_info",
                 args: { processflow: "MainFlow", activity: "Source1" } })
```

## When to use

- BEFORE `modelerai_set_activity_variable` when you don't know the variable
  names for that activity class. The `variables` list tells you what's
  writable. (Read the actual current value with
  `modelerai_get_activity_variable`.)
- For inspection / debugging: confirm class + path before downstream calls.
