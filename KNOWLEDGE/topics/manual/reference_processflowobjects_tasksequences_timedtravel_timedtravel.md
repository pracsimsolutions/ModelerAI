---
id: reference_processflowobjects_tasksequences_timedtravel_timedtravel
name: "Timed Travel"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","TimedTravel","TimedTravel"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/TimedTravel/TimedTravel.html"
---

# Timed Travel

## Overview and Key Concepts

 

The Timed Travel activity is similar to the standard Travel task sequence activity, except it lets you define a target travel time for the travel task.

 

The Timed Travel activity adds a task to a previously created task sequence. Alternately, when the Executer / Task Sequence field references a task executer directly, this activity will create a task sequence with a single travel task, and dispatch that task sequence to the task executer.

 

 

 

 

## Connectors

 

The Timed Travel activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

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

 

### Target Time

 

This field defines the target duration that the travel operation should take.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
