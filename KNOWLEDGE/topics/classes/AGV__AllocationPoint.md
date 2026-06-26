---
id: agv__allocationpoint
name: "AGV::AllocationPoint"
kind: class
module: "AGV"
signature: "AGV::AllocationPoint"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "A structure containing information about a planned AGV allocation."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.AllocationPoint.xml"
---

# AGV::AllocationPoint

A structure containing information about a planned AGV allocation.

Each time an AGV starts a travel operation, it builds a
travel path to the
destination. In this path building phase, as it encounters allocatable objects on that path,
it creates allocation points that signify that the AGV must allocate that object at a
defined point on the path. An allocation point either represents an allocation of a
Control Point, a
Control Area, or a path
transfer when traversing an accumulating path.

The AGV class includes
an allocationPoints property that is an array-like
structure that includes
all allocation points (that have not yet allocated their object) on the AGV's current travel path.
Also, allocation points are the elements you access in an
AGV.AllocatableObject's
requests and
allocations properties.

//Printing an AGV's allocation points
AGV agv = AGV(current);
for (int i = 1; i <= agv.allocationPoints.length; i++) {
AGV.AllocationPoint point = agv.allocationPoints[i];
print(
" agv ", point.agv.as(treenode),
" object ", point.object,
" stopDist ", point.stopDist,
" arrivalDist ", point.arrivalDist,
" transferPath ", point.transferPath,
" transferDistAlong ", point.transferDistAlong);
}

## Methods

### deallocate

```flexscript
void AGV::AllocationPoint.deallocate()
```

Deallocates the associated object, destroys the allocation point, and fulfills the object's next waiting allocation request if there is one.

You should only call this if you have programmatically allocated the object with
AGV.AllocatableObject.allocate().
Otherwise, deallocation will happen automatically. If you call this method when its
requestState is anything but Allocated, it
will merely destroy the allocation point, effectively cancelling the request.

### event

```flexscript
Event AGV::AllocationPoint.event(string eventName)
```

Replicates Object.event for an allocation point.

**Parameters:**
- `eventName` *(string)* — The name of the event.

**Returns:** The resulting Event.

Since AGV.AllocationPoint is not an Object, it doesn't naturally have this method, so it is replicated
explicitly. The only event available on an AGV.AllocationPoint is "OnAllocate". In other words, you use this
method in code to await the completion of the allocation.
AGV.AllocationPoint alloc = controlArea.allocate(current);
if (alloc.requestState == AGV.RequestState.Requested) {
... // do stuff if the object was not allocated immediately
await alloc.event("OnAllocate");
}
... // do stuff once the control area is allocated.

## Properties

### allocator

```flexscript
Object AGV::AllocationPoint.allocator (readonly)
```

The object that is to 'own' the allocation.

Usually this is the AGV that allocates the object. However, sometimes it may be
an A* traveler if the control area is an A* member, or it could be an object you define
yourself when you call
AGV.AllocatableObject.allocate().

### arrivalDist

```flexscript
double AGV::AllocationPoint.arrivalDist (readonly)
```

The AGV travel distance at which the AGV will have 'arrived' at the target object.

This will nearly always be farther along the path than the
stopDist as the AGV must allocate
an object before traveling to it.

### object

```flexscript
AGV.AllocatableObject AGV::AllocationPoint.object (readonly)
```

The object that is to be allocated.

This will be a valid pointer if the allocation point is for a
Control Point or Control Area, and null otherwise.

### requestState

```flexscript
AGV.RequestState AGV::AllocationPoint.requestState (readonly)
```

The state of the allocation point's request.

### stopDist

```flexscript
double AGV::AllocationPoint.stopDist
```

The AGV travel distance at which the AGV will stop if it cannot allocate the object.

You can set this property value, but only at defined times in the
AGV's travel operation. Specifically, you should only set it after the OnBuildTravelPath event is
fired (or as part of that trigger) but before the OnPreAllocate. Setting it at any other
time is not guaranteed to work. Note that the AGV.allocationPoints
array is sorted in the order of the stopDist value, so if you set
this value it may change the index of the allocation point within the AGV's allocationPoints
array, in order to preserve this ordering.

### transferDistAlong

```flexscript
double AGV::AllocationPoint.transferDistAlong (readonly)
```

The distance along the allocating transfer's owning path.

This will be a valid distance if the allocation point is for a
path transfer, and undefined otherwise. In other words, you should only access
this property if you know the allocation point is for a transfer
(transferPath is not null).

### transferPath

```flexscript
Object AGV::AllocationPoint.transferPath (readonly)
```

The owning path of the transfer that will be allocated.

This will a valid pointer if the allocation point is for a
path transfer, and null otherwise.

