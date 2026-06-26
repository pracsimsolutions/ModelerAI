---
id: reference_processflowobjects_tasksequences_dispatchtasksequence_dispatchtasksequence
name: "Dispatch Task Sequence"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","DispatchTaskSequence","DispatchTaskSequence"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/DispatchTaskSequence/DispatchTaskSequence.html"
---

# Dispatch Task Sequence

## Overview and Key Concepts

 

The Dispatch Task Sequence activity can dispatch a task sequence to a task executer (such as an Operator or Transporter). Generally you won't need to use the Dispatch Task Sequence activity with your task sequences because the **Create Task Sequence** activity and other task sequence activities will be dispatched immediately by default.

 

 

However, you could use a Dispatch Task Sequence if you would prefer to build a series of task sequences first and dispatch them later. There are two different methods you can use to build a task sequence that is dispatched later:

 

1. You could use a Task Sequence Global List. Using this method, fixed resources can push task sequences to the list or you can manually create the task sequences and push them to the list. When ready, you can pull a task sequence from that list and then dispatch it to the appropriate task executer using this activity.
1. You could use a **Create Task Sequence** activity to create a task sequence but set the **Task Executer / Dispatcher** property to *None*. (See Create Task Sequence for more information about this property.) You can then build the entire task sequence without dispatching it to a task executer. When ready, you can dispatch the task sequence to the appropriate task executer using this activity.

 

 

 

## Connectors

 

The Dispatch Task Sequence activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Dispatch Task Sequence activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Task Sequence(s)

 

Use the **Task Sequence(s)** box to select the task sequence or array of task sequences that will be dispatched. Task sequences that have already been dispatched will not redispatch.

 

### Task Executer / Dispatcher

 

Use the **Task Executer / Dispatcher** box to select the task executer or dispatcher that you want to assign the task sequence to.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
