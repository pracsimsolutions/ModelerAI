---
id: agv__allocatableobject
name: "AGV::AllocatableObject"
kind: class
module: "AGV"
signature: "AGV::AllocatableObject"
aliases: []
tags: ["class", "agv-module"]
deprecated: false
replacedBy: null
description: "An object that can be allocated by an AGV."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AGV\\AGV.AllocatableObject.xml"
---

# AGV::AllocatableObject

An object that can be allocated by an AGV.

An allocatable object is either a Control Point or a Control Area. The primary
properties you can get on this object are its set of allocation requests and its
set of allocations.

## Methods

### allocate

```flexscript
AGV.AllocationPoint AGV::AllocatableObject.allocate(Object allocator, int force = 0)
```

Attempts to allocate the object.

**Parameters:**
- `allocator` *(Object)* — The object that is to 'own' the allocation.
- `force` *(int, default 0)* — Should be 1 or 0. If 1, it will 'force' the allocation, meaning it will ignore the object's maximum allocations property, allocating the object even if it has reached its allocation capacity. If 0, then if the object has already reached its allocation capacity, a request will be added to its request queue.

**Returns:** An AGV.AllocationPoint that represents the allocation. You can get the allocation point's requestState property to see whether or not the allocation was completed immediately or was added to the object's list of requests.

Generally you do not need to call this method because allocations and deallocations
are managed by default by the AGV travel behavior. You would use this method if you need to allocate
a control area or control point that isn't part of the standard travel path of an AGV. For example,
you can make items in a conveyor system allocate a control area through the
OnArrival trigger of a conveyor decision point. There are pick list options that do this automatically
and call this method.

If you use this method to allocate an object, then later in the simulation you should call
deallocate on the result allocation point to
remove the allocation.
AGV.AllocationPoint alloc = controlArea.allocate(current);
if (alloc.requestState == AGV.RequestState.Requested) {
... // do stuff if the object was not allocated immediately
await alloc.event("OnAllocate");
}
... // do stuff once the control area is allocated.

## Properties

### allocations

```flexscript
AGV::AllocatableObject.allocations (readonly)
```

An array-like list of AGV.AllocationPoints that represent the set of AGVs who have currently allocated the object.

### requests

```flexscript
AGV::AllocatableObject.requests (readonly)
```

An array-like list of AGV.AllocationPoints that represent the set of AGVs currently requesting to allocate the object.

### routingConstraint

```flexscript
int AGV::AllocatableObject.routingConstraint
```

A value defining how/whether AGVs may travel through the object when calculating routing.

The routingConstraint property takes one of the values of
AGV.RoutingConstraint. Setting this value makes the AllocatableObject into a
dynamic barrier for routing purposes. The default value (AGV.RoutingConstraint.None) means AGVs
can travel through the object. Setting it to some other value, e.g. AGV.RoutingConstraint.Blocked,
will cause AGVs to route around the object, depending on the value you use.

Note that this supplements your use of conditional rules on paths. The difference is that
conditional rules work with routing table caching, while routingConstraint does not. If there is
any object in the model with a routingConstraint set to something other than AGV.RoutingConstraint.None,
then the AGV network will ignore any cached routing tables and recalculate paths anew. As such, using
routingConstraint can have significant impacts on run speed because it must recalculate the path
on every travel operation. Nevertheless, it can be useful when the potential number of dynamic
barriers is so large that route caching (and creating all the conditional rules) is impractical.

When routing constraints are set and the AGV network is building routes dynamically, it will
switch to an 'A* mode', using the
A* search algorithm to quickly find the path from the source to the destination. As soon
as it finds the best path, it will shortcut the search algorithm, not filling out the
routing table fully as it usually would when executing standard
Dijkstra's algorithm.
This is done to make the path search as efficient as possible when using dynamic barriers.

