---
title: modelerai_list_triggers
status: v1 — class-declared triggers + per-instance "is set" flag
---

# `modelerai_list_triggers`

Index of every trigger declared on a FlexSim object's class, with the per-instance "currently set" flag and the canonical `code_header` / named parameters the modeler/AI would write against.

## When to use

- First call when the modeler asks "what triggers does X have?" — much cheaper than calling `get_trigger` once per name to discover the surface.
- Discovery before writing a trigger: confirm the exact name (`OnExit` vs `OnExited`), pull the param signature, and only then call [`modelerai_set_trigger`](modelerai_set_trigger.md) with a known-good name.
- Audit: scan `set: true` entries to summarize "what custom logic is on this object" without paging the actual code bodies.

The full code body is NOT in this response. Once you've picked an interesting entry, call [`modelerai_get_trigger`](modelerai_get_trigger.md) for that one trigger to read its `user_code`.

## Arguments

```json
{ "object": "Source1" }
```

Or — for a one-liner — just the object name as a raw string is accepted too:

```
"Source1"
```

`object` is resolved via `Model.find`. Pathed names (e.g. `Sources/Source1`) are accepted.

## Returns

```json
{
  "ok":     true,
  "object": "Source1",
  "count":  19,
  "triggers": [
    {
      "name":         "OnCreation",
      "code_header":  "Object current = ownerobject(c);\nObject item = param(1);\nint rownumber = param(2);\n",
      "params": [
        { "title": "Created Item", "name": "item",      "type": "Object" },
        { "title": "Row Number",   "name": "rownumber", "type": "int"    }
      ],
      "set":          false,
      "storage_path": ""
    },
    {
      "name":         "OnExit",
      "code_header":  "Object current = ownerobject(c);\nObject item = param(1);\nint port = param(2);\n",
      "params": [
        { "title": "Exiting Item", "name": "item", "type": "Object" },
        { "title": "Output Port",  "name": "port", "type": "int"    }
      ],
      "set":          true,
      "storage_path": "MODEL:/Source1>variables/OnExit"
    }
  ]
}
```

### Per-entry fields

- `name` — pass this verbatim as the `trigger` argument to `set_trigger` / `get_trigger` / `remove_trigger`.
- `code_header` — the FlexScript declaration block FlexSim prepends when the trigger fires. **Do not redeclare these variables in your body** — they're already in scope.
- `params[]` — what the named parameters in `code_header` actually mean. `title` is the GUI-facing label; `name` is the variable name you use in code; `type` is the FlexScript type.
- `set` — `true` if there's per-instance storage attached to this trigger (i.e. someone — modeler, GUI, or a prior `set_trigger` — has written code or picked a preset). `false` ⇒ trigger is empty / default.
- `storage_path` — where the per-instance code lives. Empty string when `set` is `false`.

## Examples

**List every trigger on Source1.**
```json
{ "object": "Source1" }
```

**Find which triggers on Processor1 currently have custom code.**
```json
{ "object": "Processor1" }
```
Then filter the returned `triggers` array to entries with `set: true`.

## Errors

| Code | Cause |
|---|---|
| `err:missing_args` | No object name supplied. |
| `err:not_found` | `object` did not resolve via `Model.find`. |
| `err:list_triggers_failed` | Engine-side throw while enumerating. Message includes the underlying exception text. |

## What's NOT here

- **Trigger bodies.** Use `get_trigger` for that. Listing all bodies inline would balloon the response on objects with many populated triggers.
- **Pick-option metadata.** v1 does not surface which picklist presets are available per trigger — that's deferred to v2 alongside the `pick_option` argument on `set_trigger`. The AI should reproduce common pick patterns from raw FlexScript by searching the KB for canonical recipes (e.g. "Send to Port" outputs `outputtoport(item, 1);`).
