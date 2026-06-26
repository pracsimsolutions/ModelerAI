---
id: reference_propertiespanels_agvpanels_controlpoint_controlpoint
name: "Control Point"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","AGVPanels","ControlPoint","ControlPoint"]
tags: ["manual","reference","propertiespanels","agvpanels"]
source: "manual/Reference/PropertiesPanels/AGVPanels/ControlPoint/ControlPoint.html"
---

# Control Point

The Control Point panel defines the location, type, and connections of the point.

 

 

The following properties are on the Control Point panel:

 

### Position

 

Specify the X, Y, and Z position.

 

### Max Allocations

 

The maximum number of AGVs that can simultaneously claim the Control Point.

 

If you set Max Allocations to 0, the Control Point will have *no allocation limit*.

 

### Deallocation Type

 

The Control Point's assigned Deallocation Type. This defines the timing of when the object will be deallocated. Here you choose from a list of deallocation types that you can define in the AGV Network tool.

 

### Allocation Wait Point

 

Indicates whether AGVs can stop at this point when waiting to allocate ahead. The default is checked, meaning AGVs will stop at this point if they cannot allocate ahead.

 

Unchecking the Allocation Wait Point box will result in the following behaviors.

 

- The control point will behave more like a control area boundary. AGVs will not stop at the control point when waiting to allocate ahead. Instead, they will look further ahead to the next 'waitable' control point, and allocate to that point before proceeding from their previous 'waitable' point. This is done in an all-or-nothing manner. Until the AGV can allocate all the way to the next waitable point, it will not allocate any non-waitable points on its path. Once it can allocate all points to the next waitable point, it will then allocate all those points and proceed forward.
- The control point's OnPreArrival trigger will not fire. Since OnPreArrival is primarily associated with allocating ahead, and the AGV will not be making any allocations when pre-arriving to the control point, this trigger is no longer applicable.
- The control point will be displayed differently. Instead of showing a diamond with a 'plus' sign inside, it will only show the plus sign, without the diamond outline. This is to help you better distinguish these logic points from the stopping points on the network.
- By default, other control points will not consider this control point as a 'next control point' when choosing deallocation timing using the 'Deallocate at Next Control Point' deallocation type. This, however, can be configured in the Deallocation Types tab of the AGV network's properties.

 

The primary advantage of making control points 'non-waitable' is that they can essentially become points where you can execute arbitrary logic when AGVs traverse over them, without adding more points where the AGVs will stop and wait to allocate ahead. You can use the OnAllocate and OnDeallocate events for these purposes, either by writing code in these triggers, or by listening for them in process flow. The OnAllocate can be used for logic that should happen when the AGV is looking ahead to where it is going, and the OnDeallocate can be used for logic when the AGV actually traverses over the point. You can use this in conjunction with the control point's **Deallocation Type** to define exactly at which point your logic should fire relative to where the AGV is on the path.

 

You can also use non-waitable points as dynamic barriers for routing purposes by setting their routingConstraint property.

 

### Connections

 

Here you can view, add, remove, and re-order Control Point Connections.
