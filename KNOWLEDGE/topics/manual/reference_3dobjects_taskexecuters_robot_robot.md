---
id: reference_3dobjects_taskexecuters_robot_robot
name: "Robot"
kind: manual
breadcrumb: ["Reference","3DObjects","TaskExecuters","Robot","Robot"]
tags: ["manual","reference","3dobjects","taskexecuters"]
source: "manual/Reference/3DObjects/TaskExecuters/Robot/Robot.html"
---

# Robot

## Overview and Key Concepts

 

The robot is a special transport that lifts flow items from their starting locations and places them at their ending locations. Generally, the robot's base does not move. Instead, 6 joints rotate to move the flow items.

 

 

The robot is a task executer. It implements offset travel by rotating its joints. Note that by default, the x/y/z location of the robot does not change at all when it does offset travel. If the destination location is further away than the robot's maximum arm extension, then the robot will only extend its arm up to its maximum extension. It does not use the standard task executer maximum speed, acceleration and deceleration values.

 

The robot by default does not connect itself to a navigator, which means that it does not execute travel tasks unless you explicitly connect it to a network.

 

 

 

## Events

 

The robot uses the standard events that are common to all task executers. See Task Executer Concepts - Events for an explanation of these events.

 

 

 

## States

 

This object uses the task executer states. See Task Executer Concepts - States for more information.

 

 

 

## Statistics

 

The robot uses the standard statistics that are common to all task executers. See Task Executer Concepts - Statistics for an explanation of these statistics.

 

 

 

## Properties Panels

 

The Robot object uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Robot
- TaskExecuter
- Travel
- Dispatcher
- Ports
- Triggers

 

 

 

## Properties

 

The Robot object shares several properties with all task executers:

 

- Task Executer Properties

 

Additionally, the Robot object uses the following properties:

 

| Property | Type |
| --- | --- |
| JointSpeeds |  |
| MotionMode | Options |
| MoveTime | Unit Code |
| YRotationSpeed | Unit |
| ZRotationSpeed | Unit |
