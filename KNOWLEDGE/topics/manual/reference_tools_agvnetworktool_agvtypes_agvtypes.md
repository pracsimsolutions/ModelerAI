---
id: reference_tools_agvnetworktool_agvtypes_agvtypes
name: "AGV Types"
kind: manual
breadcrumb: ["Reference","Tools","AGVNetworkTool","AGVTypes","AGVTypes"]
tags: ["manual","reference","tools","agvnetworktool"]
source: "manual/Reference/Tools/AGVNetworkTool/AGVTypes/AGVTypes.html"
---

# AGV Types

## Overview and Key Concepts

 Within FlexSim's AGV system, AGV Types allow you to define one or more types of AGVs. This usually correlates with a specific make and model of AGV. Each AGV type defines the following behaviors for AGVs of that type.  

- Acceleration
- Deceleration
- Forward Max Speed
- Reverse Max Speed
- Battery Capacity
- Battery Depletion Rate
- Battery Recharge Rate
- Rotation Behavior
- Trailer Attachment Behavior

 

You use a table to enter these different values. In this table, max speed attributes are broken out by both path class and load type, whereas acceleration, deceleration, and battery depletion attributes are broken out by load type.

 

When you define multiple AGV Types, you need to specify which AGVs are associated with which AGV Type. You do this by clicking on the AGV in the 3D view, and then specifying the AGV Type in the *AGV Traveler* Properties panel.

 

 

 

## The AGV Types Tab

 

The AGV Types tab has the following properties:

 

 

You can get to the AGV Types page by right-clicking on a Path or Control Point and choosing AGV Network Properties.

 

### Path Classes

 

Here you can add, remove, reorder and rename the set of Path Classes for the model. Path Classes are specifically used for breaking out AGV max speeds by path. When you add a Path Class, a new speed row associated with that Path Class will be added to each AGV Type table for both forward and reverse speed.

 

### Load Types

 

Here you can add, remove, re-order and rename the set of Load Types for the model. Load Types are a user-defined list defining categories for what an AGV is carrying. This allows you to break out AGV speeds by the AGV's current load. The load type is resolved at the beginning of each travel operation, via the *Initialize Travel* field.

 

### Initialize Travel

 

This is a trigger that is fired at the beginning of each travel operation. The primary objective of this trigger is to set the AGV's current Load Type.

 

### AGV Types List

 

Here you can add, remove, re-order and rename the list of AGV Types for the model.

 

### AGV Type Spec Table

 

In the AGV Type Spec Table you define max speeds by Path Class, Load Type and AGV direction, as well as acceleration, deceleration, non-idle battery usage, and rotation speed.

 

### Battery Levels and Usage

 

Each AGV Type has a defined Battery Capacity (in Amp Hours), Idle Battery Usage (in Amps), and Recharge Rate (in Amps). Additionally, each AGV Type has a non-idle Battery Usage (in Amps) broken out by Load Type. Each AGV starts the simulation at its maximum battery capacity, and then will track its battery usage over the course of the simulation. Whenever it is idle, its Idle Battery Usage applies. Whenever it is doing a travel operation, its battery usage is based on its current Load Type. If you set the AGV to recharge, it will recharge at its recharge rate until it is full or it starts its next travel operation, whichever comes first. To query battery level, start a recharge, or manually set the battery level, etc., see the documentation for the agvinfo() command.

 

### Stop and Rotate Threshold

 

The stop-and-rotate threshold lets you define a threshold by which the AGV will stop and rotate to the direction of a path it is transferring to, using the rotation speed defined in the table above. If the difference in direction is greater than the threshold then the agv will stop and rotate. Otherwise it will simply travel onto the path without stopping. If your AGVs can turn 'on a dime', as many modern Autonomous Mobile Robots (AMRs) can do, then this value should be less than 180. Otherwise you should leave this value at 180 degrees, and define paths that do not require in-place turning.

 

### Switch to Reverse/Forward Threshold

 

Defines an angle threshold, in degrees, by which AGVs will reverse their travel direction. When an AGV travels from one path to another, it will check the difference in travel direction for the respective paths, and if it must change direction by more than this amount in transferring to the new path, then it will stop and reverse its direction before continuing onto the path.

 

If this threshold is above 180 degrees, then an AGV will always travel oriented forward, even when it would normally reverse direction from a previous stop. This should be used in conjunction with the rotate threshold to make the AGV turn with a rotation speed to get oriented in the direction it is going.

 

You can specify two different thresholds here: one for switching to the reverse direction, and another for switching to the forward direction. If they are different, then the AGV will 'prefer' traveling in one direction over another. If you want your AGV to prefer traveling forward, then make the Switch to Forward threshold less than the Switch to Reverse threshold. Then the AGV will more readily switch back to traveling forward after traveling in reverse.

 

### Attaching Trailers

 

You can also make the AGV attach loaded items as trailers to the AGV. This will make the loaded items trail behind the AGV on its path, instead of being carried on the AGV. Check the box **Attach Loads as Trailer**, and then define the **Trailer Gap**, which is the distance of the gap between the back of the AGV and the front of the trailing item.
