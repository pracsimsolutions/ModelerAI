---
title: ProcessFlow activity catalog
status: v2 — extracted from FlexSim's DragDropLibrary tree
---

# ProcessFlow activity catalog

When calling `modelerai_add_activity`, the `type` argument is the **class name** of the activity (e.g. `InterArrivalSource`), NOT the human-readable label (e.g. `Inter-Arrival Source`). The valid strings are the `focus` values under `views().find("modules/ProcessFlow/DragDropLibrary")`. This catalog mirrors that tree exactly.

For People-specific activities (CreatePerson, EnterLocation, etc.), pass `library_path: "people/Activities"` to `modelerai_add_activity` instead — different subtree.

## Path convention

ProcessFlows are stored at `Tools/ProcessFlow/<name>` in the model tree. `Model.find("MyFlow")` returns null; use `Model.find("Tools/ProcessFlow/MyFlow")` if reaching one via FlexScript. The `modelerai_*` PF tools handle this for you (they walk the toolbox).

## Categories — copy of the FlexSim DragDropLibrary tree

### TokenCreation (4)
Sources — create tokens that flow through the rest of the activities.

| `type` | Purpose |
|---|---|
| `InterArrivalSource` | Tokens at a fixed/distributed interval |
| `ScheduleSource` | Tokens from a schedule / arrivals table |
| `DateTimeSource` | Tokens at specific dates and times |
| `EventTriggeredSource` | Tokens in response to model events |

### Basic (10)
The everyday flow-control activities.

| `type` | Purpose |
|---|---|
| `AssignLabels` | Set / modify labels on the token or referenced objects |
| `Breathe` | 0-second hold (yield) |
| `Delay` | Hold the token for a duration |
| `CustomCode` | Run arbitrary FlexScript |
| `Decide` | Branch token to one output based on a condition |
| `Batch` | Group N tokens together |
| `WaitForEvent` | Block until a specified event fires |
| `CreateTokens` | Spawn child tokens from the current token |
| `Milestone` | Log a token milestone for analytics |
| `Sink` | Destroy the token (terminal) |

### Visual (5)
Animation and visualisation.

| `type` | Purpose |
|---|---|
| `ChangeVisual` | Change the token's visual representation |
| `RunAnimation` | Play an animation on a referenced object |
| `CreateKinematics` | Set up a kinematic motion sequence |
| `ApplyKinematics` | Apply a configured kinematic to an object |
| `FinishKinematics` | Conclude a kinematic sequence |

### Objects (3)
Direct model-object manipulation.

| `type` | Purpose |
|---|---|
| `CreateObject` | Instantiate a FlexSim object in the model |
| `MoveObject` | Move an object to a new location |
| `DestroyObject` | Destroy a referenced object |

### Sub Flow (3)
Sub Flow plumbing (the activities that wire one PF to another).

| `type` | Purpose |
|---|---|
| `RunSubFlow` | Call a Sub Flow ProcessFlow; token returns when sub completes |
| `Start` | Sub Flow entry point (used INSIDE a Sub Flow PF) |
| `Finish` | Sub Flow exit point (used INSIDE a Sub Flow PF) |

### TaskSequences (11)
Build, dispatch, and execute Task Sequences for TaskExecuters.

| `type` | Purpose |
|---|---|
| `CreateTS` | Build a Task Sequence |
| `FinishTS` | Wait for the Task Sequence to complete |
| `TETravel` | TE travels using its navigator |
| `TEAGVTravel` | TE travels using the AGV network |
| `TETimedTravel` | TE travels for a fixed time |
| `TELoad` | TE loads an item |
| `TEUnload` | TE unloads an item |
| `TEDelay` | TE waits in place for a duration |
| `TETravelToLoc` | TE travels to a specified location |
| `TETask` | Generic TE task (custom) |
| `DispatchTS` | Send the Task Sequence to a TE |

### Shared Assets (12)
Resources, Lists, Variables, Zones — things that multiple tokens share.

| `type` | Purpose |
|---|---|
| `List` | Declare a list for tokens to push / pull from |
| `PushToList` | Token pushes an entry onto a List |
| `PullFromList` | Token pulls / waits for an item from a List |
| `Resource` | Declare a capacity-limited resource |
| `Acquire` | Token waits to obtain N units of a Resource |
| `Release` | Token returns previously acquired resource units |
| `Variable` | Declares a variable holder activity |
| `GetVariable` | Read a variable into a token label |
| `SetVariable` | Write a value into a variable |
| `Zone` | Declare a labelled region |
| `EnterZone` | Token records entering a Zone |
| `ExitZone` | Token records leaving a Zone |

### Warehousing (2)
Storage / search helpers.

| `type` | Purpose |
|---|---|
| `FindItem` | Search a List for a matching entry |
| `FindSlot` | Search storage / a List for a free slot |

### Coordination (3)
Token forking & merging.

| `type` | Purpose |
|---|---|
| `Split` | Branch a token into multiple paths |
| `Join` | Wait for split paths to converge |
| `Synchronize` | Barrier: wait for N tokens then continue |

### Preemption (3)
Save & restore token context; explicit release.

| `type` | Purpose |
|---|---|
| `SaveContext` | Snapshot current token context |
| `ReleaseToken` | Manually release a token previously held |
| `RestoreContext` | Restore a previously saved context |

## Base classes — DO NOT USE

These appear in `library().find("processflow/activities")` but are abstract superclasses, NOT instantiable:

- `Activity` — root abstract class
- `BlockingActivity` — abstract intermediate (parent of Delay, Acquire, Synchronize, etc.)

If you pass either as `type`, `modelerai_add_activity` will not flag it as invalid (they ARE in the activities subtree) but the result is a malformed node. Stick to the concrete types listed above.

## Containers (not in DragDropLibrary — advanced)

`ProcessBlock` and `ResourceBlock` exist as classes but aren't surfaced in the standard DragDropLibrary. They're visual containers that hold child activities. Adding child activities INSIDE a container needs a different code path (TBD — `add_activity` does NOT currently handle this).

## People activities

For People-specific activities, swap `library_path` to `"people/Activities"`. The People library includes `CreatePerson`, `EnterLocation`, `ExitLocation`, `EscortPerson`, `Walk`, etc. (See the dump under `library().find("people/Activities")` for the full list.)

## See also

- [`modelerai_add_activity`](modelerai_add_activity.md) — uses the `type` strings cataloged here
- [`modelerai_connect_activities`](modelerai_connect_activities.md) — wires two activities with a connector arrow (distinct from the stacking that `add_activity`'s `after` arg does)
- [`modelerai_set_activity_variable`](modelerai_set_activity_variable.md) — sets variables on an activity (e.g. `arrivalSpacing` on InterArrivalSource, `flowRef` on RunSubFlow)
- [processflow-activity-variables.md](processflow-activity-variables.md) — full per-class variable list with raw dataTypes
- [activities/](activities/README.md) — hand-curated **semantic deep-dive** per class: storage modes, codeHeaders, value examples, gotchas. Currently covers Delay, AssignLabels, CustomCode, InterArrivalSource, CreateObject, EventTriggeredSource, ScheduleSource. **Read the per-class doc before setting any non-trivial variable.**
