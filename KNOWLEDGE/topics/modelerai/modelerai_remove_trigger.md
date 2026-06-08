---
title: modelerai_remove_trigger
status: v1
---

# `modelerai_remove_trigger`

Clear the per-instance code attached to a named trigger on a FlexSim object — the inverse of [`modelerai_set_trigger`](modelerai_set_trigger.md). Equivalent to right-click → "Remove" in the trigger's GUI dropdown.

## When to use

- Modeler wants a previously-scripted trigger restored to its default (empty / no-op) state.
- Reverting an experimental tweak.
- Before re-writing a trigger from a completely different starting point (calling `set_trigger` again would overwrite anyway — only use `remove_trigger` first if you specifically want the GUI to show the trigger as "unset" between the two steps).

Idempotent: removing an already-empty trigger succeeds and reports `removed: false`.

## Arguments

```json
{
  "object":  "Source1",  // required
  "trigger": "OnExit"    // required, must match enumerateEvents
}
```

## Returns

```json
{
  "ok":      true,
  "object":  "Source1",
  "trigger": "OnExit",
  "was_set": true,   // whether the trigger had storage before this call
  "removed": true    // whether this call actually destroyed the storage
}
```

- `was_set: true,  removed: true`  → cleared an existing trigger.
- `was_set: false, removed: false` → trigger was already empty (idempotent no-op). Still `ok: true`.

After this call, a subsequent `modelerai_get_trigger` for the same name returns `set: false` and `modelerai_list_triggers` shows the entry with `set: false`.

## How it works

Looks up the trigger via `enumerateEvents`, follows the `reference` coupling on the matching entry to the storage node, and destroys it. FlexSim's `reference` coupling on that entry then reverts to null — matching the GUI's "unset" representation.

## Examples

**Clear the OnExit trigger on Source1.**
```json
{ "object": "Source1", "trigger": "OnExit" }
```

**Idempotent cleanup (safe to call even when unsure if the trigger was set).**
```json
{ "object": "Processor1", "trigger": "OnEntry" }
```
Check `removed` in the response if you need to know whether something was actually cleared.

## Errors

| Code | Cause |
|---|---|
| `err:missing_args` | `object` or `trigger` not supplied. |
| `err:not_found` | `object` did not resolve via `Model.find`. |
| `err:trigger_not_found` | `trigger` is not in the object's enumerated event list. The message includes the available names — retry with the corrected name. |
| `err:remove_trigger_failed` | Engine-side throw during enumeration or destroy. Message includes the underlying exception text. |
