---
id: reference_3dobjects_conveyors_straightcurvedconveyor_straightcurvedconveyor
name: "Straight and Curved Conveyors"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","StraightCurvedConveyor","StraightCurvedConveyor"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/StraightCurvedConveyor/StraightCurvedConveyor.html"
---

# Straight and Curved Conveyors

## Overview and Key Concepts

 

The straight and curved conveyors can simulate conveyor belts or roller conveyors.

 

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The straight and curved conveyors have the following events:

 <!-- List events in alphabetical order with spaces in the event name --> 

### On Entry

 

On Entry occurs whenever an item enters the conveyor.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Entering Item | treenode | The item that entered the conveyor |

 

### On Exit

 

On Exit occurs whenever an item exits the conveyor.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Exiting Item | treenode | The item that is exiting the conveyor |

 

### On Item Bump

 

This event occurs when an item or a slug is blocked by another item or slug, forming a single slug.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Front Slug | Array | The set of items in the downstream slug |
| Back Slug | Array | The set of items in the upstream slug |

 

 

 

## States

 

For statistical purposes, the conveyor will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Empty

 

There are no items on the conveyor.

 

### Conveying

 

There is at least one item on the conveyor.

 

### Stopped

 

There is at least one item on the conveyor, but the conveyor's speed has is set to zero. The conveyor will go into this state immediately when its speed is set to zero (i.e. at the time it STARTS to decelerate to zero).

 

### Blocked

 

Only used for non-accumulating conveyors. When an item on a non-accumulating conveyor is stopped or blocked, the conveyor will become blocked. Note again that accumulating conveyors do not use this state. If you want to get better blocked state statistics on accumulating conveyors, you should use photo eyes.

 

 

 

## Statistics

 

The conveyor tracks of the following statistics. These can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Content

 

The number of items in the conveyor.

 

### Input

 

The total number of items that have entered the conveyor.

 

### Output

 

The total number of items that have exited the conveyor.

 

### Staytime

 

The durations that items are in the conveyor.

 

### Percent Full

 

The percent that the conveyor is "filled" with items. This uses the following formula: `100 * (TotalStopSpace / ConveyorLength)` where `TotalStopSpace` is the sum of the stop spaces of all the items currently on the conveyor. Note that this percentage may go above 100%, specifically when there are items straddling multiple conveyors.

 

 

 

## Properties Panels

 

The straight and curved conveyors use the following properties panels:

 

- Statistics
- Labels
- Conveyor
- Conveyor Behavior
- Triggers

 

 

 

## Properties

 

Both straight and curved conveyors support the following properties:

 

| Property | Type |
| --- | --- |
| CatchCondition | Code |
| Acceleration | Unit |
| Deceleration | Unit |
| EndLocation | Array(3) |
| EndLocationTranslate | Array(3) |
| Speed | Unit |
| StartLocationTranslate | Array(3) |
| StartLocation | Array(3) |
| UseVirtualLength | Boolean |
| VirtualLength | Unit |
| Visualization | Node Options |
| Width | Unit |
| EntrySpacePadding Unit |  |
| EntrySpaceSizeFactor Number |  |
| FixedIntervalItemEdge Options |  |
| FixedIntervalPatternCustom |  |
| IsAccumulating Boolean |  |
| IsSlugBuilder Boolean |  |
| IsSlugReadyByFillPercent Boolean |  |
| IsSlugReadyByItemCount Boolean |  |
| IsSlugReadyByTime Boolean |  |
| MoveSpacePadding Unit |  |
| MoveSpaceSizeFactor Number |  |
| RestartDelay Unit |  |
| Rise Unit |  |
| RollerAngle Number |  |
| SlugFillPercent Number |  |
| SlugItemCount Number |  |
| SlugMaxItemCount Number |  |
| SlugReadyTime Unit |  |
| SlugReleaseSpeed Unit |  |
| StopSpacePadding Unit |  |
| StopSpaceSizeFactor Number |  |
| UseCatchCondition Boolean |  |
| UseFixedIntervalMovement Boolean |  |

 

Straight conveyors use the following properties:

 

| Property | Type |
| --- | --- |
| HorizontalLength | Unit |

 

Curved conveyors use the following properties:

 

| Property | Type |
| --- | --- |
| Radius | Unit |
| StartAngle | Number |
| SweepAngle | Number |

 

The following properties are not visible in the conveyer panel but can be accessed through a Property Table:

 

### StartLocationTranslate

 

Defines the **X**, **Y**, and **Z** position of the start of the conveyor. Changing this property also updates the end of conveyer such that the same translation value is applied.

 

### EndLocationTranslate

 

Defines the **X**, **Y**, and **Z** position of the start of the conveyor. Changing this property also updates the start of conveyer such that the same translation value is applied.

 

### Rise

 

The change in height from the **start** of the conveyor to the **end** of the conveyor. An ascending conveyor will have a positive rise and a descending conveyor will have a negative rise.
