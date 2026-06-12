# modelerai_list_processflows

Walks `/Tools/ProcessFlow` (the flat storage list) and returns metadata for
every ProcessFlow registered in the model. Read-only — does not evaluate any
FlexScript.

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
    { "name": "MainFlow",  "kind": "general",  "path": "/Tools/ProcessFlow/MainFlow" },
    { "name": "SubFlow1",  "kind": "sub_flow", "path": "/Tools/ProcessFlow/SubFlow1" }
  ]
}
```

Each entry is `{ name, kind, path }`. There is no `activity_count` field — use
`modelerai_list_activities` on a specific PF for its activities.

Duplicate-named PFs disambiguate with a `~N` path-tail suffix in `name`
(`MainFlow`, `MainFlow~2`); pass that form verbatim to other tools.

## Kind

`kind` is detected from the PF node: `sub_flow` (isSubFlow), `person`
(isPersonFlow), `object` (has a non-self `attachedObject`), else `general`.
The `general` branch is verified; the others mirror `delete_processflow`'s
detection.

## Example

```jsonc
modelerai_call({ name: "modelerai_list_processflows", args: {} })
```
