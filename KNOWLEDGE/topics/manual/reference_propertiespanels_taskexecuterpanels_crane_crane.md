---
id: reference_propertiespanels_taskexecuterpanels_crane_crane
name: "Crane"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","TaskExecuterPanels","Crane","Crane"]
tags: ["manual","reference","propertiespanels","taskexecuterpanels"]
source: "manual/Reference/PropertiesPanels/TaskExecuterPanels/Crane/Crane.html"
---

# Crane

The Crane panel has options for controlling the crane's lift and movement.

 

 

The following properties are on the Crane panel:

 

### Travel Sequence

 

Here you can specify the order in which the crane performs travel operations.

 

### Speeds Table

 

Here you specify the max speed, acceleration and deceleration for each of the 4 operations the crane will do. Note that these operations only apply to offset travel. If the crane is connected to a network, then when it is on the network, only the normal maxspeed, acceleration and deceleration specified in the Task Executer tab will be used.

 

### Lift Height

 

Here you define how high the crane will lift to get to its lift height.

 

### Lift Radius

 

Specify a radius within which the crane will not do a lift operation.

 

### Frame X/Y/Z Location

 

These numbers define the location of the crane's frame. Note that this is different than the cranes actual x/y/z location. The crane's x/y/z location describes the location of the moving part of the crane. The frame will be stationary throughout the simulation, while the actual x/y/z location of the crane changes as the crane travels.

 

### Frame X/Y/Z Size

 

These numbers define the size of the crane's frame. Note that this is different than the cranes actual x/y/z size. The crane's x/y/z size describes the size of the moving part of the crane.
