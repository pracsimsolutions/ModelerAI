---
title: ProcessFlow activity variables — full per-class schema
status: v1 — extracted 2026-06-11 via library().find("processflow/activities") sweep
---

# ProcessFlow activity variables

Authoritative list of every variable on every ProcessFlow activity class.
Use with [`modelerai_set_activity_variable`](modelerai_set_activity_variable.md)
and [`modelerai_get_activity_variable`](modelerai_get_activity_variable.md).

**How to read:**
- Each section is one activity class.
- Each row is one variable: name + dataType + plain-English kind.
- Variables shown here are the union of class-specific variables AND any
  inherited from the `Activity` base class — the FlexSim engine carries
  both onto every instance.

**Path note:** the variables live under the activity's `>variables` subnode
(leading `>` is FlexSim's hidden-container convention). The curated
`modelerai_*` tools handle that path discipline for you.

## DataType key (corrected — matches FlexSim's `DATATYPE_*` constants)

| code | constant | how the value behaves |
|---|---|---|
| `0` | (no dataType) | The node has no inherent dataType — it's a plain subtree of children. The actual structure is built on top: **SDT** (Structured Data Table — FlexSim-specific helper API; used for `events` rows, `stopTimes`, `eventBinding`, etc.), **Table** (FlexScript Table API; used for ScheduleSource's `arrivals`), or just a plain container. dataType alone doesn't tell you which — check the node's children. Set via specialized helpers, not a single value. |
| `1` | `DATATYPE_NUMBER` | Numeric. Pass a JSON number to `set_activity_variable`. |
| `2` | `DATATYPE_STRING` | String. Either a literal text value OR a FlexScript body, depending on the variable (see per-variable docs). Pass a JSON string. |
| `3` | `DATATYPE_COUPLING` | Pointer to another treenode. Pass `{"ref":"SiblingActivityName"}` (same PF) or `{"ref_pf":"OtherPFName"}` (other PF). |
| `4` | `DATATYPE_OBJECT` | An Object class node. Not seen in PF activity variables to date. |
| `5` | `DATATYPE_PARTICLE` | Particle. Not seen — semantics TBD. |
| `6` | `DATATYPE_BUNDLE` | The formal Bundle type. Not seen in PF activity variables to date — do NOT confuse with dataType=0 containers. |
| `7` | `DATATYPE_SIMPLE` | **Polymorphic** — the cell can hold a coupling pointer, a literal value, OR a FlexScript expression, depending on what was assigned. Very common for `assignTo`, `objectRef`, `involved1`, `teRef` in PF activities. Read the per-variable doc; the agent often has multiple valid input shapes. |

> **The dataType in this table is the DEFAULT (template) shape.** The
> FlexSim engine supports swapping a variable node entirely (copy/replace
> from a different library template), which changes the node's runtime
> dataType. So a variable shown here as `DATATYPE_COUPLING (3)` may end
> up holding `DATATYPE_STRING (2)` FlexScript or a `dataType=0` SDT
> container if the modeler picked a different mode in the UI. The
> per-activity doc explains which modes a given variable supports.

## Per-class variables

### ProcessBlock (container — advanced)
| variable | dataType | kind |
|---|---|---|
| fontName | 2 | string |
| fontSize | 1 | number |
| fontStyle | 1 | number |
| fontColor | 0 | container |
| isExpanded | 1 | number |

### ResourceBlock (container — advanced)
| variable | dataType | kind |
|---|---|---|
| type | 1 | number |

### Activity (abstract base — DO NOT INSTANTIATE)
These propagate to every concrete activity:

| variable | dataType | kind |
|---|---|---|
| traceOption | 1 | number |
| traceRequirement | 1 | number |
| activityRank | 1 | number |
| showLinks | 1 | number |
| next | 3 | coupling |
| prev | 3 | coupling |
| hasBreakpoint | 1 | number |
| breakpointCondition | 1 | number |

### BlockingActivity (abstract base) — no >variables

### MaxWaitTimer (mixin)
| variable | dataType | kind |
|---|---|---|
| useMaxWait | 1 | number |
| startWait | 1 | number |
| waitTime | 1 | number |
| onWaitTimerFired | 2 | string |

### MaxIdleTimer (mixin)
| variable | dataType | kind |
|---|---|---|
| useMaxIdle | 1 | number |
| startIdle | 1 | number |
| idleTime | 1 | number |
| onIdleTimerFired | 2 | string |

### Acquire
| variable | dataType | kind |
|---|---|---|
| resourceRef | 3 | coupling |
| queryNode | 2 | string |
| quantity | 1 | number |
| priority | 1 | number |
| assignTo | 7 | simple |

### ApplyKinematics
| variable | dataType | kind |
|---|---|---|
| dataNode | 7 | simple |
| object | 7 | simple |
| type | 1 | number |
| xLoc | 1 | number |
| yLoc | 1 | number |
| zLoc | 1 | number |
| targetSpeed | 1 | number |
| acceleration | 1 | number |
| deceleration | 1 | number |
| startSpeed | 1 | number |
| endSpeed | 1 | number |
| startTime | 1 | number |
| assignType | 1 | number |
| waitForFinish | 1 | number |
| assignTo | 0 | container |

### AssignLabels
| variable | dataType | kind |
|---|---|---|
| assignTo | 7 | simple |
| includeDefaultLabels | 1 | number |

### Batch
| variable | dataType | kind |
|---|---|---|
| groupBy | 3 | coupling |
| batchQuantity | 1 | number |
| quantifier | 0 | container |
| releaseQuantity | 1 | number |
| overflow | 1 | number |
| labelValues | 0 | container |
| startWait | 2 | string |
| onWaitTimerFired | 2 | string |
| startIdle | 2 | string |
| onIdleTimerFired | 2 | string |

### Breathe
| variable | dataType | kind |
|---|---|---|
| delayTimeNode | 2 | string |
| container | 1 | number |
| lockedToMe | 1 | number |
| fontName | 2 | string |
| fontSize | 1 | number |
| fontStyle | 1 | number |
| fontColor | 0 | container |
| showLinks | 1 | number |
| traceOption | 1 | number |
| traceRequirement | 1 | number |
| next | 3 | coupling |
| prev | 3 | coupling |

### ChangeVisual
| variable | dataType | kind |
|---|---|---|
| fontSize | 1 | number |

### CreateKinematics
| variable | dataType | kind |
|---|---|---|
| dataNode | 7 | simple |
| object | 7 | simple |
| flags | 1 | number |
| defineStart | 1 | number |
| xLoc | 1 | number |
| yLoc | 1 | number |
| zLoc | 1 | number |
| xRot | 1 | number |
| yRot | 1 | number |
| zRot | 1 | number |

### CreateObject
| variable | dataType | kind |
|---|---|---|
| objectRef | 1 | number |
| quantity | 1 | number |
| destination | 2 | string |
| assignType | 1 | number |
| assignTo | 7 | simple |

### CreateTokens
| variable | dataType | kind |
|---|---|---|
| quantity | 1 | number |
| copy | 1 | number |
| parentLink | 1 | number |
| labelAccess | 1 | number |
| destination | 3 | coupling |
| holdToken | 1 | number |
| oneAtATime | 1 | number |
| assignTo | 0 | container |
| assignType | 1 | number |

### CreateTS
| variable | dataType | kind |
|---|---|---|
| teRef | 7 | simple |
| assignType | 1 | number |
| assignTo | 7 | simple |
| priority | 1 | number |
| preempt | 1 | number |
| waitForFinishState | 1 | number |
| dispatchAndWait | 1 | number |
| assignExecuterTo | 7 | simple |
| callbackNode | 2 | string |

### CustomCode
| variable | dataType | kind |
|---|---|---|
| codeNode | 2 | string |

### DateTimeSource
| variable | dataType | kind |
|---|---|---|
| arrivals | 0 | container |
| generated | 1 | number |
| mode | 1 | number |
| timeMode | 1 | number |
| calendarStartTime | 7 | simple |
| snapTo | 1 | number |
| arrivalSpacing | 1 | number |
| variability | 0 | container |
| repeatInterval | 1 | number |
| cycleCount | 1 | number |
| pickColor | 1 | number |
| _arrivals | 0 | container |
| enabled | 1 | number |
| randomStream | 2 | string |

### Delay
| variable | dataType | kind |
|---|---|---|
| delayTimeNode | 2 | string |
| container | 1 | number |
| lockedToMe | 1 | number |
| fontName | 2 | string |
| fontSize | 1 | number |
| fontStyle | 1 | number |
| fontColor | 0 | container |
| showLinks | 1 | number |
| traceOption | 1 | number |
| traceRequirement | 1 | number |
| next | 3 | coupling |
| prev | 3 | coupling |

### Decide
| variable | dataType | kind |
|---|---|---|
| decision | 1 | number |

### DestroyObject
| variable | dataType | kind |
|---|---|---|
| objectRef | 7 | simple |
| recycle | 1 | number |
| isAsynchronous | 1 | number |

### DispatchTS
| variable | dataType | kind |
|---|---|---|
| teRef | 7 | simple |
| tsRef | 7 | simple |
| callbackNode | 2 | string |
| waitUntilFinish | 1 | number |

### EnterZone
| variable | dataType | kind |
|---|---|---|
| zoneRef | 3 | coupling |
| enterForRef | 7 | simple |
| onFail | 2 | string |

### EventTriggeredSource
| variable | dataType | kind |
|---|---|---|
| events | 0 | container |
| listenerCoupling | 0 | container |
| assignObjectTo | 3 | coupling |
| tokenName | 2 | string |
| enabled | 1 | number |

### ExitZone
| variable | dataType | kind |
|---|---|---|
| zoneRef | 3 | coupling |
| exitForRef | 7 | simple |

### FindSlot
| variable | dataType | kind |
|---|---|---|
| queries | 0 | container |
| queryParams | 0 | container |
| assignType | 1 | number |
| assignTo | 7 | simple |
| assignToItem | 7 | simple |
| fifo | 1 | number |

### FindItem
| variable | dataType | kind |
|---|---|---|
| queries | 0 | container |
| assignTo | 7 | simple |
| outbound | 1 | number |

### Finish
| variable | dataType | kind |
|---|---|---|
| returnNode | 0 | container |

### FinishKinematics
| variable | dataType | kind |
|---|---|---|
| dataNode | 7 | simple |
| object | 0 | container |

### FinishTS
| variable | dataType | kind |
|---|---|---|
| tsRef | 7 | simple |

### GetVariable
| variable | dataType | kind |
|---|---|---|
| variableRef | 3 | coupling |
| assignTo | 7 | simple |
| assignType | 1 | number |

### InterArrivalSource
| variable | dataType | kind |
|---|---|---|
| arrivalAtTimeZero | 1 | number |
| interArrivalTime | 2 | string |
| tokenName | 2 | string |
| enabled | 1 | number |

### Join
| variable | dataType | kind |
|---|---|---|
| partitionID | 2 | string |
| partitions | 0 | container |

### List
| variable | dataType | kind |
|---|---|---|
| listRef | 3 | coupling |
| pushCallback | 2 | string |
| pullCallback | 2 | string |

### Milestone
| variable | dataType | kind |
|---|---|---|
| involved | 7 | simple |
| milestoneName | 2 | string |
| collector | 3 | coupling |
| container | 1 | number |
| lockedToMe | 1 | number |
| fontName | 2 | string |
| fontSize | 1 | number |
| fontStyle | 1 | number |
| fontColor | 0 | container |
| showLinks | 1 | number |
| traceOption | 1 | number |
| traceRequirement | 1 | number |
| next | 3 | coupling |
| prev | 3 | coupling |

### MoveObject
| variable | dataType | kind |
|---|---|---|
| objectRef | 7 | simple |
| destination | 7 | simple |

### PushToList
| variable | dataType | kind |
|---|---|---|
| listRef | 3 | coupling |
| push | 7 | simple |
| partitionID | 3 | coupling |
| assignType | 1 | number |
| assignTo | 7 | simple |
| keepOnList | 1 | number |

### PullFromList
| variable | dataType | kind |
|---|---|---|
| listRef | 3 | coupling |
| query | 2 | string |
| allOrNothing | 1 | number |
| doNotRemove | 1 | number |
| partitionID | 3 | coupling |
| puller | 7 | simple |
| request | 1 | number |
| require | 1 | number |
| assignType | 1 | number |
| assignTo | 7 | simple |
| keepBackorder | 1 | number |
| startIdle | 2 | string |

### Release
| variable | dataType | kind |
|---|---|---|
| resource | 7 | simple |
| requirement | 1 | number |
| assignTo | 3 | coupling |
| assignType | 1 | number |

### ReleaseToken
| variable | dataType | kind |
|---|---|---|
| tokenRef | 7 | simple |
| releaseTo | 3 | coupling |

### Resource
| variable | dataType | kind |
|---|---|---|
| reference | 3 | coupling |
| refCount | 1 | number |
| defaultQuery | 2 | string |
| pullCallback | 2 | string |

### RestoreContext — no >variables

### RunAnimation
| variable | dataType | kind |
|---|---|---|
| objectRef | 7 | simple |
| animation | 1 | number |
| waitUntilFinish | 1 | number |
| speedType | 1 | number |
| speedValue | 1 | number |
| repeatType | 1 | number |
| repeatValue | 1 | number |
| fontSize | 1 | number |

### RunSubFlow
| variable | dataType | kind |
|---|---|---|
| labelAccess | 1 | number |
| holdToken | 1 | number |
| oneAtATime | 1 | number |

### SaveContext
| variable | dataType | kind |
|---|---|---|
| tokenRef | 7 | simple |
| labelName | 2 | string |

### ScheduleSource
| variable | dataType | kind |
|---|---|---|
| arrivals | 0 | container |
| repeatSchedule | 1 | number |
| offsetTime | 1 | number |
| currentArrival | 1 | number |
| repeatCount | 1 | number |
| lastTime | 1 | number |
| enabled | 1 | number |

### SetVariable
| variable | dataType | kind |
|---|---|---|
| variableRef | 3 | coupling |
| value | 1 | number |
| rate | 0 | container |

### Sink
| variable | dataType | kind |
|---|---|---|
| deallocateAssets | 1 | number |
| preserveData | 1 | number |

### Split
| variable | dataType | kind |
|---|---|---|
| labelAccess | 1 | number |
| copy | 1 | number |
| parentLink | 1 | number |
| assignTo | 7 | simple |

### Start — no >variables

### Synchronize
| variable | dataType | kind |
|---|---|---|
| partitionID | 2 | string |
| partitions | 0 | container |
| startWait | 2 | string |
| onWaitTimerFired | 2 | string |
| startIdle | 2 | string |
| onIdleTimerFired | 2 | string |

### TEDelay
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| var1 | 1 | number |
| var2 | 1 | number |

### TELoad
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |
| involved2 | 0 | container |

### TETask
| variable | dataType | kind |
|---|---|---|
| callbackNode | 2 | string |
| involved1 | 0 | container |
| involved2 | 0 | container |
| resourceRef | 7 | simple |
| var1 | 1 | number |
| var2 | 1 | number |
| var3 | 1 | number |
| var4 | 1 | number |
| waitUntilFinish | 1 | number |
| taskType | 1 | number |

### TETravel
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |
| var1 | 2 | string |

### TEAGVTravel
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |

### TETimedTravel
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |
| var4 | 1 | number |

### TETravelToLoc
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| var4 | 2 | string |

### TEUnload
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |
| involved2 | 7 | simple |

### TEUtilize (NOT in DragDropLibrary — exists but not standard-exposed)
| variable | dataType | kind |
|---|---|---|
| taskType | 1 | number |
| involved1 | 7 | simple |
| involved2 | 0 | container |
| var1 | 1 | number |

### Variable
| variable | dataType | kind |
|---|---|---|
| variableRef | 1 | number |
| variableData | 7 | simple |

### WaitForEvent
| variable | dataType | kind |
|---|---|---|
| events | 0 | container |
| onListenerInitialized | 2 | string |
| assignObjectTo | 3 | coupling |
| decision | 1 | number |
| fontSize | 1 | number |

### Zone
| variable | dataType | kind |
|---|---|---|
| subsets | 0 | container |
| constraints | 0 | container |
| queueStrategy | 2 | string |
| enforceQueueOrder | 1 | number |
| maxContent | 1 | number |
| hasMaxContent | 1 | number |
| allowMultipleRequestsBeforeCheck | 1 | number |
| customConstraintCheck | 0 | container |
| trackUnmatched | 1 | number |
| Unmatched Statistics | 0 | container |
| Unmatched Request Statistics | 0 | container |
| partitionNode | 2 | string |
| partitionCalculations | 0 | container |

## Common-noise variables (skip unless intentional)

These show up on many classes via inheritance / shared visuals and rarely
need to be set by the modeler. Listed here so the agent doesn't waste a
turn reaching for them when a semantic variable is wanted:

- `traceOption`, `traceRequirement`, `activityRank`, `showLinks`
- `next`, `prev` (auto-managed by connector tools)
- `hasBreakpoint`, `breakpointCondition`
- `fontName`, `fontSize`, `fontStyle`, `fontColor`
- `container`, `lockedToMe`
- `_arrivals`, `generated`, `currentArrival`, `lastTime` (internal state)
