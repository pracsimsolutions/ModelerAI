---
id: elevatorbank__elevator
name: "ElevatorBank::Elevator"
kind: class
module: "ElevatorBank"
signature: "ElevatorBank::Elevator"
aliases: []
tags: ["class", "elevatorbank-module"]
deprecated: false
replacedBy: null
description: "A class that represents an elevator member of an ElevatorBank."
seeAlso: []
source: "help\\FlexScriptAPIReference\\People\\ElevatorBank.Elevator.xml"
---

# ElevatorBank::Elevator

A class that represents an elevator member of an ElevatorBank.

## Methods

### canStop

```flexscript
int ElevatorBank::Elevator.canStop(double floorZ, int travelDirection)
```

Returns true if the elevator can stop at the target z location and desired travel direction, given the elevator's current location and travel direction.

**Parameters:**
- `floorZ` *(double)* — The desired z location to stop at.
- `travelDirection` *(int)* — The desired travel direction for a request. 1 means going up, -1 means going down, 0 means to ignore desired travel direction.

**Returns:** 1 if the elevator can stop at the location, 0 otherwise.

This method uses several factors to determine whether the elevator can stop at a given location, as follows:

If the elevator is currently idle, it will always return true.
If the elevator is not idle, then the desired travel direction must match the elevator's travel
direction, and the target z must be "ahead of it" given the elevator's
current travel direction. In other words, if the elevator is currently going down, then the target
z must be below the elevator's current location. If the elevator is going up, the target z must be
above the elevator's current location.
If the elevator is currently moving, then the target z location must be far enough ahead of the
elevator so that the elevator can stop at the target z (given its current speed and deceleration)
without going past it.

### moveToFloor

```flexscript
void ElevatorBank::Elevator.moveToFloor(double floorZ)
```

Tells the elevator to move to the target floor.

**Parameters:**
- `floorZ` *(double)* — The desired z location to move to.

**Returns:** True if the elevator can stop at the location.

Since the elevator implements automatic logic for moving between floors, generally you will only use this
method in the following situations.

When the elevator becomes idle (the OnIdle event), you may want to send the elevator to a home
location.
If you want to preempt the elevator's normal behavior, because of some abnormal occurrence.

## Properties

### bank

```flexscript
ElevatorBank ElevatorBank::Elevator.bank (readonly)
```

A reference to the ElevatorBank associated with this elevator.

### capacity

```flexscript
double ElevatorBank::Elevator.capacity (readonly)
```

The total capacity. This value is entered by the user in the elevator bank properties.

### isGoingDown

```flexscript
int ElevatorBank::Elevator.isGoingDown (readonly)
```

A boolean telling if the elevator is currently going down.

### isGoingUp

```flexscript
int ElevatorBank::Elevator.isGoingUp (readonly)
```

A boolean telling if the elevator is currently going up.

### lastFloorZ

```flexscript
double ElevatorBank::Elevator.lastFloorZ (readonly)
```

The z location of the floor that the elevator most recently stopped at.

### nextFloorZ

```flexscript
double ElevatorBank::Elevator.nextFloorZ (readonly)
```

The z location of the floor that the elevator was most recently dispatched to.

### travelDirection

```flexscript
int ElevatorBank::Elevator.travelDirection (readonly)
```

An integer signifying the elevator's current travel direction. 1 means going up, -1 means going down. 0 means the elevator is idle.

### usedCapacity

```flexscript
double ElevatorBank::Elevator.usedCapacity (readonly)
```

The currently used capacity, i.e. the sum of the capacity usages of each traveler currently on the elevator.

