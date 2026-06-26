---
id: reference_processflowobjects_tasksequences_traveltoloc_traveltoloc
name: "Travel to Loc"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","TravelToLoc","TravelToLoc"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/TravelToLoc/TravelToLoc.html"
---

# Travel to Loc

## Overview and Key Concepts

 

Use the Travel to Location activity to make a task executer (such as an Operator or Transporter) travel to specific X, Y, and Z coordinates in the 3D simulation model. This will cause the task executer to perform offset travel (ignoring its navigator) to the location.

 

The Travel to Location activity creates a travel task sequence and dispatches it to a task executer. Optionally, the task may be appended onto an already existing task sequence. See Create Task Sequence activity for more information.

 

  

#### Travel vs. Travel to Loc

 

Use the Travel task sequence activity if you want a task executer to go to a specific object in the 3D simulation model. Use the Travel to Loc task sequence activity if you want a task executer to go to specific X, Y, and Z coordinates in the 3D simulation model (such as a place where there is no object).

  

 

 

## Connectors

 

The Travel To Loc activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Travel To Loc activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Executer / Task Sequence

 

Use the **Executer / Task Sequence ** box to determine which task executer or task sequence should receive the task. See Executer / Task Sequence for more information about this property.

 

### Relative

 

When the **Relative** checkbox is checked, the X, Y, and Z coordinates will be relative to the current location of the task executer. If the **Relative** checkbox is cleared, the task executer will travel to the X, Y, and Z coordinates of the object containing the task executer, whether that be the model or a visual tool.

 

### X, Y, and Z Location

 

The location coordinates to which the task executer will move.

  

#### Finding the X, Y, and Z Coordinates in the Model

 

Position your mouse over the location you want the task executer to travel to. The bottom left corner of the FlexSim window will display the current mouse position, such as in the following image:

 

 

In this example, the X coordinate is 7.36, Y is 4.46, and Z is 0.

  

### End Speed

 

This field defines the target end speed for the travel operation.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
