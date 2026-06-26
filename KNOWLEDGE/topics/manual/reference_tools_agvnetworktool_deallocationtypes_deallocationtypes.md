---
id: reference_tools_agvnetworktool_deallocationtypes_deallocationtypes
name: "AGV Deallocation Types"
kind: manual
breadcrumb: ["Reference","Tools","AGVNetworkTool","DeallocationTypes","DeallocationTypes"]
tags: ["manual","reference","tools","agvnetworktool"]
source: "manual/Reference/Tools/AGVNetworkTool/DeallocationTypes/DeallocationTypes.html"
---

# AGV Deallocation Types

## Overview and Key Concepts

 

Deallocation Types let you define at what point AGVs should deallocate the Control Points and Control Areas that they have allocated. These settings let you customize AGV allocation mechanism to your specific needs.

 

 

 

## The Deallocation Types Tab

 

 

You can get to the Deallocation Types page by right-clicking on a Path or Control Point and choosing AGV Network Properties.

 

The Deallocation Types tab has the following properties:

 

### Deallocation Types List

 

Here you can add, remove, re-order and rename each Deallocation Type.

 

### Edge Definition

 

This defines which "edge" of the AGV will determine the deallocation time. Options are:

 

- **Center** - Deallocation will be triggered when the AGV's center passes the given point.
- **Trailing Edge** - Deallocation will be triggered when the AGV's trailing edge passes the given point.
- **Leading Edge** - Deallocation will be triggered when the AGV's leading edge passes the given point.

 

### Travel Point Definition

 

Defines the associated point on the path that determines deallocation time. Options are:

 

- **Arrives at Next Control Point** - Deallocation will be triggered when the AGV's defined edge arrives at the next Control Point. For Control Areas, this is the next Control Point after the path exits the Control Area.
- **Passes Current Point** - Deallocation will be triggered when the AGV's defined edge passes the current point. For Control Points this is the Control Point itself. For Control Areas, this is the point on the path where the AGV's defined edge exits the Control Area.

 

### Allocation Wait Points Only

 

This property is only relevant when you have chosen **Arrives at Next Control Point** above. When checked, it will only consider as a 'next control point' control points whose ** Allocation Wait Point** property has been checked. This is checked by default, meaning it will ignore non-waitable control points on its path in determining what the 'next' control point is.

 

### Plus Distance

 

This is an additional distance that will be added onto travel before deallocating the object.

 

### Plus Time

 

This is an additional time to delay deallocation, after the defined edge has reached the travel point plus the distance.
