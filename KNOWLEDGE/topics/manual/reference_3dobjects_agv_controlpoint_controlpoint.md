---
id: reference_3dobjects_agv_controlpoint_controlpoint
name: "Control Point"
kind: manual
breadcrumb: ["Reference","3DObjects","AGV","ControlPoint","ControlPoint"]
tags: ["manual","reference","3dobjects","agv"]
source: "manual/Reference/3DObjects/AGV/ControlPoint/ControlPoint.html"
---

# Control Point

## Overview and Key Concepts

 

Control Points are points on the AGV network where various decision logic happens.

 

 

A control point may be:

 

- A point where AGVs pick up and/or drop off flow items.
- A stopping point on the network where AGVs wait to enter an area or section of a path.
- A decision point on the network where the AGV looks for work to do.
- A decision point on the network where an AGV is dispatched to some other point on the network, dependent on network state/availability.
- A dynamic barrier that will cause AGVs to route around it when its routingConstraint property is set.
- An arbitrary point on an AGV path where you can execute custom logic.

 

### Control Point Allocation/Deallocation

 

Control Points act as allocation/deallocation points on the network. AGVs automatically look ahead to their next 'waitable' control point and must allocate to that control point, including all allocatable objects up to that point, before proceeding on the network.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The control point has the following events:

 

### On Allocated

 

Fired when the control point is allocated by an AGV.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that allocated the control point. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |

 

### On Arrival

 

Fired at an AGV's arrival at the control point, i.e. when the AGV has decelerated to stop at the control point, either because it could not allocate further ahead or if the control point is the final destination.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that arrived at the control point. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |
| Dest CP | [Object](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object) | The AGV's destination. |
| Path Offset | [double](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=double) | The AGV's destination path offset. |
| AGV Body Offset | [double](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=double) | The AGV's destination body offset. |

 

### On Deallocated

 

Fired when the Control Point is deallocated by an AGV. If you return a reference to an AGV.AllocationPoint from this trigger (by searching the AGV.AllocatableObject's requests array), then the object will allocate itself to that request. This allows you to implement custom prioritization strategies.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that deallocated the control point. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |

 

### On Entry

 

Fired when an object (usually a flow item) is moved into the control point.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | [Object](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object) | The item that entered the control point. |

 

### On Exit

 

Fired when an object (usually a flow item) is moved out of the control point.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | [Object](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object) | The item that is exiting the control point. |

 

### On Pre Arrival

 

Fired at an AGV's pre-arrival to the control point, i.e. the point at which the AGV would start to decelerate to stop at the control point if needed. On Pre Arrival is fired prior the AGV allocating further ahead or when the AGV starts decelerating to its final destination.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer associated with the event. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |
| Dest CP | [Object](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object) | The AGV's destination. |

 

### On Request

 

This event is fired when the control point receives a request to be allocated by an AGV. You can return an AGV.Request value from this trigger to force the request to either be allowed (i.e. allocate the object), or blocked, overriding the default allocation behavior.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that is requesting allocation. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |
| Point | [AGV.AllocationPoint](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV.AllocationPoint) | The allocation point that is requesting allocation. |

 

 

 

## States

 

The control point does not implement any states.

 

 

 

## Statistics

 

The control point tracks the following statistics:

 

### AllocationCount

 

The total number of AGVs who have allocated the control point. Usually this will either be 0 or 1, unless you set the control point's Max Allocations to be greater than 1.

 

 

 

## Properties Panels

 

The Control Point object uses the following properties panels:

 

- Control Point
- Labels
- Ports

 

 

 

## Properties

 

The Control Point object uses the following properties:

 

| Property | Type |
| --- | --- |
| DeallocationType | Options |
| MaxAllocations | Number |
| IsAllocationWaitPoint | Boolean |
