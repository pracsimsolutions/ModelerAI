---
id: reference_processflowobjects_tasksequences_customtask_customtask
name: "Custom Task"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","CustomTask","CustomTask"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/CustomTask/CustomTask.html"
---

# Custom Task

## Overview and Key Concepts

 

If you need to give a task executer a task that isn't currently available in the process flow library, you can use the Custom Task activity to build your own custom task. The Custom Task activity has a **Task Type** menu with a complete list of other tasks that are available in addition to the common ones that are available in the library.

 

The Custom Task activity creates a task sequence with a single task and dispatches it to a task executer. If the task executer already has a task sequence, the task will be added to the end of a task sequence. Optionally, the task may be appended onto an already existing task sequence. See Create Task Sequence activity for more information.

 

 

 

 

## Connectors

 

The Custom Task activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for Custom Task Activities:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Executer / Task Sequence

 

Use the **Executer / Task Sequence ** box to determine which task executer or task sequence should receive the task. See Statistics for more information about this property.

 

### Task Type

 

You can select one of the available task types from the **Task Type** menu. This menu lists all possible task types. See the Task Sequence Types reference if you would like more information about each of the task types listed in this menu.

 

### Custom Properties

 

The custom properties are not available until after you select a task from the **Task Type** menu. The names of the properties will change based on the task you selected. Each box will represent one of the parameters that are required or optional for that task. See the Task Sequence Types reference for a description of the parameters for each task type.

 

### Wait Until Complete

 

If the **Wait Until Complete** box is checked, the token will be held in the activity until this task has been completed.
