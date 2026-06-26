---
id: reference_3dobjects_taskexecuters_asrsvehicle_asrsvehicle
name: "ASRS Vehicle"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","ASRSvehicle","ASRSvehicle"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/ASRSvehicle/ASRSvehicle.html"
---

# ASRS Vehicle

## Overview and Key Concepts

 

The ASRS vehicle is a special type of transport specifically designed to work with racks. The ASRS vehicle will slide back and forth in an aisle between two racks picking up and dropping off flow items. The reach, lift, and travel motions are fully animated by the ASRS vehicle. The lift and travel motions will occur simultaneously, but the reach will only occur after the vehicle has come to a complete stop.

 

 

The ASRS vehicle is task executer. It implements offset travel by only travelling along its own x-axis. It travels until it is perpendicular with the destination location, lifting its platform as well. If the offset travel is for a load or unload task, then once the offsetting is finished, it will use the user-specified load/unload time to convey the flow item onto its platform, or off of its platform to the destination location.

 

The ASRS vehicle does not connect itself to a navigator by default. This means that travel tasks will not be performed. Instead, all traveling is done using offset travel.

  

#### Conveying a Flow Item Onto an ASRS Vehicle

 

For a load task, the conveying of the item onto the platform may not work if the flow item is in an object that continuously refreshes the location of the flow item, like a conveyor for example. In this case, if you want the conveying of the item onto the platform to show up, then make sure that the ASRS vehicle is ranked after the object it is picking up from in the model tree (the ASRS vehicle must be lower down in the tree).

  

In addition to the standard task executer fields, the ASRS vehicle has a modeler-defined lift speed and initial lift position for the its platform. The platform will return to this position whenever the ASRS vehicle is idle or is not doing offset travel.

 

### Context

 

Since the main distinction of an ASRS vehicle is that it only moves along its x and z axes and doesn't rotate, this object can be used for any purpose in which you don't want the object to turn, but rather just go forward and backward and up and down. In some models it has been used as a simple transfer car, or as a transfer conveyor between two or more conveyors.

 

 

 

## Events

 

The ASRS vehicle uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The ASRS vehicle uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The ASRS vehicle object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- ASRS
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The ASRS vehicle object shares several properties with all task executers:

 

- Task Executer Properties

 

Additionally, the ASRS vehicle object uses the following properties:

 

| Property | Type |
| --- | --- |
| InitialLiftHeight | Unit |
