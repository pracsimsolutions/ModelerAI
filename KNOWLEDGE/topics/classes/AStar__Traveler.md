---
id: astar__traveler
name: "AStar::Traveler"
kind: class
module: "AStar"
signature: "AStar::Traveler"
aliases: []
tags: ["class", "astar-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to an A* traveler."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AStar\\AStar.Traveler.xml"
---

# AStar::Traveler

Provides access to data and methods related to an A* traveler.

## Methods

### addAllocation

```flexscript
AStar::Traveler.addAllocation(AStar.Allocation alloc, int force, int notifyPendingAllocations)
```

For future use. Adds an accumulation allocation to the traveler's scheduled cell allocations.

**Parameters:**
- `alloc` *(AStar.Allocation)* — The target allocation to add.
- `force` *(int)* — Pass 1 to force the allocation to be added regardless of collisions with other allocations, 0 otherwise.
- `notifyPendingAllocations` *(int)*

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc. For normal users, this method is automatically called by
navigatePath().

### arriveAtBridge

```flexscript
AStar::Traveler.arriveAtBridge(int pathIndex)
```

For future use. Notifies A* that the traveler has arrived at a bridge/elevator associated with a specific cell in its travel path, handing over control to the bridge.

**Parameters:**
- `pathIndex` *(int)* — The index of the travel path associated with the bridge.

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc.

### AStar.Traveler

```flexscript
AStar.Traveler AStar::Traveler.AStar.Traveler(treenode te)
```

Constructs an AStar.Traveler object based on the given TaskExecuter.

**Parameters:**
- `te` *(treenode)* — A TaskExecuter that is connected to the A* network as an A* Traveler.

Casting from a treenode is not correct.
AStar.Traveler traveler = AStar.Traveler(te); // correct
AStar.Traveler traveler = te; // incorrect

### calculatePath

```flexscript
AStar::Traveler.calculatePath(Object object, int flags = 0)
```

For future use. Calculates the A* path to the destination and stores it in the traveler's travelPath property.

**Parameters:**
- `object` *(Object)* — The target destination object.
- `flags` *(int, default 0)* — Configuration flags for the calculation.

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc.

### findCollision

```flexscript
AStar::Traveler.findCollision(AStar.Allocation alloc, int ignoreSameTravelerAllocs)
```

For future use. Finds an existing allocation that "collides" with the passed allocation.

**Parameters:**
- `alloc` *(AStar.Allocation)* — The target allocation to add.
- `ignoreSameTravelerAllocs` *(int)* — If 1, the search will not look at allocations for this traveler.

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc.

### finishPath

```flexscript
AStar::Traveler.finishPath()
```

For future use. Notifies A* that the traveler is finished traveling.

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc.

### getAllocations

```flexscript
AStar::Traveler.getAllocations(double time = -1)
```

Gets an array of AStar.Allocations that the traveler has currently allocated or is scheduled to have allocated at a given time.

**Parameters:**
- `time` *(double, default -1)* — The target time for retrieving the set of allocations.

The allocations can be accessed as an array, including using the length property and array accessor.
var allocations = traveler.getAllocations();
for (int i = 1; i <= allocations.length; i++) {
AStar.Cell cell = allocations[i].cell;
}
Note that a traveler's allocations are correlated with, yet distinct from, a traveler's travelPath.
Each entry in a traveler's path can have up to 3 allocations associated with it, specifically when the traveler is traveling diagonally. For example,
if a traveler's travelPath includes a traversal from cell (4, 4) to cell (5, 5), the traveler would need to allocate, as part of traversing that
diagonal, cells (4, 5) and (5, 4), in addition to the destination cell (5, 5).

### navigatePath

```flexscript
AStar::Traveler.navigatePath(int startAtIndex)
```

For future use. Initiates the logic that builds kinematics and cell allocations along the traveler's calculated path.

**Parameters:**
- `startAtIndex` *(int)*

This method call is meant for advanced users who are implementing their
own A* allocation/navigation schemes. You should only call this method if you are
overriding default path calculation/navigation mechanism by listening to traveler events
like OnCalculatePath, etc.

## Properties

### isActive

```flexscript
int AStar::Traveler.isActive (readonly)
```

Returns 1 if the traveler is actively performing a travel task, 0 otherwise.

### isBlocked

```flexscript
int AStar::Traveler.isBlocked (readonly)
```

Returns 1 if the traveler is currently blocked by an accumulation cell allocation request.

### object

```flexscript
TaskExecuter AStar::Traveler.object (readonly)
```

Returns the TaskExecuter that is associated with this A* traveler.

### travelPath

```flexscript
AStar::Traveler.travelPath (readonly)
```

Accesses the traveler's current travel path.

A traveler's travelPath describes the current sequence of A* grid cells that
the traveler will traverse in traveling to its current destination. The travelPath can
be accessed using array-type usage, including using the "length" property and array accessors.
for (int i = 1; i <= traveler.travelPath.length; i++) {
AStar.Cell cell = traveler.travelPath[i].cell;
...
}
Each element of the travelPath includes the AStar.Cell to be traversed.
AStar.Cell cell = traveler.travelPath[1].cell;
The travelPath also includes an indexOf() method to find which index (if any) corresponds to
a given grid cell.
int index = traveler.travelPath.indexOf(cell);
The travelPath also includes an isBlocked() method. This would be used when you dynamically
change the A* grid and want to see if you should preempt the traveler to have it
recalculate its path based on the new grid change.

if (traveler.travelPath.isBlocked()) {
...
}

