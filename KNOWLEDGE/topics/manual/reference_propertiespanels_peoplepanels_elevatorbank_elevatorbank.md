---
id: reference_propertiespanels_peoplepanels_elevatorbank_elevatorbank
name: "Elevator Bank"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","PeoplePanels","ElevatorBank","ElevatorBank"]
tags: ["manual","reference","propertiespanels","peoplepanels"]
source: "manual/Reference/PropertiesPanels/PeoplePanels/ElevatorBank/ElevatorBank.html"
---

# Elevator Bank

The Elevator Bank panel defines the logic of the elevators in the bank.

 

 

The following properties are on the Elevator Bank panel:

 

### Max Speed, Acceleration, Deceleration

 

Define the speed settings of the elevators in the elevator bank.

 

### Door Open/Close Time

 

Define the time it takes to open and close an elevator door. For opening, this is the duration applied from the time the elevator arrives at a floor, to the time that the first person starts to exit/enter the elevator. For closing, this is the duration applied from the time the last person finishes entering/exiting the elevator to the time the elevator starts moving to the next floor.

 

### Wait Time

 

Define the minimum amount of time the elevator will remain on a floor with its door open.

 

### Max Entries/Exits

 

Define the maximum number of people who can be entering/exiting the elevator simultaneously. This value is used in conjunction with the **Entry/Exit Time** property to determine the total time it takes for multiple people to enter/exit the elevator.

 

#### Entry/Exit Algorithm

 

The elevator bank uses a simple algorithm for sequencing the entries and exits of an elevator. If we represent *e* as the individual entry/exit time, and *s* as the number of simultaneous entries/exits, then *e*/*s* determines the duration from the time that one person may start to enter/exit the elevator to the time a next person may start to enter/exit the elevator. For example, if the entry/exit time is 5 seconds, and the number of simultaneous entries/exits is 2, then there will be a 2.5 second gap between each person starting to exit the elevator. If there are multiple people needing to exit the elevator, starting for example at time 100, then the first person will start to exit at time 100. The second person will start to exit at time 102.5. The third person will start to exit at time 105, when the first person finishes exiting, and so on. In this scenario, where there are for example 5 people needing to exit, the first person will start exiting at time 100, and the last person will finish exiting at time 115.

 

When exiting/entering, elevators will first have all people exit who are to exit at that floor. Then once all people are finished exiting, people who need to enter will start to enter. In other words, people do not exit and enter simultaneously. Instead, people first exit, then people enter.

 

### Elevator Capacity

 

Define the maximum capacity of an elevator. This is defined in whatever units of measure are used in the **Traveler Capacity Usage** property.

 

### Traveler Capacity Usage

 

Define how much of an elevator's capacity a single traveler takes up. The default is to use the x/y footprint of the traveler plus the x/y footprint of anything the traveler is carrying.

 

### Entry/Exit Time

 

Define the amount of time it takes for a person to enter/exit the elevator. This time is used in conjunction with the **# Simultaneous Entries/Exits** property to determine the total amount of time it takes for multiple people to enter/exit the elevator.

 

### Request List Pull Query

 

Define the query used by an available elevator to determine the next floor to go to.

 

The Elevator Bank manages a list of currently active requests. You might think of a specific request as corresponding to a pressed/lighted button at the entrance to an elevator bank, or a pressed/lighted button inside an elevator. When a person arrives at an Elevator Bank, the person will press a button to go either up or down. The pressing of the button creates a new request for an elevator to come to that floor. Also, when a person enters an elevator, s/he presses a button for the destination floor to go to. This creates another new request for that specific elevator to go to the target floor.

 

The Elevator Bank internally contains a List that references all active requests. When an elevator needs to decide which floor to go to next, it pulls from this list, using the query you define here, and then moves to whichever floor is chosen from the pull operation.

 

You can view the properties of the request list by pressing **Request List Properties...** Alternatively you can view the current list of requests by pressing **View Request List...**.

 

The elevator bank's request list uses custom aliases in defining the request list pull query. Instead of using the default terms *value* and *puller*, this list uses the terms *request* and *elevator*, since the values of this list are associated with elevator requests, and the pullers of the list are the elevators. Additionally, the *request* term is of the FlexScript type [ElevatorBank.Request](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=ElevatorBank.Request) and the *elevator* term is of the type [ElevatorBank.Elevator](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=ElevatorBank.Elevator). Thus your pull query can directly use any method or property associated with these interfaces.

 

#### The Default Pull Query

 

The default request list pull query is as follows

 

```
WHERE 
  (request.onElevator == 0 OR request.onElevator == elevator)
  AND elevator.canStop(request.floorZ, request.travelDirection)
ORDER BY distance ASC
```

 

In this default query, the WHERE clause has two criteria. First is the expression `(request.onElevator == 0 OR request.onElevator == elevator)`. Note that a request is like a pressed/lighted button, either inside an elevator or at a floor's entrance to an elevator bank. This expression means that the elevator should only go to a lighted button that is either not on any elevator (`request.onElevator == 0`, i.e. the button is at a floor, outside any specific elevator) or a button that is on that specific elevator (`request.onElevator == elevator`). In other words, the elevator should never go to a request that was pushed inside a different elevator.

 

The second requirement of the where clause is `elevator.canStop(request.floorZ, request.travelDirection)`. This means that the elevator should only take requests that, first, are in the direction that the elevator is currently already going, and second, are far enough ahead of the elevator (if the elevator is currently moving) that the elevator can still stop at the floor, given its current speed and deceleration.

 

The ORDER BY clause of the default query simply prioritizes the requests that are closest to the elevator.

 

### Request List Properties

 

Opens the properties window of the internal request list.

 

### View Request List

 

Opens the Entries window of the internal request list.
