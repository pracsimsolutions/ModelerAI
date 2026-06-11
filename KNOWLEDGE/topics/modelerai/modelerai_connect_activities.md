# modelerai_connect_activities

Draws an explicit connector arrow between two existing activities in a named
ProcessFlow. Both activities must already exist in the PF before calling this
tool. The view is opened (idempotent) and `createConnector` is invoked on the
ProcessFlow.

> **Path note**: ProcessFlows do NOT live at the model root.
> `Model.find("MainFlow")` returns null.
> The correct path is `Tools/Toolbox/ProcessFlow/<kind>/<name>`.
> This tool walks the toolbox correctly — do not use `Model.find` shortcuts.

## Args

```jsonc
{
  "processflow": "MyFlow",   // required — PF name under Tools/Toolbox/ProcessFlow
  "from":        "Source1",  // required — name of the source activity in the PF
  "to":          "Delay1"    // required — name of the target activity in the PF
}
```

## Returns

```jsonc
{ "ok": true, "processflow": "MyFlow", "from": "Source1", "to": "Delay1", "connected": true }
```

## Behavior

- Locates the ProcessFlow via the toolbox walk (not `Model.find`).
- Opens the ProcessFlowView via `applicationcommand("openprocessflowview", pf)`
  — idempotent; safe to call even if the view is already open.
- Calls `function_s(pf, "createConnector", activedocumentnode(), fromAct,
  toAct)` to draw the arrow.
- Does NOT reposition activities. If you want auto-positioned stacking, use
  `modelerai_add_activity` with an `after` arg instead.
- Does NOT validate that `from` → `to` is logically valid (e.g., Sink →
  anything is unusual). The engine accepts the connector; logic validation is
  the modeler's responsibility.

## Difference from `add_activity` with `after`

| | `add_activity` with `after` | `connect_activities` |
|---|---|---|
| Creates a new activity | Yes | No |
| Draws a connector | Yes (automatic) | Yes (explicit) |
| Repositions activities | Yes (stacking) | No |
| Use when | Adding a new step immediately after an existing one | Wiring two already-existing activities |

## Errors

| error_code | Meaning |
|---|---|
| `processflow_not_found` | No PF with that name under Tools/Toolbox/ProcessFlow |
| `from_not_found` | `from` activity not found inside the named ProcessFlow |
| `to_not_found` | `to` activity not found inside the named ProcessFlow |
| `connect_activities_failed` | View could not be opened, or FlexScript threw during `createConnector` |
| `missing_args` | `processflow`, `from`, or `to` not provided |

## Examples

```jsonc
// Wire Source1 → Delay1 in MainFlow
modelerai_call({
  name: "modelerai_connect_activities",
  args: { processflow: "MainFlow", from: "Source1", to: "Delay1" }
})

// Wire Delay1 → Sink1 in MainFlow
modelerai_call({
  name: "modelerai_connect_activities",
  args: { processflow: "MainFlow", from: "Delay1", to: "Sink1" }
})
```

## Typical workflow

When building a ProcessFlow from scratch:

1. `modelerai_create_processflow` — create the PF.
2. `modelerai_add_activity` (no `after`) — place Source.
3. `modelerai_add_activity` with `after: "Source1"` — place Delay, auto-stacked + wired.
4. `modelerai_add_activity` with `after: "Delay1"` — place Sink, auto-stacked + wired.

Use `modelerai_connect_activities` when you need to wire activities that were
placed independently (e.g., branching flows, back-edges, or activities added
without `after`).
