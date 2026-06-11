# modelerai_create_processflow

Creates a ProcessFlow tool of the requested kind and registers it under the
model's Tools/Toolbox/ProcessFlow tree.

## Args

```jsonc
{
  "kind":        "general" | "object" | "sub_flow" | "person",  // required
  "name":        "MyFlow",          // optional — setname() after creation
  "category":    "General",         // optional — PFTypes folder; defaults by kind
  "template":    "MAIN:/library/…", // optional — load from a template node
  "attached_to": "Processor1",      // optional — attach target (kind "object" only)
  "open_view":   true               // optional — default true
}
```

## Returns

```jsonc
{ "ok": true, "kind": "general", "name": "MyFlow", "path": "Tools/…/MyFlow" }
// kind "object" with attached_to:
{ "ok": true, "kind": "object", "name": "MyFlow", "path": "…", "attached_to": "Processor1" }
```

## Kind details

| kind | PF_TYPE_* constant | Default category | Auto-attach |
|---|---|---|---|
| `general` | `PF_TYPE_GENERAL` | `"General"` | self-attaches |
| `object` | `PF_TYPE_OBJECT` | `"Object"` | attaches to `attached_to` if given |
| `sub_flow` | `PF_TYPE_SUB_FLOW` | `"Sub Flow"` | none |
| `person` | n/a (`addPersonFlow`) | n/a | n/a |

## Example

```jsonc
modelerai_call({
  name: "modelerai_create_processflow",
  args: { kind: "general", name: "MainFlow" }
})
```

## Errors

| error_code | Meaning |
|---|---|
| `bad_kind` | `kind` is not one of the four valid values |
| `create_failed` | FlexScript returned no treenode (module not loaded, model limit, bad template path) |
| `bad_args_json` | JSON parse failed |
