---
id: reference_3dobjects_agv_agvtraveler_agvtraveler
name: "AGV Traveler"
kind: manual
breadcrumb: ["Reference","3DObjects","AGV","AGVTraveler","AGVTraveler"]
tags: ["manual","reference","3dobjects","agv"]
source: "manual/Reference/3DObjects/AGV/AGVTraveler/AGVTraveler.html"
---

# AGV Traveler

## Overview and Key Concepts

 

When you connect a task executer to an AGV path network, it becomes a traveler on the AGV network. Once connected, there are additional properties, events, and statistics that are available on that AGV. This topic explains these elements and shows how to access them.

 

 

 

## Properties

 

An AGV Traveler object uses the following properties panel:

 

- AGV Traveler

 

 

 Events 

An AGV also has additional events that can be accessed either through the task executer's Triggers panel, or from a process flow.

 

- **OnStartTravel** - Fired when the AGV starts a travel task.
- **OnFinishTravel** - Fired when the AGV finishes a travel task.
- **OnPreAllocate** - Fired just before the AGV attempts to allocate forward. This will either be followed by one or more OnAllocate events or by an OnAllocationFailed event if it was not able to allocate forward. Allocation happens either on pre-arrival to a control point or when trying to allocate an intersection point on an accumulating path.
- **OnAllocate** - Fired when a control point, control area, or accumulating path intersection point is allocated.
- **OnAllocationFailed** - Fired when the AGV fails to allocate forward and hence must stop and wait. If you implement code for this and return 1, then the default behavior will be skipped. This is especially useful if you are defining your own deadlock recovery mechanism, and don't want the default behavior (stop the model and show an error message) to be executed.
- **OnNavigationFailed** - Fired when the AGV fails to find a valid path to a destination. If you implement code for this and return 1, then the traveler will try again. This means that, before returning 1, you should first make changes to the network so as to make the destination accessible from the traveler's location. If you return 2, then the traveler will finish its current travel task as if it arrived at the destination.
- **OnDeallocate** - Fired when the AGV deallocates a control point, control area, or accumulating path intersection point. If you return a reference to an AGV.AllocationPoint from this trigger (by searching the AGV.AllocatableObject's requests array), then the object will allocate itself to that request. This allows you to implement custom prioritization strategies. Note that the allocatable object's own OnDeallocated trigger takes precedence if both triggers return an allocation point.
- **OnAccumulationStop** - Fired when the AGV hits its proximity stop threshold on an accumulating path and must stop.
- **OnAccumulationResume** - Fired when the AGV hits its proximity resume threshold on an accumulating path and can resume.
- **OnPreArrival** - Fired at an AGV's pre-arrival to a control point, i.e. the point at which the AGV would start to decelerate to stop at the control point if needed. OnPreArrival is fired prior the AGV allocating further ahead, or when the AGV starts decelerating to its final destination.
- **OnArrival** - Fired at an AGV's arrival at a control point, i.e. when the AGV has decelerated to stop at the control point, either because it could not allocate further ahead or if the control point is the final destination.

 

 

 Statistics 

AGVs can also track the following statistics:

 

- **BatteryLevel** - The AGV's battery level, as a percentage between 0 and 100.
