---
id: reference_3dobjects_taskexecuters_dispatcher_dispatcher
name: "Dispatcher"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Dispatcher","Dispatcher"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Dispatcher/Dispatcher.html"
---

# Dispatcher

## Overview and Key Concepts

 

The dispatcher is used to control a group of transporters or operators. Task sequences are sent to the dispatcher from an object and the dispatcher delegates them to the transports or operators that are connected to its output ports. The task sequence will be performed by the mobile resource that finally receives the request.

 

 

The dispatcher object performs queueing and routing logic for task sequences. Depending on the modeler's logic, task sequences can be queued up or dispatched immediately once they are given to a dispatcher.

 

You can view a dispatcher's task sequence queue during a model run by right-clicking on the dispatcher and selecting **View Task Sequence Queue**.

 

### Ports

 

You use input and output ports to associate teams of task executers. When you connect the output ports of a dispatcher to the input ports of a task executer, this makes that task executer a member of the dispatcher's "team". When the dispatcher receives a task sequence, it will dispatch the task sequence to one of the task executers connected to its output ports, depending on logic you define.

 

### Dispatching Logic

 

When a dispatcher receives a task sequence, triggered by the [TaskSequence.dispatch](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)() method, it first calls its Pass To function. This function returns the port number to pass the task sequence on to. The dispatcher will then immediately pass the task sequence on to the object connected to that port. If the function returns a 0 instead of a port number, then the task will be queued up in the dispatcher's task sequence queue. This is done by calling the queue strategy function for the task sequence. This queue strategy returns a value associated with the task sequence, and represents a priority to sort the task sequence in the queue. Higher priority values go to the front of the queue, and lower values go the back. Usually you will simply return the priority value of the task sequence, but the queue strategy function allows you to dynamically change the priority of a task sequence if needed. When ordering the task sequence in the queue, the dispatcher actually calls the queue strategy function several times, once for each task sequence in the queue, to get each priority value and compare it with the new task sequence's priority value. Once it has found the right location to put the task sequence, it ranks the new task sequence accordingly.

 

A dispatcher is a super-class of all task executers, or in other words all task executers are also dispatchers. This means that an Operator or Transporter can also act as a dispatcher or team leader, giving task sequences to other members of its team, as well as executing task sequences itself.

 

 

 

## Events

 

The dispatcher uses some of the standard events that are common to all task executers. These include:

 

- On Receive Task Sequence
- On Resource Available
- Pass To
- Queue Strategy
- On Draw
- On Reset
- On Message
- On Stop
- On Resume

 

### Pass To

 

This function is fired when the dispatcher receives a task sequence.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Task Sequence | treenode | The task sequence that was received by the dispatcher. |

 

The return value of the Pass To field should be the output port that the dispatcher will pass the task sequence to. If 0 is returned, the task sequence will automatically queue up according to the defined Queue Strategy until the task sequence can be passed to an available dispatcher or task executer. If a value greater than 0 is returned, the task sequence will be sent immediately to the returned port number. If a value of -1 is returned, then the dispatcher does nothing, but rather assumes all dispatch logic is done within the Pass To function using the [TaskSequence.move](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=move)() and [TaskSequence.dispatch](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=TaskSequence&Method=dispatch)() methods.

 

See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

The dispatcher does not implement any states.

 

 

 

## Statistics

 

The dispatcher does not track any statistics.

 

 

 

## Properties Panels

 

The Dispatcher object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- ASRS
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Dispatcher object uses the following properties:

 

| Property | Type |
| --- | --- |
| PassTo | Number |
| QueueStrategy | Number |
