---
id: reference_3dobjects_fixedresources_queue_queue
name: "Queue"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Queue","Queue"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Queue/Queue.html"
---

# Queue

## Overview and Key Concepts

 

The queue is used to store flow items when a downstream object cannot accept them yet. By default, the queue works in a first-in-first-out manner, meaning that when the downstream object becomes available, the flow item that has been waiting for that object the longest will leave the queue first. The queue has options for accumulating flow items into a batch before releasing them.

 

 

The queue is a fixed resource. It will continue to receive flow items until it reaches its maximum content. If batching is disabled, the queue will release the flow item as soon as it arrives in the queue, and the OnEndCollecting trigger will be called right before each flow item is released.

 

### Batching

 

If batching is enabled, then the queue will wait until it receives its target batch of flow items, then it will release all the flow items in the batch at the same time. By default the maximum wait time is 0. A max wait time of 0 means that there is no max wait time, or the queue will wait indefinitely to collect its batch. If the max wait time is non-zero, then when the first flow item arrives, the queue will start a timer. If at the end of the timer the batch is still not met, the queue will finish collecting the batch and release all flow items it collected.

 

The OnEndCollecting trigger is called just before the flow items are released, a reference to the first flow item in the batch is passed as item, and the number of items collected is passed as param(2). If the queue is configured to flush contents between batches, then it will close its input ports as soon as it ends collecting a batch, and will wait until the full batch has exited before opening the input ports again. If the queue doesn't flush contents between batches, then if will immediately start collecting another batch as soon as it finishes collecting each batch. This means that you can have several finished batches still in the queue at any given time, waiting to leave.

  

#### Target Batch Size

 

Setting a target batch size does NOT nullify the queue's maximum content value. This means if you set your target batch size to a value higher than the maximum content, the queue will never meet its batch because its maximum content is too small.

  

 

 

## Events

 

For information on events, see the Event Listening page.

 

The queue uses the standard events that are common to all fixed resources. See Fixed Resources - Events for an explanation of these events.

 

The queue has the following additional events:

 <!-- List events in alphabetical order --> 

### On End Collecting

 

This event occurs when the queue completes a batch of items, before the items leave the queue.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Entering Item | Object | A reference to the last flow item in the batch. |
| Batch Size | int | The number of flow items in the batch. |

 

 

 

## States

 

For statistical purposes, the queue will be in one of the following states at various points during a simulation run. The current state can be viewed by clicking on the object and then viewing the Statistics panel in Properties.

 

### Empty

 

The queue is empty.

 

### Collecting

 

The queue is collecting flow items for a batch.

 

### Releasing

 

The queue has finished collecting its batch(es) and is releasing the flow items that it has. Also if the queue is not batching, and has flow items in its queue, it will be in this state.

 

### Waiting for Transport

 

The queue has flow items that have been released and are ready to move downstream, but are waiting for a transport to come and pick them up.

 

 

 

## Statistics

 

The queue uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Queue object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Queue
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The Queue object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Queue object uses the following properties:

 

| Property | Type |
| --- | --- |
| BatchSize |  |
| DoBatching | Boolean |
| FlushContent | Boolean |
| ItemPlacement | Options |
| LIFO | Boolean |
| MaxBatchTime | Unit |
| StackBaseZ | Unit |
