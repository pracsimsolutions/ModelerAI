# modelerai_add_activity

Adds an activity to a named ProcessFlow using the view-based `createActivity`
API (`openprocessflowview` + `createActivity`). The view is opened (idempotent)
before every call so the view's connector wiring and placement logic is reused.

> **Path note**: ProcessFlows do NOT live at the model root.
> `Model.find("MainFlow")` returns null.
> The correct path is `Tools/Toolbox/ProcessFlow/<kind>/<name>`.
> This tool walks the toolbox correctly — do not use `Model.find` shortcuts.

## Args

```jsonc
{
  "processflow":  "MainFlow",               // required — PF name
  "type":         "Delay",                  // required — activity class name in the library
  "library_path": "processflow/activities", // optional — defaults to "processflow/activities";
                                            //   use "people/Activities" for People activities
  "name":         "MyDelay",               // optional — setname() after creation
  "after":        "Source1",               // optional — auto-wire after this existing activity
  "position":     [10, 5]                  // optional — [x, y] position override
}
```

## Returns

```jsonc
{ "ok": true, "name": "MyDelay", "type": "Delay",
  "processflow": "MainFlow", "path": "Tools/…/MainFlow/MyDelay" }

// With `after` — includes wiring confirmation:
{ "ok": true, "name": "MyDelay", "type": "Delay",
  "processflow": "MainFlow", "path": "…", "connected_to": "Source1" }
```

## Behavior

- If `after` is given: calls `createActivity(view, "ProcessFlowView", libObj,
  prevActivity)` — the 4th arg triggers the view's stacking logic, which
  auto-positions the new activity visually below the predecessor **and** draws
  a connector arrow from the predecessor to the new activity. Returns
  `connected_to` in the result. This is distinct from
  `modelerai_connect_activities`, which draws an explicit arrow between two
  already-existing activities without repositioning either.
- If `after` is NOT given: calls `createActivity(view, "ProcessFlowView",
  libObj)` — standalone, no automatic wiring. Default position is at the
  origin (0, 0, 0).
- `position` overrides placement for either form (in stacked mode the view
  auto-positions; only set `position` if you need an exact override).
- `name` calls `setname()` on the new activity after positioning.
- The ProcessFlowView is opened via `applicationcommand("openprocessflowview",
  pf)` before every call — this is idempotent and required for `createActivity`
  to work.

## Common library paths

| library_path | Activity types |
|---|---|
| `processflow/activities` (default) | Source, Delay, Sink, Assign, Decide, RunSubFlow, … |
| `people/Activities` | CreatePerson, DisposePerson, PersonSource, PersonSink, … |

## Errors

| error_code | Meaning |
|---|---|
| `processflow_not_found` | No PF with that name under Tools/Toolbox/ProcessFlow |
| `library_path_not_found` | The `library_path` subtree doesn't exist in the FlexSim library |
| `activity_type_not_found` | `type` not found under the given `library_path` |
| `previous_not_found` | `after` activity not found inside the named ProcessFlow |
| `add_activity_failed` | View couldn't be opened, or pf.last was null after create |
| `missing_args` | `processflow` or `type` not provided |

## Examples

```jsonc
// Add a standalone Delay to MainFlow
modelerai_call({
  name: "modelerai_add_activity",
  args: { processflow: "MainFlow", type: "Delay", name: "PackDelay" }
})

// Add a Sink auto-wired after "PackDelay"
modelerai_call({
  name: "modelerai_add_activity",
  args: { processflow: "MainFlow", type: "Sink", after: "PackDelay" }
})

// Add a People activity
modelerai_call({
  name: "modelerai_add_activity",
  args: {
    processflow:  "PersonFlow",
    type:         "CreatePerson",
    library_path: "people/Activities",
    name:         "PersonSource1"
  }
})
```
