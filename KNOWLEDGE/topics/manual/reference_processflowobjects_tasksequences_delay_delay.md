---
id: reference_processflowobjects_tasksequences_delay_delay
name: "Delay (Task Sequence)"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","Delay","Delay"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/Delay/Delay.html"
---

# Delay (Task Sequence)

## Overview and Key Concepts

 

Use the Delay (task sequence) activity to make a task executer (such as an Operator or Transporter) delay for a specific period of time in the 3D simulation model. For example, a delay could represent a task that takes a specific amount of time to complete such as cleaning a machine, assembling a product, etc.

 

The Delay activity creates a delay task sequence and dispatches it to a task executer. Optionally, the task may be appended onto an already existing task sequence. See Create Task Sequence activity for more information.

 

  

#### Make Sure You're Using the Correct Delay Activity

 

Don't confuse the Delay (task sequence) activity with the general Delay activity .

  

 

 

## Connectors

 

The Delay activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Delay activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Executer / Task Sequence

 

Use the **Executer / Task Sequence ** box to determine which task executer or task sequence should receive the task. See Executer / Task Sequence for more information about this property.

 

### Delay Time

 

The **Delay Time** defines the amount of time that the task executer will be delayed.

 

### State

 

Use the **State** box to determine which state the task executer will be in during the delay task (such as idle, busy, on break, etc.). The task executer's state is useful if you want to record statistics about how long the task executer is in a particular state. Click the arrow next to the **State** box to see a menu of available states. You can also specify user defined states.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
