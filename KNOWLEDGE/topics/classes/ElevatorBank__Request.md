---
id: elevatorbank__request
name: "ElevatorBank::Request"
kind: class
module: "ElevatorBank"
signature: "ElevatorBank::Request"
aliases: []
tags: ["class", "elevatorbank-module"]
deprecated: false
replacedBy: null
description: "A class that represents a request for an ElevatorBank.Elevator."
seeAlso: []
source: "help\\FlexScriptAPIReference\\People\\ElevatorBank.Request.xml"
---

# ElevatorBank::Request

A class that represents a request for an ElevatorBank.Elevator.

You might think of a specific request as corresponding to a pressed/lighted button at the entrance
to an elevator bank, or a pressed/lighted button inside an elevator. When a person arrives at an
ElevatorBank, the person will press a button to go either up or down. The pressing
of the button creates a new request for an elevator to come to that floor. Also, when a person
enters an elevator, s/he presses a button for the destination floor to go to. This creates another new
request for that specific elevator to go to the target floor.

The ElevatorBank manages an array of all currently active requests, in its
ElevatorBank.requests property.
It also pushes the request onto a List. When elevators need to decide which
floor to go to next, they pull from this list, using a user-defined pull query, and then move to
whichever floor is chosen from the pull operation.

## Properties

### bank

```flexscript
ElevatorBank ElevatorBank::Request.bank (readonly)
```

A reference to the ElevatorBank associated with this request.

### creationTime

```flexscript
double ElevatorBank::Request.creationTime (readonly)
```

The time that the request was created, i.e. the time that the person pushed the button.

### floorZ

```flexscript
double ElevatorBank::Request.floorZ (readonly)
```

The z location of the request's target floor.

If the request corresponds to a button
outside the elevator (onElevator = 0), then floorZ is the floor location at which to pick up
the person. If the requests corresponds to a button inside an elevator (onElevator != 0)
then floorZ is the location of the floor that the person wants to go to.

### isGoingDown

```flexscript
int ElevatorBank::Request.isGoingDown (readonly)
```

A boolean telling if the request is to go down.

### isGoingUp

```flexscript
int ElevatorBank::Request.isGoingUp (readonly)
```

A boolean telling if the request is to go up.

### onElevator

```flexscript
ElevatorBank.Elevator ElevatorBank::Request.onElevator (readonly)
```

If not 0, the elevator in which the corresponding button was pushed. In other words, the elevator that the person was on when s/he pushed the button.

### travelDirection

```flexscript
int ElevatorBank::Request.travelDirection (readonly)
```

An integer signifying the request's travel direction. 1 means going up, -1 means going down.

