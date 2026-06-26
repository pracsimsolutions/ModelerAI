---
id: reference_propertiespanels_taskexecuterpanels_robot_robot
name: "Robot"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TaskExecuterPanels","Robot","Robot"]
tags: ["manual","reference","propertiespanels","taskexecuterpanels"]
source: "manual/Reference/PropertiesPanels/TaskExecuterPanels/Robot/Robot.html"
---

# Robot

The Robot panel has options for controlling the robot's movements.

 

 

The following properties are on the Robot panel:

 

### Edit Geometry

 

Pressing this button opens the Robot Geometry window.

 

### Motion Mode

 

The robot responds to task sequences created by fixed resources much like the Crane object, more specifically the robot responds to FRLOAD, FRUNLOAD, TRAVELTOLOC and TRAVELRELATIVE task types. The robot has 4 modes of operation, each mode changes the way the robot responds to a task and more importantly how long it takes to perform the task. The properties that are below the Motion Mode menu will vary depending on which option you selected.

 

- **Move Time** - This is the default option. Here you define a move time explicitly with code.
- **Simple Motion Method** - This option allows you to provide a few basic speed inputs that determine the move time.
- **Use Joint Speeds** - Here you define a speed for each of the robot's joints.
- **Define Motion Paths** - The most flexible option, explained in the Robot Motion Paths topic.

 

 

 

## The Robot Geometry Window

 

The Robot Geometry window has the following properties:

 

 

### Relative Speeds

 

The options in this column control the relative speeds for each joint.

 

### Reset Position

 

The options in the column control the position to which the robot will return on reset.

 

### Manual Positioning Trackers

 

The sliders in this column control the current position of each joint.

 

### Open Gripper Width

 

This option controls how far open the gripper is when it's open.

 

### Set Reset Position to current

 

This option saves the current position of the robot as its reset position.
