---
id: conveyor
name: "Conveyor"
kind: class
module: ""
signature: "Conveyor"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that provides properties and methods for monitoring and controlling a conveyor."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.xml"
---

# Conveyor

A class that provides properties and methods for monitoring and controlling a conveyor.

## Methods

### estimateConveyTime

```flexscript
double (static) Conveyor.estimateConveyTime(Object origin, double n_a, Object dest, double n_a, double itemLength, int flags = 0)
```

Estimates the time for an item to travel from one part of the conveyor system to another.

**Parameters:**
- `origin` *(Object)* — The origin point from which to estimate the convey time. This must be either a conveyor or a point on a conveyor, such as a decision point or photo eye.
- `n_a` *(double)* — Reserved for future use. Must be 0.
- `dest` *(Object)* — The destination point for the time estimation. This must be either a conveyor or a point on a conveyor, such as a decision point or photo eye.
- `n_a` *(double)* — Reserved for future use. Must be 0.
- `itemLength` *(double)* — Defines an item length for the estimation algorithm. This can affect the result depending on how speeds are defined in conveyor transfers.
- `flags` *(int, default 0)* — This can either be 0, or CONV_ESTIMATE_STANDARD_SPEEDS. If 0 (default), the estimation algorithm will assume slug-build conveyors will convey at their slug release speed. If CONV_ESTIMATE_STANDARD_SPEEDS is defined, the estimation algorithm will use normal conveying speeds.

**Returns:** The estimated simulation time it will take for an item of the given size to convey from the point where the item's center is at the origin point, to the point where the item's center is at the destination point, assuming no blockage.

DecisionPoint src = current;
DecisionPoint dest = current.outObjects[1];
Object item = param(1);
double conveyTime = Conveyor.estimateConveyTime(src, 0, dest, 0, item.size.x);
In making the estimation, the algorithm will always assume default conveyor speeds (not current conveyor speeds).
Estimation speeds for slug-building conveyors are determined by the flags parameter.

### sendItem

```flexscript
void (static) Conveyor.sendItem(Object item, Object dest)
```

Sets an item's target destination in the conveyor system.

**Parameters:**
- `item` *(Object)* — The item whose target destination is to be set.
- `dest` *(Object)* — The target destination. This must be a reference to a point within the conveyor system that is reachable from the item's current position on the conveyor system. This must either be a decision point, a photo eye, a station, or an exit transfer.

Conveyor.sendItem(item, current.outObjects[1]);
Once an item's target destination is set, the item will take the shortest distance route, in
the conveyor system, to that defined destination.

## Properties

### acceleration

```flexscript
double Conveyor.acceleration (readonly)
```

Gets the conveyor's acceleration.

### currentSpeed

```flexscript
double Conveyor.currentSpeed (readonly)
```

Gets the conveyor's current speed.

### deceleration

```flexscript
double Conveyor.deceleration (readonly)
```

Gets the conveyor's deceleration.

### defaultSpeed

```flexscript
double Conveyor.defaultSpeed (readonly)
```

Gets the conveyor's default speed, which is the speed the conveyor starts with at the beginning of the simulation.

### itemData

```flexscript
Conveyor.itemData (readonly)
```

Accesses conveyor-specific data associated with items on the conveyor

This gives access to the set of Conveyor.Items associated with the
items currently on the conveyor.
Use itemData.length to get the number of items on the conveyor.
Conveyor conveyor = param(1);
int numItems = conveyor.itemData.length;
Note that itemData.length may be different than conveyor.subnodes.length, as some items may be partially on the
conveyor, but not in the conveyor's subtree. conveyover.itemData.length should always be greater than or equal to
conveyor.subnodes.length.
conveyor.subnodes.length; // the number of items in the conveyor's subtree
conveyor.itemData.length; // the total number of all items partially-or-wholly on the conveyor.
To access item data by rank, you can index into conveyor.itemData with the [ ] operator.
conveyor.itemData[1]; // gets the data associated with the most downstream item on the conveyor
conveyor.itemData[conveyor.itemData.length]; // gets the data associated with the most upstream item on the conveyor
You can also access the itemData associated with a specific item, again using the [ ] operator.
Object item = param(2);
Conveyor.Item conveyorItem = conveyor.itemData[item];
See Conveyor.Item for more information.

### length

```flexscript
double Conveyor.length (readonly)
```

Gets the conveyor's simulated length.

If the conveyor does not use a virtual length, this is the geometric length of the conveyor. If the conveyor uses a virtual
length, it is the virtual length.

### motor

```flexscript
Object Conveyor.motor (readonly)
```

Gets the conveyor's assigned motor.

### slugBuilder

```flexscript
Conveyor.SlugBuilder Conveyor.slugBuilder (readonly)
```

Gets data associated with the conveyor's slug-building status.

This object only exists if the conveyor is connected to a merge controller object.

### targetSpeed

```flexscript
double Conveyor.targetSpeed
```

Gets or sets the conveyor's current target speed.

If you set this value, the conveyor's speed will
change to the target speed. The conveyor's acceleration/deceleration will determine how long it takes to
get to the target speed.

Reversing Conveyor Direction
If you set a conveyor's targetSpeed to a non-zero negative value, the conveyor will reverse direction. When a conveyor's direction
is reversed the following things happen.

All stopped items on the conveyor will be fully resumed.
Each item on the conveyor will be set to a moving state.
Each item's target destination will be erased. For example, if the item previously had been sent to
a point in the system using the
Conveyor.sendItem() method,
that destination will be forgotten.
Each item will regenerate its list of traversal points, e.g. photo eyes, decision points,
etc. as if the item is entering the conveyor at the point where it is when the conveyor is
reversed.

Specific caveats that you should be aware of:

If an item is straddling1 two inline transfers when direction reversal happens, then both conveyors
that the item is straddling1 must be reversed at the same time. Doing otherwise will result in undefined
behavior.
An item cannot straddle1 more than two conveyors when direction reversal happens, or else an exception
will be thrown.
No updates are made to the internal routing table when a conveyor's direction is reversed. This means,
for routing purposes, i.e. when sending an item to a destination point in the system, the conveyor is
always assumed to be conveying in its original direction.

1Straddle: when any part of an item's entry space is over more than one conveyor at the same time.

### width

```flexscript
double Conveyor.width (readonly)
```

Gets the conveyor's width.

