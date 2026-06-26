---
id: astar__navigator
name: "AStar::Navigator"
kind: class
module: "AStar"
signature: "AStar::Navigator"
aliases: []
tags: ["class", "astar-module"]
deprecated: false
replacedBy: null
description: "Provides access to data and methods related to a model's A* navigator."
seeAlso: []
source: "help\\FlexScriptAPIReference\\AStar\\AStar.Navigator.xml"
---

# AStar::Navigator

Provides access to data and methods related to a model's A* navigator.

## Methods

### addDynamicBarrier

```flexscript
Variant AStar::Navigator.addDynamicBarrier(Variant val, int skipOnChange = 0)
```

Adds a dynamic barrier to the navigator's A* grid network.

**Parameters:**
- `val` *(Variant)* — The value containing the barrier(s) to add. The value can either by a reference to an object, an array of references to objects, or a 3x3 array of numbers. If it is an object, it will add that object as a dynamic barrier. If it is an array of objects, it will add each object in the array as a barrier. If it is a 3x3 number array, it will construct 3 Vec3s from the array and forward to the Vec3-based overload.
- `skipOnChange` *(int, default 0)* — Optional. If 1, the call to the navigator's OnGridChange event will be skipped.

This method lets you dynamically change the A* path calculation mechanism
during a simulation run. It is more flexible than the standard
barrier condition rule in that
you can add and remove barriers that have been dynamically created, or
that are moving around during the simulation. You call this method to integrate the object's
current location as a barrier into the navigation grid.

Calling this method will only affect the path calculation algorithm. It does not
affect the paths of objects who are actively traveling on their path. If you want them to
recalculate their path after adding the barrier, you should preempt them so they will
restart their travel operation. You can query whether an active traveler's path has been
blocked by a dynamic barrier by calling AStar.Traveler(object).travelPath.isBlocked().

The return value is a reference to a treenode that represents the added dynamic barrier.
If you pass an array of object references into this method, then an array of treenodes will
be returned. Once the barrier is destroyed or moves, you should call
removeDynamicBarrier() to remove it, passing
in the value that was returned by this method.

### getCell

```flexscript
AStar.Cell AStar::Navigator.getCell(Vec3 modelLoc)
```

Gets the cell associated with a given model location.

**Parameters:**
- `modelLoc` *(Vec3)* — The target model location.

AStar.Cell cell = AStar.navigator.getCell(Vec3(10, 3, 0));

### getLocation

```flexscript
Vec3 AStar::Navigator.getLocation(AStar.Cell cell)
```

Gets the model location associated with a given AStar.Cell.

**Parameters:**
- `cell` *(AStar.Cell)* — The target cell.

Vec3 loc = AStar.navigator.getLocation(traveler.travelPath[1].cell);

### removeDynamicBarrier

```flexscript
AStar::Navigator.removeDynamicBarrier(Variant barrier)
```

Removes a dynamic barrier that was added with addDynamicBarrier().

**Parameters:**
- `barrier` *(Variant)* — The value that was returned from a call to addDynamicBarrier().

## Properties

### travelers

```flexscript
AStar::Navigator.travelers (readonly)
```

References the set of travelers that are members of the A* network.

This property
is available so you can visit each of the navigator's travelers when something like
a grid change happens.

