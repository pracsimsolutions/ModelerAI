---
id: reference_3dobjects_conveyors_decisionpoint_decisionpoint
name: "Decision Point"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","DecisionPoint","DecisionPoint"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/DecisionPoint/DecisionPoint.html"
---

# Decision Point

## Overview and Key Concepts

 

A Decision Point is a versatile object you can use to build logic into your conveyor system. Decision Points can also be linked to a Merge Controller to notify it when a released slug has cleared designated points in the system.

 

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The decision point has the following additional events:

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Arrival

 

On Arrival fires when an item arrives at the decision point.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Arriving Item | Object | The item that just arrived |
| Conveyor Item | Conveyor | The conveyor the decision point is on |

 

### On Continue

 

On Continue fires right before an item leaves the decision point.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Continuing Item | Object | The item that is about to leave |
| Conveyor Item | Conveyor | The conveyor the decision point is on |

 

 

 

## States

 

The decision point does not implement any states.

 

 

 

## Statistics

 

The decision point tracks the following statistics. These can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Content

 

The number of items that have arrived but not yet continued through the decision point. This will always be 0 or 1.

 

### Input

 

Input The total number of items that have arrived at the decision point.

 

### Output

 

The total number of items that have continued through the decision point.

 

 

 

## Properties Panels

 

The Decision Point uses the following properties panels:

 

- Statistics
- Labels
- Decision Point
- Ports
- Triggers

 

 

 

## Properties

 

The Decision Point object uses the following properties:

 

| Property | Type |
| --- | --- |
| DistAlongConveyor | Unit |
| ItemEdge | Options |
