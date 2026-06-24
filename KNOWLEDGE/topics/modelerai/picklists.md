---
title: Picklist presets (parameters + performance measures)
status: live — read from the VIEW tree, no stored catalog
read_topic_id: modelerai/picklists
---

# Picklist presets — `list_picks` / `apply_pick` / `get_pick`

FlexSim attaches **picklists** (dropdowns of preset behaviours) to many surfaces. These three tools let you discover, apply, and read those presets **by name**, with no stored catalog — every call reads FlexSim's live `VIEW` tree, so results are always current.

A pick is a code template with two things you fill in:
- **tags** — named slots like `property`, `row`, `stat`. A dropdown tag also reports its allowed `options`.
- **reference** — the object/group the behaviour acts on (the code runs `reference.as(Group)`, `reference.as(Object).setProperty(...)`, etc.).

## Surfaces

| `surface` | picklist read | applied to | what it is |
|---|---|---|---|
| `parameter` (default) | `parameterpicklist` | the parameter's `Value/onSet` | runs when the parameter's value changes (e.g. on reset) |
| `performance_measure` | `performancemeasurepicklist` | the PM's `Value/valueNode` | computes the PM's value when read |
| `trigger` | a named event-type picklist (you supply it) | the object's trigger event storage | runs when that event fires on the object |
| `property` | auto-resolved from the QuickProperties panel (no picklist arg) | the object's property value node (variable) | a FlexScript-valued property (e.g. a Processor's process time) |

## `modelerai_list_picks` — discover picks (read-only)

```json
{ "picklist": "parameterpicklist" }
```
- `picklist` (optional, default `parameterpicklist`) — any picklist node under `VIEW:/picklists/`. For PMs use `performancemeasurepicklist`.
- `module` (optional) — read a **module's own** picklists at `VIEW:/modules/<module>/picklists/` instead of the general collection. Families: `ProcessFlow` (44), `People` (19), `AGV` (8: `InitializeTravel`, `WayPointRequirement`, `WayPointTrigger`, `AGVStartMethod`, `OffsetTravelPicklist`, `PathCondition`, `PathCost`, `GeneralTrigger`). The same `module` arg works on `apply_pick`/`get_pick` (trigger/property surfaces).
- Returns `{ header, pick_count, picks: [ { pick_name, popup, tags: [ { name, default, options? } ], code_template } ] }`.
- Use this first to learn the exact `pick_name` and which `tags` a pick exposes.
- **Property surface:** `{ "surface": "property", "object": "...", "property": "<display name>" }` discovers the picks for a property the same way `apply`/`get_pick` resolve them — auto-resolves the picklist from the QuickProperties panel and **follows links** (so `ProcessTime` returns the full distribution list plus the direct time presets). Adds `variable` + `picklists` to the result.

## `modelerai_apply_pick` — apply a pick by name

```json
{
  "surface": "parameter",
  "parameter": "Queues",
  "pick_name": "Delete and Copy Group Members",
  "tags": { "xOffset": "3" },
  "reference": "Queues"
}
```
- `surface` (optional, default `parameter`) — `parameter` or `performance_measure`.
- `parameter` / `performance_measure` — the target instance name (also accepted: `target`).
- `pick_name` — must match a `pick_name` from `list_picks`. A wrong name returns `{ error: "pick_not_found", available: [...] }` — retry from that list.
- `tags` (optional) — `{ tagName: "<raw FlexScript expression>" }`. **Values are raw FlexScript, so quote string literals yourself:** `"property": "\"ItemPlacement\""`, `"row": "2"`. An unknown tag returns `{ error: "unknown_tag", valid_tags: [...] }`.
- `reference` (optional) — the object or group the behaviour acts on. **Most picks need this** (they call `reference.as(...)`); pass it in the same call. Resolved via `Model.find`, falling back to `Tools/Groups/<name>` for groups. A bad value returns `{ error: "reference_not_found" }`.

Applying writes `header + filled template` to the instance's code node and compiles it; the embedded `/***popup:Name*/` marker makes FlexSim's UI show the pick as selected.

## `modelerai_get_pick` — read the current pick

```json
{ "surface": "parameter", "parameter": "Queues" }
```
A bare string is treated as a parameter name. Returns `{ current_pick, popup, tags: [ { name, value, options? } ] }`, or `current_pick: null` when the code node holds hand-written (non-pick) code.

## Triggers (surface `trigger`)

Triggers differ from parameters/PMs: the target is an **(object, trigger)** pair, and the picklist is **not** derivable from the event — **you supply its name**, chosen by event type:

| trigger event | picklist |
|---|---|
| OnEntry / OnExit | `entryexittriggerpicklist` |
| OnMessage | `messagetriggerpicklist` |
| OnStateChange | `statechangepicklist` |
| OnReset | `resettriggerpicklist` |
| (others) | the matching `*triggerpicklist` / `*picklist` |

Trigger picklists are **nested under categories** (Data / Control / Visual / Lists), so `list_picks` returns each pick with a `category` field. The code header comes from the **event** (not the picklist), and there is **no `reference`**.

```json
// discover
modelerai_list_picks { "picklist": "entryexittriggerpicklist" }
// apply
modelerai_apply_pick {
  "surface": "trigger", "object": "Queue1", "trigger": "OnEntry",
  "picklist": "entryexittriggerpicklist",
  "pick_name": "Set Object Color", "tags": { "color": "Color.red" }
}
// read back
modelerai_get_pick { "surface": "trigger", "object": "Queue1", "trigger": "OnEntry" }
```
A wrong `trigger` returns the available event names; a wrong `pick_name` returns the available picks. Choose the picklist whose event type matches the trigger — applying an entry/exit pick to an unrelated event can reference variables the event's header doesn't declare.

## Properties (surface `property`)

For a property whose value is a FlexScript expression (e.g. a Processor's process time). The target is an **(object, property)** pair where `property` is the **DISPLAY name** (e.g. `"ProcessTime"`, `"InterArrivalTime"`, `"SetupTime"`). Everything else **auto-resolves** from the object's QuickProperties panel — you do **not** supply the picklist or the variable:

- The real variable (e.g. `ProcessTime` → `cycletime`), the code header (`current`/`item` declarations), and the picklist are looked up for you.
- Pick collection **follows links**, so the distributions behind `timeitempicklist → statisticaldistribution` (Normal, Exponential, Triangular, …) are reachable in the same call as the direct time presets (Values By Case, By Percentage, Batch Processing, Periodic Rates, …). There is no `pick_is_a_link` for this surface.
- **Pass plain numbers for distribution tags.** Picks fill positional tags `par1, par2, par3…` in FlexScript signature order; the tool inserts the argument separators (commas) and auto-fills the trailing random-stream arg with `getstream(current)`, so e.g. Exponential(location, mean) is just `{ "par1": "0", "par2": "40" }` — never write `", 40"` or set the stream yourself.
- **Use the picks, not `set_property`, for any distribution / random expression** — only the pick gets a per-object stream AND round-trips in the GUI dropdown. `set_property` is for plain constants (a flat `ProcessTime = 60`).
- `picklist` is accepted only as an override when the panel can't resolve the property (then `property` is treated as the variable name directly).

Discover what's available for a property, apply by name, read the current value:

```json
// discover — flattened pick list for this property (distributions + direct presets)
modelerai_list_picks { "surface": "property", "object": "Processor1", "property": "ProcessTime" }

// apply — Exponential, mean 40 → exponential(0, 40, getstream(current))
modelerai_apply_pick {
  "surface": "property", "object": "Processor1", "property": "ProcessTime",
  "pick_name": "Exponential", "tags": { "par1": "0", "par2": "40" }
}

// read back
modelerai_get_pick { "surface": "property", "object": "Processor1", "property": "ProcessTime" }
```

### Map every picklist-backed property — `modelerai_list_object_picklists`

`list_picks {surface:"property"}` drills into ONE property. To see **every** property an object can drive with a preset (process time, setup time, operator ref, …) in one read-only call:

```json
modelerai_list_object_picklists { "object": "Processor1" }
// → { ok, object, class, property_count,
//     properties: [ { property: "ProcessTime", variable: "cycletime",
//                     picklists: ["VIEW:/picklists/timeitempicklist"] }, … ] }
```

Each entry's `property` is the DISPLAY name to feed straight into `apply_pick`/`get_pick`. `picklists` is the panel's dropdown union and may include **custom** picklists at `VIEW:/custompicklists/<name>` (model/class-specific) alongside the general ones — the property surface reads both. Accepts a bare object-name string too.

## Worked example — count-driven duplication

```
1. create Queue1, Source1; connect Source1 → Queue1
2. create group "Queues" containing Queue1
3. add_parameter { name:"Queues", type:"int", value:1 }
4. apply_pick {
     parameter: "Queues",
     pick_name: "Delete and Copy Group Members",
     tags: { xOffset: "3" },
     reference: "Queues"          // bind the group the pick copies
   }
```
Now setting the `Queues` parameter to N and resetting rebuilds N copies of the queue, 3 units apart in X.

## Notes

- Every parameter pick exposes a `condition` tag (default `isReset`) — the guard controlling when the behaviour runs.
- Parameter pick bodies have `reference`, `newValue`, `oldValue`, `isReset` in scope; PM bodies have `reference`, `extraData`, `repData`.
- Prefer these over `run_script` for parameter/PM behaviours — they validate names against the live tree and bind the reference for you.
