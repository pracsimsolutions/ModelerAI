---
id: reference_3dobjects_taskexecuters_elevator_elevator
name: "Elevator"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Elevator","Elevator"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Elevator/Elevator.html"
---

# Elevator

## Overview and Key Concepts

 

The elevator is a special type of transport that moves flow items up and down. It will automatically travel to the level where flow items need to be picked up or dropped off. Flow items are animated as they enter and exit the elevator. This gives a better feel for the load and unload time of the elevator.

 

 

The elevator is a task executer. It implements offset travel by only traveling the z portion of the offset location. If the offset travel is for a load or unload task, then once the offsetting is finished, it will use the user-specified load/unload time to convey the flow item onto its platform, or off of its platform to the destination location. When conveying the item onto or off of the elevator, the flow item moves directly along the elevator's x-axis.

  

#### Conveying a Flow Item Onto an Elevator

 

For a load task, the conveying of the item onto the platform may not work if the flow item is in an object that continuously refreshes the location of the flow item, like a conveyor for example. In this case, if you want the conveying of the item onto the platform to show up, then make sure that the elevator is ranked before the object it is picking up from in the model tree (the elevator must be higher up in the tree).

  

### Context

 

Since the main distinction of an elevator is that it only moves along its z axis, this object can be used for any purpose in which you want the object to only travel along one axis.

 

 

 

## Events

 

The elevator uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The elevator uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Elevator object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Elevator object shares several properties with all task executers:

 

- Task Executer Properties
