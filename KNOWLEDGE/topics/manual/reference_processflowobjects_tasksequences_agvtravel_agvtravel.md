---
id: reference_processflowobjects_tasksequences_agvtravel_agvtravel
name: "AGV Travel"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","AGVTravel","AGVTravel"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/AGVTravel/AGVTravel.html"
---

# AGV Travel

## Overview and Key Concepts

 

The AGV Travel activity is similar to the standard Travel task sequence activity, except it is specifically intended for task executers connected to an AGV network, and provides additional options specific to AGV functionality.

 

The AGV Travel activity adds a task to a previously created task sequence. Alternately, when the Executer / Task Sequence field references a task executer directly, this activity will create a task sequence with a single travel task, and dispatch that task sequence to the task executer.

 

 

 

 

## Connectors

 

The AGV Travel activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

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

 

### Relative To Trailer

 

This field optionally defines an AGV trailer that the target destination point should be relative to. See Customizing AGV Travel Tasks.

 

### End Speed

 

This field defines the target end speed for the travel operation.

 

### Path Offset

 

This field defines a target offset distance along the destination path. See Customizing AGV Travel Tasks.

 

### AGV Body Offset

 

This field defines a target offset distance along the AGV's body. See Customizing AGV Travel Tasks.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
