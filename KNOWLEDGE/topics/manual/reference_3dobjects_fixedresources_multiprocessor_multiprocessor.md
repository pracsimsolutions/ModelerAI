---
id: reference_3dobjects_fixedresources_multiprocessor_multiprocessor
name: "MultiProcessor"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","MultiProcessor","MultiProcessor"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/MultiProcessor/MultiProcessor.html"
---

# MultiProcessor

## Overview and Key Concepts

 

The multiprocessor is used to simulate the processing of flow items in sequentially ordered operations. The user defines a set of processes for each multiprocessor object. Each flow item that enters will go through each process in sequence. Multiprocessors may call for operators during their process steps.

 

 

The multiprocessor is a fixed resource. It receives one flow item, puts the flow item through its sequence of processes, then releases the flow item. Once the flow item has exited the multiprocessor, it receives another flow item and goes through the processes again. Only one flow item will ever be in the multiprocessor at one time.

 

For each process that you define, you can specify a name for the process, a process time, a number of operators to use for that process, priority and preempting values for the tasks sent to the operators, and the operator or dispatcher to send operator tasks to. At the beginning of each process, the multiprocessor calls the process time field, sets its state to the name of the process, and calls operators if the number of operators value is greater than 0. When the process is finished, the multiprocessor releases all operators called for that process, and calls the process finish trigger. It also passes the process number into the process finish trigger as parval(2).

 

### When Should I Use a MultiProcessor?

 

You would use the multiprocessor if you have one station that involves several operations with separate process times and/or different resources. You can also use the multiprocessor as a shared station for different types of operations. For example, itemtype 1 needs to go through operations A, B, C, and D, and itemtype 2 needs operations E, F, G, and H, but both itemtypes must share one station for their processes. Give the multiprocessor 8 processes: A - H, and for itemtype 1 have processes E - H have 0 process time, and for itemtype 2 have processes A - D have 0 process time.

  

#### The Multiprocessor Does Not Accommodate Piping of Processes

 

Piping happens if, once a flow item gets finished with process 1 and moves to process 2, another flow item can take up process 1. Thus several flow items can be "flowing down the pipe" at any given time. If you would like to simulate this, use several Processor objects connected in sequence.

  

 

 

## Events

 

For information on events, see the Event Listening page.

 

The queue uses the standard events that are common to all fixed resources. See Fixed Resources - Events for an explanation of these events.

 

The combiner has no additional events.

 

 

 

## States

 

For statistical purposes, the multiprocessor will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Idle

 

There are no flow items being processed.

 

### User-defined States

 

These are states defined by the user, one for each process.

  

#### Getting Statistics for User-defined States

 

The Multiprocessor state profile differs from the Default state profile. To view these individual User-defined states in a State Chart, create a custom State Table with the User-defined states in it and reference the Multiprocessor's state profile.

  

### Blocked

 

The multiprocessor has finished all processes for a flow item, released it, but there is no downstream object ready to receive it.

 

### Waiting for Operator

 

The multiprocessor is waiting for operator(s) to arrive in order to start a process.

 

### Waiting for Transport

 

The multiprocessor has finished all processes for a flow item, released it, and there is a downstream object ready to receive it, but the flow item has not been picked up by a TaskExecuter yet.

 

 

 

## Statistics

 

The multiprocessor uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The MultiProcessor object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- MultiProcessor
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The MultiProcessor object shares several properties with all fixed resources:

 

- Fixed Resource Properties
