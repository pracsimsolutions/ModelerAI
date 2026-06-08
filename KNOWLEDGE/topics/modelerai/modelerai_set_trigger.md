---
title: modelerai_set_trigger
status: v1 — raw FlexScript only (picks deferred to v2)
---

# `modelerai_set_trigger`

Attach FlexScript code to a named trigger event on a FlexSim object — the curated alternative to `run_script` for OnReset / OnEntry / OnExit / OnMessage / OnDraw / OnPreDraw / OnStateChange / etc.

## When to use

Any time the modeler asks for a trigger to be set, modified, or scripted. **Prefer this over `run_script`** for trigger work — it validates the trigger name against the object's enumerated event list (catching typos in one round), automatically prepends the canonical FlexScript header so the named parameters (`current`, `item`, `port`, etc.) are in scope, and returns the storage path so the AI can confirm what was written.

## Arguments

```json
{
  "object":  "Source1",                        // required, FlexSim object name
  "trigger": "OnExit",                          // required, must match enumerateEvents
  "code":    "outputtoport(item, 1);"           // required, raw FlexScript body
}
```

- `object` — resolved via `Model.find`. Must exist. Pathed names accepted (e.g. `Sources/Source1`).
- `trigger` — must match a name returned by `function_s(obj, "enumerateEvents", tmp, EVENT_TYPE_TRIGGER)` on the object. Error returns the available list if you guess wrong, so retry once and you'll have the right name.
- `code` — FlexScript body the modeler wants executed when the trigger fires. **Do not include the variable declaration header** — the tool prepends it automatically (see "What gets stored" below).

## What gets stored

The tool writes `codeHeader + your_code` to the storage node returned by `function_s(obj, "assertEventWithCode", trigger)`. The `codeHeader` is FlexSim's canonical declaration block for that event, e.g. for `Source.OnExit`:

```flexscript
Object current = ownerobject(c);
Object item = param(1);
int port = param(2);
```

So if you pass `code: "outputtoport(item, 1);"`, the storage node ends up with:

```flexscript
Object current = ownerobject(c);
Object item = param(1);
int port = param(2);
outputtoport(item, 1);
```

That means **`current`, `item`, `port`** (and whatever else the event signature provides) are already declared and ready to use in your body. Don't re-declare them.

To see the exact header for a given event before writing, call [`modelerai_list_triggers({object})`](modelerai_list_triggers.md) and read the `code_header` field on the matching entry. After writing, `set_trigger` also returns the resolved `code_header` so you can verify what got prepended.

## Returns

```json
{
  "ok": true,
  "object":       "Source1",
  "trigger":      "OnExit",
  "storage_path": "MODEL:/Source1>variables/OnExit",   // where the code lives
  "code_header":  "Object current = ownerobject(c);\nObject item = param(1);\nint port = param(2);\n",
  "stored_code":  "<header + your code, exactly as written>"
}
```

On a name miss, returns `err:trigger_not_found` with the available trigger names in the message — retry once with the corrected name.

## Examples

**Send all output items to port 1.**
```json
{ "object": "Source1", "trigger": "OnExit", "code": "outputtoport(item, 1);" }
```

**Stamp the exit time onto each item as a label.**
```json
{ "object": "Processor1", "trigger": "OnExit",
  "code": "item.labels.assert(\"exitTime\").value = Model.time;" }
```

**Stop the model when a particular item reaches the sink.**
```json
{ "object": "Sink1", "trigger": "OnEntry",
  "code": "if (item.name == \"Critical\") stop();" }
```

## Picklist presets (deferred to v2)

FlexSim's GUI shows preset code snippets ("Set Output Port", "Send Message", "Move Object", etc.) in a dropdown next to each trigger. Those presets live in a picklist VIEW node referenced by the class's `<Class>>behaviour/eventInfo/<event>/variables/picklist` coupling. **v1 of `set_trigger` does not accept a pick-name argument.** You write raw code; the AI is expected to reproduce the canonical patterns from search_kb on the picklist VIEW path when the modeler asks for one of them by name.

V2 of this tool will add a `pick_option` arg that resolves the named pick from its VIEW node, writes the resolved code, and round-trips the pick name through `get_trigger`. Same deferral applies to `set_property` for property fields with picklists (e.g. `processTime`). See the "DESIGN NOTE — picklists are deferred to v2" block at the top of `ModelerAiDLL/commands/commands.cpp` for the full rationale.

## Errors

| Code | Cause |
|---|---|
| `err:missing_args` | `object` or `trigger` not supplied. |
| `err:not_found` | `object` did not resolve via `Model.find`. |
| `err:trigger_not_found` | `trigger` is not in the object's enumerated event list. The message includes the available names — retry with the correct one. |
| `err:set_trigger_failed` | Engine-side throw while enumerating, asserting, or writing. Message includes the underlying exception text. |
