---
id: reference_3dobjects_fixedresources_processor_processor
name: "Processor"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Processor","Processor"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Processor/Processor.html"
---

# Processor

## Overview and Key Concepts

 

The processor is used to simulate the processing of flow items in a model. The process is simply modeled as a forced time delay. The total time is split between a setup time and a process time. The processor can process more than one flow item at a time. Processors may call for operators during their setup and/or processing times. When a processor breaks down, all of the flow items that it is processing will be delayed.

 

 

The processor is a fixed resource. It is also a super-class of the combiner and separator 3D objects. It continues to receive flow items until its maximum content is met. Each flow item that enters the processor goes through a setup time followed by a process time. After these two processes have finished, the flow item is released. If the maximum content value is greater than one, then flow items will be processed in parallel.

  

#### Maximum Content Value

 

Be very careful about setting a maximum content value greater than 1. The processor will not correctly request operators if you are processing more than one flow item at a time. MTBF/MTTR times will also not be calculated correctly if the maximum content is greater than one. Also, state statistics won't be recorded correctly. The maximum content value is only there to allow you to create multiple very simple processors in parallel without having to drag that many into your model. If you want to edit any properties other than the simple setup, process times and flow page properties, then you should drag several Processors out, and have each just receive one flow item at a time.

  

### Setup/Process Operators

 

If the processor is set to use operators during its setup or process time, then at the beginning of the respective operation it will call the user-defined number of operators using the requestoperators command with the processor as the station, and the item as the involved object. This will cause the processor to be stopped until the operators have arrived.

  

#### Be Aware of Relevant Commands

 

Please be aware of how a requestoperators task sequence is constructed, as described in the requestoperators documentation in the Command Helper. Also know how the stopobject command works as explained in the Command Helper.

  

Once all operators have arrived, the processor will resume its operation. Once the operation is finished, the processor will release the operators it called. If the processor is set to use the same operators for both setup and process time, then the processor won't release the operators until both the setup process times are finished.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The processor uses the standard events that are common to all fixed resources. See Fixed Resources - Events for an explanation of these events.

 

The processor has the following additional events:

 

### On Process Finish

 

This event is fired when the process time has expired. When this event fires it will execute the on process finish trigger where you can execute custom logic using FlexScript or preconfigured pick options.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Process Item | Object | The flow item being processed |

 

### On Setup Finish

 

This event is fired when the setup time has expired, right before the process time event fires. When this event fires it will execute the on setup finish trigger where you can execute custom logic using FlexScript or preconfigured pick options.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Setup Item | Object | The flow item that is about to be processed |

 

### Operator Reference

 

The operator reference event will only fire if either the **Use Operator(s) for Setup** or **Use Operator(s) for Process** is checked. This event will fire after the item has entered the processor, before the setup or process time has begun. This event will evaluate the **Pick Operator** field.

 

If the processor has a setup and process time and the **Same as Setup** checkbox on the processor's properties is unchecked, the operator reference event will fire twice. The first event will fire right after the setup time event. The second event will fire after the process time event.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item being processed |
| Trigger | int | If the operator is being called for setup, the trigger value is 1. If the operator is being called for processing, the trigger value is 2. |

 

The **Pick Operator** field should return a reference to a task executer or dispatcher object that will be used to process the item. The processor will dispatch a task sequence to the associated object which will call the task executer to the processor then utilize them until the setup and/or process time is complete.

 

### Process Time

 

This event fires after the setup is finished. It will evaluate the **Process Time** field.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item being processed |

 

The **Process Time** field should return a number which is the processing time for the item.

 

### Setup Time

 

This event fires after the item has entered the object. It will evaluate the **Setup Time** field.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item being processed |

 

The **Setup Time** field should return a number which is the setup time for the item.

 

 

 

## States

 

For statistical purposes, the processor will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Idle

 

The object is empty.

 

### Setup

 

The object is in its modeller-defined setup time.

 

### Processing

 

The object is in its modeller-defined process time.

 

### Blocked

 

The object has released its flow item(s), but downstream objects are not ready to receive them yet.

 

### Waiting for Operator

 

The object is waiting for an operator to arrive, either to repair a breakdown, or to operate on a batch.

 

### Waiting for Transport

 

The object has released a flow item and a downstream object is ready to receive it, but a transport object has not picked it up yet.

 

 

 

## Statistics

 

The processor uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Processor object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Processor
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The Processor object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Processor object uses the following properties:

 

| Property | Type |
| --- | --- |
| AnimateItems | Boolean |
| NumProcessOperators | Number |
| NumSetupOperators | Number |
| OperatorPreempt | Options |
| OperatorPriority | Number |
| OperatorRef | Code |
| ProcessTime | Unit Code |
| SetupTime | Unit Code |
| UseProcessOperators | Boolean |
| UseSameNumOperators | Boolean |
| UseSetupOperators | Boolean |
