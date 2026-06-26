---
id: reference_processflowobjects_tasksequences_unload_unload
name: "Unload"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","Unload","Unload"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/Unload/Unload.html"
---

# Unload

## Overview and Key Concepts

 

Use the Unload activity to make a task executer (such as an Operator or Transporter) unload an object in the 3D simulation model. For example, you can make a task executer drop off a flowitem at a processor or queue.

 

The Unload activity creates an unload task sequence and dispatches it to a task executer. Optionally, the task may be appended onto an already existing task sequence. See Create Task Sequence activity for more information.

 

 

 

 

## Connectors

 

The Unload activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Unload activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Executer / Task Sequence

 

Use the **Executer / Task Sequence ** box to determine which task executer or task sequence should receive the task. See Executer / Task Sequence for more information about this property.

 

### Item

 

Use the **Item** box to determine which item or object should be unloaded. Typically this is a flow item, however, other objects can be used. There are many different ways to reference flow items and objects in the model. Below are just a couple of examples: 

- You can use a Create Object activity to create a flow item in the 3D model and assign the appropriate label to that object.
- You can create a Wait For Event or Event Triggered Source activity that listens for flow items arriving at a fixed resource and assigns a label to those entering flow items.

 

### Station

 Use the **Station** box to select the object from which the task executer should unload the item to. Usually you will point to a fixed resource (such as a processor or queue) but you can also point it to another task executer.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
