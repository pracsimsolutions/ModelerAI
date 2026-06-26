---
id: reference_tools_agvnetworktool_accumulationtypes_accumulationtypes
name: "AGV Accumulation Types"
kind: manual
breadcrumb: ["Reference","Tools","AGVNetworkTool","AccumulationTypes","AccumulationTypes"]
tags: ["manual","reference","tools","agvnetworktool"]
source: "manual/Reference/Tools/AGVNetworkTool/AccumulationTypes/AccumulationTypes.html"
---

# AGV Accumulation Types

## Overview and Key Concepts

 

Accumulation Types let you define parameters for accumulating paths, i.e. paths on which AGVs will detect proximity and avoid collisions with each other.

 

You can get to the AGV Accumulation Types page by right-clicking on a Path or Control Point and choosing AGV Network Properties.

 

 

 

## The Accumulation Types Tab

 

The Accumulation Types tab has the following properties:

 

 

Add, remove, rename and re-order Accumulation Types through the combobox and buttons at the top of the page.

 

To assign a path an Accumulation Type, click on the path, and in its Quick Properties on the right, choose the desired type from the Accumulation Type drop-down.

  

#### Allocation

 

AGV Accumulation is separate and distinct from the AGV Control Point allocation mechanism. AGVs will continue to allocate ahead to Control Points while on accumulating paths. Thus if you want to allow more than one AGV between Control Points on an accumulating path, you should increase the Max Allocations value for the Control Points on that path.

  

### Proximity Detection

 

These properties define how two AGVs will detect proximity with each other while on a common accumulating path.

 

- **From Front AGV Edge To Behind AGV Edge** - This defines the AGV edges used to determine proximity. Usually this will be the default: Front AGV  to Behind AGV . This will evaluate the distance from the back of the AGV ahead to the front of the AGV behind. You could also choose something like Front AGV  to Behind AGV , which would evaluate the distance from center to center.
- **Stop Threshold** - This is the proximity distance at which an AGV will go into a "proximity stop" state, and slow down to a stop.
- **Use as Target Stop Spacing** - If checked, then if an ahead AGV is stopped, the stop threshold defines the target stop spacing. Stopped AGVs will thus accumulate with this stop spacing. In other words, an AGV approaching a stopped AGV will begin decelerating to stop BEFORE the stop threshold (as a proximity distance) is breached. The AGV will instead start decelerating such that it comes to a stop with the stop threshold as its spacing behind the ahead AGV. In situations where both AGVs are still moving, the stop threshold will continue to be used in regular proximity detection.
- **Added Trailing Time** - In addition to direct distance-based proximity detection, you can also include time-based proximity detection. In other words, you may want AGVs to follow *n* seconds behind each other, in addition to (or instead of) following *m* meters behind each other. When you add time-based trailing, AGVs will enforce more spacing between themselves as they travel faster. For example, an added trailing time of 5 seconds means that, when the AGV is traveling 1 meter per second, it must stay 5 meters behind the AGV in front of it, whereas when it is traveling at 2 meters per second, it must stay 10 meters behind, etc.
- **Resume Threshold** - The proximity distance at which an AGV can resume its travel after going into a "proximity stop" state. The resume threshold must be greater than the stop threshold.
- **Plus Time** - An optional additional time, started when the Resume Threshold is reached, that the AGV will wait before resuming from a "proximity stop" state.

 

### Intersections

 

When you define an Accumulation Type for a path, the AGV network will treat intersections on that path as allocations. Similar to the way AGVs must allocate Control Points and Control Areas, AGVs must allocate the intersection points on an accumulating path before proceeding past those intersection points. In the Accumulation Types page you define stop distances, which are distances before the intersection where the AGV must stop before an intersection point if the AGV cannot allocate it, as well as clear distances, which are distances after passing the intersection point where AGVs will release the intersection and allow other AGVs to claim it. Each of these distances are split out by whether the AGV is entering, exiting, or on the path. If already on a path or staying on a path, the distances are split out by the path geometry, i.e. whether or not the intersection branches out away from the AGV or toward it.

 

### Intersection Stop Point

 

Here you define the stop distances for an intersection. For each distance, you define the distance itself as well as an AGV edge, which determine what part of the AGV should stop at the stop distance. Usually stop distances will be based on the leading edge of the AGV. When you click in a given field, the diagram on the right will display the distance / edge that you are defining, to help you figure out what exactly the field defines.

 

- **Path Entry** - This defines the stop distance and agv edge associated with entering a path.
- **On Path Long** - This defines the stop distance and agv edge associated with approaching an intersection when already on a path. It is the "long" stop distance because it will be applied to intersections that branch out toward the AGV, and thus require the AGV to stop farther away from the intersection point in order to provide room for merging AGVs.
- **On Path Short** - This defines the stop distance and agv edge associated with approaching an intersection when already on a path. It is the "short" stop distance because it will be applied to intersections that branch out away from the AGV, hence the AGV can stop closer to the intersection without causing issues.
- **Prioritize Control Point Lookahead** - If checked, the stop point / allocation point for path transfers will be adjusted when there is a control point before an intersection, to ensure that the control point lookahead mechanism will happen before allocation of path transfers. This can prevent gridlock in some situations.

 

### Intersection Clear Point

 

Here you define the clear distances for an intersection. Like with stop distance, you define the distance itself as well as an AGV edge. Usually clear distances will be based on the trailing edge of the AGV. When you click in a given field, the diagram on the right will display the distance / edge that you are defining, to help you figure out what exactly the field defines.

 

- **On Path Long** - This defines the clear distance and agv edge associated with clearing an intersection when still on the path (not exiting). It is the "long" clear distance because it will be applied to intersections that branch out toward the clear point, and thus require the AGV to travel farther away from the intersection point before clearing it.
- **On Path Short** - This defines the clear distance and agv edge associated with clearing an intersection when still on the path (not exiting). It is the "short" clear distance because it will be applied to intersections that branch out away from the clear point, hence the AGV can clear it closer to the intersection point without causing issues.
- **Path Exit** - This defines the clear distance and agv edge associated with exiting a path.

 

### Special Rules

 

There are a few special rules that apply to allocating intersection points.

 

- **Pure On-Path Allocations** - The AGV network tries to make basic proximity detection take precedence over intersection stop and clear points. Thus, if an AGV is already on a path and already has an AGV ahead of it for which it is detecting proximity, and if it is not exiting the path at that intersection, then the AGV will be allowed to allocate the intersection point before the ahead AGV has cleared it. Since it is already detecting proximity on the ahead AGV, it is OK to have a simultaneous allocation of the intersection point because the basic proximity detection will already avoid proximity errors.
- **End-To-End Path Transfers** - When transferring from the end of one path to the beginning of another path, the network still treats that as an "intersection" between two paths, i.e. it still requires an allocation of the intersection point. However, again if it can detect that, in transferring to the new path, the AGV will still be detecting proximity with the same AGV that it was detecting before the transfer, then it will go ahead and allow the intersection point to be allocated simultaneously by both AGVs, since the basic proximity detection will already prevent proximity errors. Note that this only applies to transfers between two paths of the same Accumulation Type. If the Accumulation Type is different, it will treat it like a regular intersection allocation.

 

### Troubleshooting AGV Accumulation

 

When you use accumulation in an AGV system, you may run into deadlock issues. See Troubleshooting AGV Accumulation for more information on how to remedy these problems.
