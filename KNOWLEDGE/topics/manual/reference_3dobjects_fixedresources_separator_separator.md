---
id: reference_3dobjects_fixedresources_separator_separator
name: "Separator"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Separator","Separator"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Separator/Separator.html"
---

# Separator

## Overview and Key Concepts

 

The separator is used to separate a flow item into multiple parts. This can either be done by unpacking a flow item that has been packed by a combiner or by making multiple copies of the original flow item. The splitting/unpacking is done after the process time has completed. The separator can be set to require operators during its setup, processing and repair times.

 

 

The combiner is a sub-class of the processor, which is in turn a fixed resource. It receives one flow item, then executes the setup and process times for that flow item. If the separator is in unpack mode, then once the setup and process times are finished, the separator moves the unpack quantity out of the flow item and into itself. Then it releases all flow items that it unpacked. Once all unpacked flow items have left the separator, it releases the container flow item. If the separator is in split mode, once the setup and process times are finished, the separator duplicates the flow item so that the resulting total number of flow items is the split quantity. Then it releases all of the flow items. For both unpack and split modes, once all flow items have left the separator, the separator immediately receives the next flow item.

 

### The Split/Unpack Quantity

 

This quantity value has subtle differences for unpack mode versus split mode. When in unpack mode, the separator unpacks the exact number of flow items specified by this quantity. This means the total number of flow items that result is one more than the unpack quantity (the unpack quantity + the container flow item). When in split mode, however, the separator duplicates the flow item split quantity - 1 number of times. This means the total number of flow items that result is the exact same as the split quantity.

 

### Unpacking Order

 

When the Separator is in unpack mode, it unpacks the container flow item from back-to-front, meaning it pulls the last item in the container out first, then the second to last, and so on. If you want flow items to be unpacked out in a certain order, then set their rank in the entry trigger.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The separator uses all of the same events as the processor. See Processor - Events for an explanation of these events.

 

The separator has the following additional events:

 

### Split Unpack Quantity

 

This event fires after the on process finish event fires. It determines how to unpack or split the flow item.

 

It has the following parameters:

 

| Event Parameter | Type | Explanation |
| --- | --- | --- |
| Item | Object | The flow item to be split or unpacked |

 

The **Split/Unpack** field should return a number which is either how many items to unpack from the container item, or how many items to split the item into.

 

 

 

## States

 

The separator uses all of the same states as the processor. See Processor - States for an explanation of these states.

 

 

 

## Statistics

 

The separator uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Separator object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Separator
- Processor
- Output
- Input
- Ports
- Triggers

 

 

 

## Properties

 

The Separator object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Separator object uses the following properties:

 

| Property | Type |
| --- | --- |
| SeparateMode | Options |
| SeparateQty | Number |
