---
id: reference_3dobjects_warehousing_storageobject_storageobject
name: "Storage Object"
kind: manual
breadcrumb: ["Reference","3DObjects","Warehousing","StorageObject","StorageObject"]
tags: ["manual","reference","3dobjects","warehousing"]
source: "manual/Reference/3DObjects/Warehousing/StorageObject/StorageObject.html"
---

# Storage Object

## Overview and Key Concepts

 

The Storage Object is a parent class of several objects that are used to store flow items in a two dimensional grid of storage locations, called slots. These slots are organized, first by bay, then by level, and finally by position/slot number.

 

The Storage Object is an *abstract class*, meaning you do not create instances of this specific class directly. Rather, the Storage Object is a parent class of the Rack, Floor Storage, Gravity Flow Rack, Push Back Rack, and Drive In Rack classes. Thus, when you create an instance of one of these classes, you are inherently creating an instance of a Storage Object.

 

The Storage Object is a Fixed Resource. However, fixed resource functionality is turned off by default. You can enable it through the Flow Panel, or merely by connecting other fixed resources to it using an 'A' connection. When fixed resource logic is enabled, the storage object receives flow items through its input ports until its maximum content value is met. When each flow item enters the rack, it executes the minimum dwell time function for that item. This function returns the minimum stay time for that flow item. The rack starts a timer for that amount of time. After the timer expires, the rack releases the flow item.

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The Storage Object is a Fixed Resource, and uses the standard events that are common to all fixed resources. See Fixed Resources - Events for an explanation of these events.

 

The Storage Object has the following additional events:

 <!-- List events in alphabetical order --> 

### Minimum Dwell Time

 

This event occurs when the Minimum Dwell Time field is evaluated. It is only evaluated if a storage object's Use as Fixed Resource checkbox is selected.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Item | Object | The entering item, which will be assigned a dwell time |
| Port | int | The input port number for the entering item |

 

This event returns the minimum dwell time for the specified item.

 

### On End Dwell Time

 

This event occurs when an item has reached its minimum dwell time. After this event, the storage object will release the item. It is only evaluated if the storage object's Use as Fixed Resource checkbox is selected.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Item | Object | The item that has just completed its dwell time |

 

### On Slot Assign

 

This event occurs when an item is assigned to a specific slot in this object, i.e. when the Storage.Item.assignedSlot property of the item is set.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Slot | Storage.Slot | The assigned slot |
| Item | Object | The assigned item |

 

### On Slot Entry

 

This event occurs when an item enters a specific slot. It occurs at essentially the same time as the On Entry event, however, it is executed after a slot has been assigned. The slot is therefore a parameter of the event.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Slot | Storage.Slot | The associated slot |
| Item | Object | The entering item |

 

### On Slot Exit

 

This event occurs when an item exits a specific slot. It occurs at essentially the same time as the On Exit event, however, it is executed specifically associated with the slot that is being exited. The slot is therefore a parameter of the event.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Slot | Storage.Slot | The associated slot |
| Item | Object | The exiting item |

 

### Slot Assignment Strategy

 

This event occurs when the respective Slot Assignment Strategy field is evaluated.

 

It has the following parameters:

 

| Parameter | Type | Description |
| --- | --- | --- |
| Item | treenode | The entering item, which will be assigned a bay or level |

 

The execution of this field is responsible fore finding a proper slot to place the item into, and assigning the item to that slot by setting the Storage.Item.assignedSlot property.

 

 

 

## States

 

The Storage Object does not implement any states. Instead, use the content graph to get statistics.

 

 

 

## Statistics

 

The Storage Object uses the standard statistics that are common to all fixed resources. See Fixed Resources - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Storage Object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Storage Object
- Storage Options
- Address Mapping
- Flow
- Output (when Used as Fixed Resource)
- Input (when Used as Fixed Resource)
- Ports
- Triggers

 

 

 

## Properties

 

The Storage Object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Storage Object uses the following properties:

 

| Property | Type |
| --- | --- |
| AddressScheme | Options |
| AisleID | String |
| BayProgression | Options |
| BayStride | Number |
| LevelProgression | Options |
| LevelStride | Number |
| MarkOutbound | Boolean |
| MinimumDwellTime | Unit Code |
| SlotAssignmentStrategy | Code |
| SlotProgression | Options |
| SlotStackingAxis1 | Options |
| SlotStackingAxis2 | Options |
| SlotStackingAxis3 | Options |
| SlotStride | Number |
| StartBay | Number or String |
| StartLevel | Number or String |
| StartSlot | Number or String |
| UseAsFixedResource | Boolean |
| Visualization | Options |
| ZoneID | String |
| VirtualizeItems | Boolean |

 

#### Dimensions Properties

 

The storage object also supports several properties associated with its dimensions. These properties define things like the number of bays, levels, and slots, their sizes, and padding. Here a property value can be represented as a single value if all slots/levels/bays in the object are homogeneous with respect to that property. But if there are differences across bays, levels, or slots, then the property's value becomes an array.

 

For example, consider the *NumLevels* property. If the storage object contains 10 bays, and all bays have 5 levels, then the object's *NumLevels* property will be the single value `5`. However, if the first 5 bays have 3 levels, and the last 5 bays have 6 levels, then the *NumLevels* property becomes an array, with one element for each bay. In this example the *NumLevels* value would be `[3, 3, 3, 3, 3, 6, 6, 6, 6, 6]`.

 

The *NumLevels* property can become at most a one-dimensional array. However, other properties (*LevelSize*, *NumSlots*, *SlotSize*, etc.) may be two or three dimensional arrays, depending on the heterogeneity/homogeneity of the bays, levels, or slots.

 

The storage object also supports a *Dimensions* property. This encompasses all of the other dimension-focused properties, aggregated into a Map.

 

The dimension-focused properties are as follows

 

| Property |
| --- |
| BaySize |
| Dimensions |
| LevelSize |
| NumBays |
| NumLevels |
| SlotPaddingBack |
| SlotPaddingBottom |
| SlotPaddingFront |
| SlotPaddingLeft |
| SlotPaddingRight |
| SlotPaddingTop |
| SlotSize |
| SlotsPerBay |
| StorableSlots |
