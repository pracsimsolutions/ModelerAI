---
id: reference_processflowobjects_tasksequences_travel_travel
name: "Travel"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","Travel","Travel"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/Travel/Travel.html"
---

# Travel

## Overview and Key Concepts

 

Use the Travel activity to make a task executer (such as an Operator or Transporter) travel to a specific object (such as a fixed resource) in the 3D simulation model. If connected to a Travel Network, the task executer will use that network (such as AStar or Network Nodes) to travel to the specified object.

 

The AGV Travel activity adds a task to a previously created task sequence. Alternately, when the Executer / Task Sequence field references a task executer directly, this activity will create a task sequence with a single travel task, and dispatch that task sequence to the task executer.

 

  

#### Travel vs. Travel to Loc vs. AGV Travel

 

Use the Travel task sequence activity if you want a task executer to go to a specific object in the 3D simulation model. Use the Travel to Loc task sequence activity if you want a task executer to go to specific X, Y, and Z coordinates in the 3D simulation model (such as a place where there is no object). Use the AGV Travel task sequence activity if the task executer is traveling on an AGV path network, and you specifically want to define a custom path offset or AGV body offset value.

  

 

 

## Connectors

 

The Travel activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Executer / Task Sequence

 

Use the **Executer / Task Sequence ** box to determine which task executer or task sequence should receive the task. See Executer / Task Sequence for more information about this property.

 

### Destination

 

Use the **Destination** box to set the location to which the task executer will travel. The destination should be an object in the 3D model.

 

### End Speed

 

This field defines the target end speed for the travel operation.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
