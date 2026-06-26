---
id: reference_3dobjects_fixedresources_source_source
name: "Source"
kind: manual
breadcrumb: ["Reference","3DObjects","FixedResources","Source","Source"]
tags: ["manual","reference","3dobjects","fixedresources"]
source: "manual/Reference/3DObjects/FixedResources/Source/Source.html"
---

# Source

## Overview and Key Concepts

 

The source is used to create the flow items that travel through a model. Each source creates one class of flow item and can then assign properties such as labels or color to the flow item it creates. Sources can create flow item per an inter-arrival rate, per a scheduled arrival list, or simply from a defined arrival sequence.

 

 

The source is a fixed resource, although it does not receive flow items. Instead it creates the flow items and releases them. For that reason, it has no Input properties.

 

The source can operate in one of three possible modes:

 

- **Inter-Arrivaltime Mode:** In inter-arrivaltime mode, the source uses its inter-arrivaltime function. This function's return value is the time to wait till the next arrival of a flow item. The source waits this amount of time, then creates a flow item and releases it. Once the flow item has exited, it calls the inter-arrivaltime function again and repeats the cycle. Note that the inter-arrivaltime is defined as the time between the exit of one item and arrival of the next item, not as the time between the arrival of one item and the arrival of the next item. If you would like to make the inter-arrivaltime be defined as the true time between arrivals, then use a downstream queue with a large capacity to make sure that the source immediately sends on flow items when they are created. You can also specify whether the inter-arrivaltime should be executed for the first arrival, or if the first flow item should be created at time 0.
- **Arrival Schedule Mode:** In arrival schedule mode, the source follows a schedule defined by the user in the schedule table. Each row of the table specifies an arrival of flow item(s) at a given time in the simulation. For each arrival entry, you can specify the arrival time, name, number of items to create, and additional item labels for that arrival. The arrival times should be ordered correctly in the schedule table, meaning each entry's arrival time should be greater than or equal to the previous entry's arrival time. If the source is set to repeat the schedule, then it will immediately loop back on the first arrival after the last arrival, causing the first entry's arrival to happen at the exact same time as the last entry's arrival. Note here that, when repeating the schedule, the first entry's arrival time only applies to the very first loop through the schedule. This allows you to have an initial arrival time that is executed once, but not repeated. If you would like the source to wait a given amount of time between the last arrival and the repeated first arrival, then add an extra entry to the end of the table, give it an arrival time greater than the previous entry's arrival time, but have 0 flow item arrive for that new entry.
- **Arrival Sequence Mode:** Arrival sequence mode is like arrival schedule mode, except that there is no time associated with the arrivals. The source will create the flow item for a given table row, and then as soon as the last flow item for that entry has exited, it will immediately go to the next row in the table. You can also repeat the arrival sequence.
- Statistics
- Visuals
- Labels
- Source
- Output
- Ports
- Triggers

 

 

 

## Properties

 

The Source object shares several properties with all fixed resources:

 

- Fixed Resource Properties

 

Additionally, the Source object uses the following properties:

 

| Property | Type |
| --- | --- |
| ArrivalSchedule |  |
| ArrivalSequence |  |
| ArrivalStyle | Options |
| FlowItemClass | Options |
| InterArrivalTime | Unit Code |
| RepeatTable | Boolean |
| TimeZeroArrival | Boolean |
