---
title: modelerai_get_trigger
status: v1 — raw FlexScript only (pick-option reverse-resolution deferred to v2)
---

# `modelerai_get_trigger`

Read the currently-stored FlexScript on a named trigger of a named FlexSim object. Pair with [`modelerai_set_trigger`](modelerai_set_trigger.md) for "read, edit, write" round-trips.

## When to use

- Before suggesting a change to a trigger: fetch what's already there and reason about a diff, not a rewrite-from-scratch.
- To answer modeler questions like "what happens on OnExit for Source1?".
- To verify what `modelerai_set_trigger` actually wrote (the returned `stored_code` will match this command's `stored_code`).

For an index of all triggers on an object, use [`modelerai_list_triggers`](modelerai_list_triggers.md) — it's cheaper than fetching every trigger one-by-one.

## Arguments

```json
{
  "object":  "Source1",  // required, FlexSim object name (or Model.find-style path)
  "trigger": "OnExit"    // required, must match enumerateEvents output
}
```

## Returns

```json
{
  "ok":           true,
  "object":       "Source1",
  "trigger":      "OnExit",
  "set":          true,                                 // false ⇒ no per-instance storage
  "storage_path": "MODEL:/Source1>variables/OnExit",   // empty when set:false
  "code_header":  "Object current = ownerobject(c);\nObject item = param(1);\nint port = param(2);\n",
  "stored_code":  "<header + body, exactly as stored>",
  "user_code":    "<stored_code with the header prefix stripped>",
  "header_match": true                                  // false ⇒ stored_code did not start with code_header
}
```

### Interpreting the fields

- `set: false` → the trigger enumerates on this object's class but has no per-instance storage yet. `storage_path`, `stored_code`, and `user_code` are empty. Calling `modelerai_set_trigger` will create the storage.
- `set: true` + `header_match: true` → the trigger was written through `modelerai_set_trigger` (or the GUI, which uses the same `assertEventWithCode` path). `user_code` is the part the modeler would actually edit.
- `set: true` + `header_match: false` → unusual: stored content doesn't start with the canonical header. Could be a legacy hand-edit, a pick-option reference (v1 doesn't reverse-resolve picks — see deferral note below), or content written by some other tool. In that case `user_code` falls back to the full `stored_code` so nothing is lost.

## Examples

**Read OnExit on Source1.**
```json
{ "object": "Source1", "trigger": "OnExit" }
```

**Confirm Sink1's OnEntry was left empty.**
```json
{ "object": "Sink1", "trigger": "OnEntry" }
```
Look for `set: false` on the response.

## Errors

| Code | Cause |
|---|---|
| `err:missing_args` | `object` or `trigger` not supplied. |
| `err:not_found` | `object` did not resolve via `Model.find`. |
| `err:trigger_not_found` | `trigger` is not in the object's enumerated event list. The message includes the available names — retry with the corrected name. |
| `err:get_trigger_failed` | Engine-side throw during enumeration or storage read. Message includes the underlying exception text. |

## Picklist presets (deferred to v2)

`set_trigger`'s v1 deferral note also applies here: when a trigger has been set via the GUI's picklist preset (e.g. "Set Output Port"), v1 of `get_trigger` returns the resolved raw code — it does not surface which preset name was chosen. V2 will add a `pick_option` field to the return that names the matching preset when the stored content matches a known pick. See the "DESIGN NOTE — picklists are deferred to v2" block at the top of `ModelerAiDLL/commands/commands.cpp`.
