# Lists — create & configure (`create_list` / `add_list_field` / `list_premade_fields` / `set_list_initial_content` / `get_list_info`)

FlexSim **global Lists** (under `Tools/GlobalLists`) hold values/objects that get pushed and pulled by resource logic. These five tools **create and configure** a List from scratch — no `run_script`. (Runtime **push/pull** is *not* here: use a ProcessFlow **Push To List** / **Pull From List** activity, or `run_script` with `listpush`/`listpull`.)

## `modelerai_create_list`
```json
{ "name": "AvailableOps",
  "fields": [ <field-spec>, ... ],          // optional — inline, same shape as add_list_field (minus "list")
  "initial_content": [ "Obj"|"Group", ... ] // optional — objects/groups pushed on reset
}
→ { ok, list, existing, path, initial_content?, field_errors?, unresolved? }
```
`Tools.create("List")`; **idempotent on name** (returns the existing list, `existing:true`). Inline `fields`/`initial_content` apply the same logic as the standalone tools, so a whole list can be built in one call.

## `modelerai_add_list_field`
```json
{ "list": "AvailableOps", "source": "expression" | "label" | "premade",
  // source=expression: "name" (column), "expression" (FlexScript body), "dynamic"?
  // source=label:      "name" (= the label to read off the pushed value), "dynamic"?
  // source=premade:    "category", "premade_name", "name"? (rename), "dynamic"? (override)
}
→ { ok, list, field, type, dynamic }
```
- **expression** — any FlexScript; the field value = its return. In scope: `value` = param(1) (the pushed value), `puller` = param(2), `entry` = param(3), `pushTime` = param(4). (The tool prepends that header; you supply just the body, e.g. `"return Model.time - pushTime;"`.)
- **label** — the field **name** *is* the label name read off each pushed value. No expression.
- **premade** — copies a template from the catalog (see below). Bad `category`/`premade_name` returns `{ error:"premade_not_found", available:[...] }`.
- `dynamic:true` → the field is re-evaluated per pull query / per puller (needed when the field references `puller` or live state); `false` → evaluated once at push time.

## `modelerai_list_premade_fields`  *(read-only)*
```json
{ }
→ { ok, categories: { Item:[...], FixedResource:[...], TaskExecuter:[...],
                      TaskSequence:[...], Token:[...], CPConnection:[...] } }
```
The premade catalog lives on the List **class**, keyed by *what the list holds*. Examples: `Item` → `Type`, `age`, `queueSize`, `ageInQueue`, `meetsPullRequirement`; `TaskExecuter` → `utilization`, `totalTravel`, `pullerPriority`; `Token` → `tokenID`, `activity`, `parent`. Use a `category` + `premade_name` from here with `add_list_field` `source:"premade"`.

## `modelerai_set_list_initial_content`
```json
{ "list": "AvailableOps", "objects": [ "Operator1", "Operators" ], "replace"?: true }
→ { ok, list, initial_content: [...] }
```
Each name resolves as a model object, else a **Group** (`Tools/Groups/<name>`). Stored as `contentsOnReset` name→pointer entries; pushed onto the list on model **reset**. **Atomic:** if any name doesn't resolve, nothing is written and you get `{ error:"object_not_found", unresolved:[...] }`. `replace:true` clears existing reset-content first (default appends).

## `modelerai_get_list_info`  *(read-only)*
```json
{ "list": "AvailableOps" }
→ { ok, name, path, field_count,
    fields: [ { name, type:"expression"|"label", dynamic, expression? } ],
    initial_content: [ "Operator1", ... ] }
```
`type` is classified by the node's structure (has an `expression` subnode → expression, else label) — so a premade field reports its *actual* type, which may be `label`.

## Worked example
```
1. create_list { name:"AvailableOps" }
2. add_list_field { list:"AvailableOps", source:"label", name:"resType" }
3. add_list_field { list:"AvailableOps", source:"premade", category:"TaskExecuter", premade_name:"utilization" }
4. add_list_field { list:"AvailableOps", source:"expression", name:"idleAge",
                    expression:"return Model.time - pushTime;", dynamic:true }
5. set_list_initial_content { list:"AvailableOps", objects:["Operators"] }   // a Group
6. get_list_info { list:"AvailableOps" }   // verify
```
Or all at once: `create_list { name:"AvailableOps", fields:[{source:"label",name:"resType"}, {source:"premade",category:"TaskExecuter",premade_name:"utilization"}], initial_content:["Operators"] }`.

## Not yet (later phases)
General-tab options (unique values/pullers, partitioning, back-order strategy), Group / Push-Argument field types, the Back Orders tab, and runtime push/pull from code. Field config covers Expression / Label / premade only.
