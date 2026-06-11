# modelerai_list_processflows

Walks `Tools/Toolbox/ProcessFlow/*` and returns metadata for every ProcessFlow
registered in the model. Read-only — does not evaluate any FlexScript.

## Args

```jsonc
{}   // no filters in v1
```

## Returns

```jsonc
{
  "ok": true,
  "count": 2,
  "processflows": [
    { "name": "MainFlow",  "kind": "general",  "path": "Tools/…/MainFlow",  "activity_count": 5 },
    { "name": "SubFlow1",  "kind": "sub_flow", "path": "Tools/…/SubFlow1",  "activity_count": 3 }
  ]
}
```

## Kind inference

Inferred from the parent toolbox folder name:

| Folder | kind |
|---|---|
| `"General"` | `"general"` |
| `"Object"` | `"object"` |
| `"Sub Flow"` | `"sub_flow"` |
| `"Person Flow"` | `"person"` |
| anything else | `"unknown"` |

`activity_count` is the raw subnode count of the ProcessFlow node (v1 — total,
not filtered to activity types).

## Example

```jsonc
modelerai_call({ name: "modelerai_list_processflows", args: {} })
```
