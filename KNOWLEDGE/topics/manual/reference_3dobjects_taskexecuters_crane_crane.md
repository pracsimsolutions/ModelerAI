---
id: reference_3dobjects_taskexecuters_crane_crane
name: "Crane"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Crane","Crane"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Crane/Crane.html"
---

# Crane

## Overview and Key Concepts

 

The crane has similar functionality to the transporter but with a modified graphic. The crane works in a fixed space following rectangular x,y,z movements. It is designed to simulate rail-guided cranes such as gantry, overhead, or jib cranes. By default, the crane picker rises to the height of the crane object after picking up or dropping off a flow item before it will travel to the next location. To exercise more control over the movements of the picker from one pickup to the next, change the crane's travel sequence in its Properties window.

 

 

The crane is a task executer. It implements offset travel according to a travel sequence that the user specifies. By default, this travel sequence is L > XY > D. The > character separates travel operations, L means lift the hoist, X means move the gantry, Y means move the trolley, and D means drop the hoist to the offset position. The default travel sequence tells the crane to first lift the hoist, then move the gantry and trolley simultaneously to the offset position, then drop the hoist. The crane travels so that its x/y center and z base arrive at the destination location. If there is an involved flow item for the offset travel task, then the crane travels so that its x/y center and z base arrive at the top of the flow item, or in other words, it increases the arrival z location by the z size of the flow item.

 

 

 

## Events

 

The crane uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The crane uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Crane object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Crane
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Crane object shares several properties with all task executers:

 

- Task Executer Properties

 

Additionally, the Crane object uses the following properties:

 

| Property | Type |
| --- | --- |
| CraneSpeeds |  |
| FrameLocation | Array(3) |
| FrameSize | Array(3) |
| LiftHeight | Unit |
| LiftRadius | Unit |
| TravelSequence |  |
