---
id: reference_peopleobjects_3dobjects_elevatorbank_elevatorbank
name: "Elevator Bank"
kind: manual
breadcrumb: ["Reference","PeopleObjects","3DObjects","ElevatorBank","ElevatorBank"]
tags: ["manual","reference","peopleobjects","3dobjects"]
source: "manual/Reference/PeopleObjects/3DObjects/ElevatorBank/ElevatorBank.html"
---

# Elevator Bank

## Overview and Key Concepts

 

An object representing a bank of elevators for transporting people between different floors (grids) in an A* network.

 

 

The Elevator Bank is used to simulate elevators moving people between different floors of a facility. The Elevator Bank acts as a custom A* bridge between A* grids that are stacked on top of each other along a model's z-axis.

 

Create an elevator bank by dragging it from the library. Then expand its x-axis size to define the desired number of elevators. See Working with A* Navigation for more instruction on adding an Elevator Bank to your model.

 

 

 

## Events

 

### On Traveler Arrival

 

The OnTravelerArrival event fires when a traveler arrives at the elevator bank to transfer floors.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | TaskExecuter | The traveler that is arriving at the elevator bank. |
| DestFloorZ | double | The z location of the destination floor that the traveler is going to. |

 

### On Traveler Continue

 

The OnTravelerContinue event fires when a traveler is finished using the elevator bank and continues on the A* network.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | TaskExecuter | The traveler that is arriving at the elevator bank. |

 

The elevator bank also relays events of the bank elevator. When listening to these events via the elevator bank (using Process Flow's Wait For Event or Event-Triggered Source activities), you need to define the rank of the target elevator that you want to listen to.

 

 

 

## States

 

The Elevator Bank does not implement a state profile. Instead refer to Bank Elevator states.

 

 

 

## Statistics

 

The Elevator Bank does not implement any of its own statistics. Instead refer to Bank Elevator statistics.

 

 

 

## Properties Panels

 

The Elevator Bank object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- ElevatorBank
- Ports
- Triggers

 

 

 

## Bank Elevator

 

A Bank Elevator, or simply an *elevator*, is an individual elevator within an elevator bank. You will primarily interface with elevators in an elevator bank through the [ElevatorBank](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=ElevatorBank) and [ElevatorBank.Elevator](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=ElevatorBank.Elevator) FlexScript interfaces. However, you may also want to listen to an individual elevator's events, or collect state information on it.

 

 

 

## Elevator Events

 

### On Floor Arrival

 

The OnFloorArrival event fires when the elevator arrives at a floor.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Floor Z | double | The z location of the floor at which the elevator has arrived. |

 

### On Floor Departure

 

The OnFloorDeparture event fires when the elevator departs a floor.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Next Floor Z | double | The z location of the next floor that the elevator is going to. |
| Last Floor Z | double | The z location of the floor that the elevator is departing from. |

 

### On Idle

 

The OnIdle event fires when the elevator finishes at a floor and does not find another floor to go to.

 

This event does not have any associated parameters

 

### On Traveler Entry

 

The OnTravelerEntry event fires when a traveler enters the elevator.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | TaskExecuter | The traveler that is entering the elevator. |

 

### On Traveler Exit

 

The OnTravelerExit event fires when a traveler exits the elevator.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Traveler | TaskExecuter | The traveler that is exiting the elevator. |

 

 

 

## Elevator General States

 

The elevator's general state profile uses the following states.

 

### Idle

 

The elevator is idle and stationary.

 

### Busy

 

The elevator is currently opening or closing its door.

 

### Loading

 

Travelers are entering the elevator.

 

### Unloading

 

Travelers are exiting the elevator.

 

### Travel Empty

 

The elevator is moving between floors and does not contain any travelers.

 

### Travel Loaded

 

The elevator is moving between floors and contains one or more travelers.

 

## Elevator Custom Profile States

 

The elevator includes a custom profile, profile 1, that uses the following states.

 

### Idle

 

The elevator is idle and stationary.

 

### Opening Door

 

The elevator is opening its doors.

 

### Closing Door

 

The elevator is closing its doors.

 

### Exiting Travelers

 

Travelers are exiting the elevator.

 

### Entering Travelers

 

Travelers are entering the elevator.

 

### Moving

 

The elevator is moving to a floor to fulfill a request.

 

### Idle Moving

 

The elevator is moving to a floor but not to fulfill a request. This state will only be used if you expressly tell the elevator to move to a "home" location when it becomes idle.

 

 

 

## Elevator Statistics

 

The elevator uses the same statistics as a task executer. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Traveler Events

 

A* Travelers that use the elevator bank will also have the following events that you can listen to.

 

### On Elevator Bank Arrival

 

The OnElevatorBankArrival event fires when the traveler arrives at an elevator bank to move between floors. It is fired at the same time as the elevator bank's OnTravelerArrival event, but here the event is owned by the traveler, not the elevator bank.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Elevator Bank | ElevatorBank | The elevator bank that the traveler is arriving at. |
| Dest Floor Z | double | The z location of the traveler's destination floor. |

 

### On Elevator Entry

 

The OnElevatorEntry event fires when the traveler enters an elevator. It is fired at the same time as the elevator's OnTravelerEntry event, but here the event is owned by the traveler, not the elevator.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Elevator | ElevatorBank.Elevator | The elevator that the traveler is entering. |

 

### On Elevator Exit

 

The OnElevatorExit event fires when the traveler enters an elevator. It is fired at the same time as the elevator's OnTravelerExit event, but here the event is owned by the traveler, not the elevator.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Elevator | ElevatorBank.Elevator | The elevator that the traveler is exiting. |

 

### On Elevator Bank Continue

 

The OnElevatorBankContinue event fires when the traveler continues from an elevator bank, resuming normal A* travel. It is fired at the same time as the elevator bank's OnTravelerContinue event, but here the event is owned by the traveler, not the elevator bank.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Elevator Bank | ElevatorBank | The elevator bank that the traveler is arriving at. |
