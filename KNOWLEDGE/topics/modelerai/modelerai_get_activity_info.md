# modelerai_get_activity_info

Returns descriptive info about a single activity in a named ProcessFlow:
its `name`, `class`, and full tree `path`. Read-only.

This tool does NOT return a list of variables. To discover valid variable names
for an activity class, read the KB topic
[`processflow-activity-variables.md`](processflow-activity-variables.md).

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
  "path": "/Tools/ProcessFlow/MainFlow/Source1"
}
```

The return contains only `name`, `class`, and `path` (plus `ok` and
`processflow`). There is no `variables` array.

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

- For inspection / debugging: confirm an activity's class + path before
  downstream calls.
- To discover which variables an activity class supports, do NOT call this tool
  — read the KB topic
  [`processflow-activity-variables.md`](processflow-activity-variables.md)
  for valid variable names, then read current values with
  `modelerai_get_activity_variable`.
