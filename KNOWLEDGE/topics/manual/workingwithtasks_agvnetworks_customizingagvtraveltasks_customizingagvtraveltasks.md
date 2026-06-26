---
id: workingwithtasks_agvnetworks_customizingagvtraveltasks_customizingagvtraveltasks
name: "Customizing AGV Travel Tasks"
kind: manual
breadcrumb: ["WorkingWithTasks","AGVNetworks","CustomizingAGVTravelTasks","CustomizingAGVTravelTasks"]
tags: ["manual","workingwithtasks","agvnetworks","customizingagvtraveltasks"]
source: "manual/WorkingWithTasks/AGVNetworks/CustomizingAGVTravelTasks/CustomizingAGVTravelTasks.html"
---

# Customizing AGV Travel Tasks

## Overview and Key Concepts

 

Flexsim's AGV module gives you great flexibility in how to direct AGV travel operations. While AGVs primarily travel to Control Point destinations on the AGV path network, you can customize this functionality as needed. First, you can define an offset distance, along the path of the destination control point, that the AGV should travel to. Second, you can define an offset distance, along the AGV's "train body", that you want the AGV to travel to. This can be defined directly with an agv body offset distance, and/or by defining one of the AGV's trailers as the object "relative to which" the AGV should travel.

 

All of this customization is done using some otherwise-unused parameters of the TASKTYPE_TRAVEL task, as explained below. If you are defining your task sequence logic in Process Flow, you can use the AGV Travel task sequence activity to define these custom parameters of a travel task.

 

By using these two offset mechanisms, either separately or in combination, you can more easily simulate complex travel operations that require traveling to any point on the AGV path network, not just to control points. This is especially useful in simulating operations like train coupling and uncoupling in train yards.

 

 

 

## Defining Destination Path Offset

 

When giving an AGV a travel task, you can define a target offset along the destination path. This offset is relative to the destination control point, and progresses along the path's *primary direction*.

 

You define this offset through the travel task's Var 2 parameter. Pass in the desired offset distance as a number, and the AGV will travel so that it arrives with its center location offset by that distance from the destination control point.

 

If you are defining this through a process flow, you can use a AGV Travel activity.

 

 

 

## Defining Destination AGV Body Offset

 

When giving an AGV a travel task, you can define a target offset along the AGV's "train body". This offset progresses forward along the AGV's train body, in that it represents a distance from the center of the agv, moving along any trailers that are attached to the AGV. A positive offset defines a target ahead of the AGV, whereas a negative offset defines a target behind the AGV.

 

You define this offset through the travel task's Var 3 parameter. Pass in the desired offset distance as a number, and the AGV will travel so that it arrives with this offset body location location at the target destination.

 

You can also specify an enumerated AGV.BodyOffset for this value, and the AGV will automatically resolve the associated offset distance.

 

If you are defining this through a process flow, you can use an AGV Travel activity.

 

 

 

## Defining the Destination Relative to a Trailer

 

You can also define a travel destination "relative to" one of the AGV's trailers. This can technically already be done using the AGV Body Offset option, but defining a "relative to" object can bypass a potentially more complex body offset calculation. Further, the AGV Body Offset works in combination with the "relative to" trailer, so that you can use them both to, for example, travel to a target relative to the back or front of a given trailer, as opposed to its center.

 

You define the "relative to" trailer through the travel task's Involved 2 parameter. Pass in the target trailer object, and the AGV will travel so that it arrives with that trailer's center at the target destination (assuming 0 added AGV body offset distance).

 

If you are defining this through a process flow, you can use a AGV Travel activity.

 

In defining the "relative to" object it is helpful to be familiar with how you access an AGV's trailers. See the AGV interface reference for more information.

 

Below are examples of defining the AGV body offset and "relative to" trailers.

 

 

 

 

 

### Deceleration and End Speed

 

Usually when you give an AGV a task to travel to a destination, you want the AGV to end that travel task fully decelerated to a stop. However, in some cases you may want to end a travel task while still moving, for example if you want to make dispatching decisions on-the-fly. In this case you can define a non-zero end speed for the travel task. Doing this will actually shift earlier the end time of the travel task, as well as the position of the AGV at the time it finishes. In other words, instead of ending on the destination control point while still moving, the travel task will end while the AGV is still approaching the control point, at the defined end speed. If you don't immediately give the AGV any subsequent travel tasks, then the AGV will decelerate down to stopped, arriving at the control point AFTER the travel task has finished. If, however, you immediately give the AGV a new travel task, it will continue to the new task, starting at the end speed of the previous task, traveling through the original destination control point.

 

The AGV system's default end speed behavior does not technically follow FlexSim's standard for TASKTYPE_TRAVEL tasks. FlexSim's standard travel task specification says an end speed of 0 means to end at the task executer's max speed. The reasoning is that travel tasks are often followed by offset tasks (load, unload). You don't want the task executer to decelerate to stop, then immediately start from 0 to do offset travel. So, the "default" end speed of 0 should mean that a travel task should end with the task executer still moving, since the task executer will continue to an offset task.

 

In the AGV system, however, an end speed of 0 means an end speed of 0. This is inconsistent with the standard. This inconsistency between the AGV system and FlexSim's standard was generally not a problem, because in the initial versions of the AGV system, AGVs could not perform offset travel. Even now, AGVs do not perform offset travel by default. However, since AGVs do now include the ability to perform offset travel if you specifically add offset travel logic, the inconsistency is more apparent. To resolve this, you can check the *Zero Travel End Speed Means Max Speed* box in the AGV Network properties General tab. This will bring the AGV system inline with the standard travel mechanism.
