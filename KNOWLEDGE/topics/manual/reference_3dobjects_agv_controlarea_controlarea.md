---
id: reference_3dobjects_agv_controlarea_controlarea
name: "Control Area"
kind: manual
breadcrumb: ["Reference","3DObjects","AGV","ControlArea","ControlArea"]
tags: ["manual","reference","3dobjects","agv"]
source: "manual/Reference/3DObjects/AGV/ControlArea/ControlArea.html"
---

# Control Area

## Overview and Key Concepts

 

A control area is an object that enforces mutual exclusion on one or more paths in the AGV network. It can also act as a dynamic barrier that will cause AGVs to route around it when its routingConstraint property is set.

 

In addition to being used within an AGV system, the control area can be integrated in an A* network to restrict the number of A* travelers in a certain area of your grid. You can also programmatically [allocate](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV.AllocatableObject&Method=allocate) and [deallocate](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV.AllocationPoint&Method=deallocate) a control area. For example, using the OnArrival trigger of a conveyor decision point, you can have items in a conveyor system allocate the control area, so that AGVs are not allowed to travel certain paths while items are on a given section of conveyor. FlexSim provides pick list options for doing this automatically.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The control area has the following events:

 

### On Allocated

 

This event is fired when the control area is allocated.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that allocated the control area. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |

 

### On Deallocated

 

This event is fired when the control area is deallocated by an AGV. If you return a reference to an AGV.AllocationPoint from this trigger (by searching the AGV.AllocatableObject's requests array), then the object will allocate itself to that request. This allows you to implement custom prioritization strategies.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that deallocated the control area. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |

 

### On Request

 

This event is fired when the control area receives a request to be allocated by an AGV. You can return an AGV.Request value from this trigger to force the request to either be allowed (i.e. allocate the object), or blocked, overriding the default allocation behavior.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| TE | [TaskExecuter](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskExecuter) | The task executer that is requesting allocation. |
| AGV | [AGV](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV) | The AGV instance associated with the task executer. |
| Point | [AGV.AllocationPoint](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=AGV.AllocationPoint) | The allocation point that is requesting allocation. |

 

### On Allocation Count Change

 

This event is fired both when an AGV allocates the control area, and when it deallocates the control area.

 

This event has no parameters.

 

 

 

## States

 

The control area does not implement any states.

 

 

 

## Statistics

 

The control area tracks the same statistics as the control point.

 

 

 

## Properties Panels

 

The Control Area object uses the following properties panels:

 

- Control Area
- Labels
- Ports

 

 

 

## Properties

 

The Control Area object uses the following properties:

 

| Property | Type |
| --- | --- |
| DeallocationType | Options |
| MaxAllocations | Number |
| IsAGVMember | Boolean |
| IsAStarMember | Boolean |
