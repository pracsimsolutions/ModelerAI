# modelerai_list_activities

Lists every activity inside a named ProcessFlow. Read-only.

Reads from the storage location `/Tools/ProcessFlow/<name>` — **not** from
the toolbox coupling subtree. The toolbox path is a UI shortcut; the storage
path is the authoritative tree location for ProcessFlows.

## Args

```jsonc
// Object form
{ "processflow": "MainFlow" }

// Bare string also accepted
"MainFlow"
```

## Returns

```jsonc
{
  "ok": true,
  "processflow": "MainFlow",
  "count": 3,
  "activities": [
    { "name": "Source1", "class": "InterArrivalSource",
      "path": "/Tools/ProcessFlow/MainFlow/Source1" },
    { "name": "Delay1",  "class": "Delay",
      "path": "/Tools/ProcessFlow/MainFlow/Delay1" },
    { "name": "Sink1",   "class": "Sink",
      "path": "/Tools/ProcessFlow/MainFlow/Sink1" }
  ]
}
```

Activities are filtered by `getclassname(node) != ""`; bookkeeping subnodes
(variables, connectors, view state) are skipped.

## Errors

| error_code | meaning |
|---|---|
| `processflow_not_found` | no PF named `<processflow>` at `/Tools/ProcessFlow/` (returns empty `activities` array, not an error in current build — TBD) |
| `missing_args` | no `processflow` supplied |

## Example

```jsonc
modelerai_call({ name: "modelerai_list_activities",
                 args: { processflow: "MainFlow" } })
```

## When to use

- BEFORE `modelerai_connect_activities` to confirm both endpoints exist.
- BEFORE `modelerai_set_activity_variable` to confirm the target activity name.
- For verification after `modelerai_add_activity` — does the activity actually
  show up in the PF?
