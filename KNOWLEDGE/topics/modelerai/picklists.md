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
| `property` | a named picklist (you supply it) | the object's property value node (variable) | a FlexScript-valued property (e.g. a Processor's process time) |

## `modelerai_list_picks` — discover picks (read-only)

```json
{ "picklist": "parameterpicklist" }
```
- `picklist` (optional, default `parameterpicklist`) — any picklist node under `VIEW:/picklists/`. For PMs use `performancemeasurepicklist`.
- Returns `{ header, pick_count, picks: [ { pick_name, popup, tags: [ { name, default, options? } ], code_template } ] }`.
- Use this first to learn the exact `pick_name` and which `tags` a pick exposes.

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

For a property whose value is a FlexScript expression (e.g. a Processor's process time). The target is an **(object, property)** pair where `property` is the **variable name** (e.g. `cycletime` for a Processor's process time — not the GUI label). You supply the picklist:

- `statisticaldistribution` — the 32 distributions (Normal, Exponential, Triangular, …). Each pick has a `distribution` tag plus `par1`–`par5` (note: par2+ carry a leading comma, e.g. `", 5"`).
- `timepicklist` — time-value presets (Values By Case, By Global Table Lookup, By Percentage, Periodic Rates). Its "Statistical Distribution" entry is a **link** into `statisticaldistribution` — apply returns `pick_is_a_link` pointing you there.

The property keeps its **own header** (`current`/`item` declarations) — apply preserves it and swaps in the pick body.

```json
// set a Processor's process time to Exponential(0, 8, getstream(current))
modelerai_apply_pick {
  "surface": "property", "object": "Processor1", "property": "cycletime",
  "picklist": "statisticaldistribution", "pick_name": "Exponential",
  "tags": { "par1": "0", "par2": ", 8" }
}
// read back
modelerai_get_pick { "surface": "property", "object": "Processor1", "property": "cycletime" }
```

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
