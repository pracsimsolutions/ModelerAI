---
id: reference_processflowobjects_tasksequences_createtasksequence_createtasksequence
name: "Create Task Sequence"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","TaskSequences","CreateTaskSequence","CreateTaskSequence"]
tags: ["manual","reference","processflowobjects","tasksequences"]
source: "manual/Reference/ProcessFlowObjects/TaskSequences/CreateTaskSequence/CreateTaskSequence.html"
---

# Create Task Sequence

## Overview and Key Concepts

 

The Create Task Sequence activity will create an empty task sequence and optionally dispatch it to a task executer.

 

 

A task sequence is a series of actions (known as tasks, such as Travel, Load, or Unload) that can be assigned to a task executer (such as an Operator or Transporter). Each task executer has a list of task sequences that have been dispatched to them. When a task executer finishes a task sequence, they will immediately begin executing the next task sequence in their list.

 

By default, this activity is configured to create a task sequence that uses a wait state. This means that once the Task Executer begins executing the task sequence, they will not move on to other task sequences until they are explicitly given a finish task. For this reason, this activity is meant to be paired with the Finish Task Sequence activity, which is used to give the finish task.

  

#### Using a Wait State is Recommended

 

By using a wait state and a Finish Task Sequence activity, you can more easily use task sequences from both 3D objects and from Process Flow. It also creates a natural acquire/release mechanism that can sometimes replace using Acquire/Release Resource activities.

  

 

 

## Connectors

 

The Create Task Sequence activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Create Task Sequence activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Task Executer / Dispatcher

 

Use the **Task Executer / Dispatcher** box to select the task executer or dispatcher that you want to assign the task sequence to. If you want to create a task sequence but not dispatch it, click the button so that this box now reads *None*. You'll also need to uncheck the **Dispatch and Wait** box. This can be used to do things like pushing the task sequence onto a list before dispatching it. Once you know who to dispatch the task sequence to, you'll need to use a **Dispatch TS (Task Sequence)** activity to assign the task sequence to a task executer later in your process flow. See Dispatch Task Sequence for more information about this activity.

 

### Priority

 

The **Priority** box sets the priority level of the task sequence. By default, task executers will complete task sequences and activities in the order they were received (first in, first out). However, you might want a task executer to complete more important tasks first. You can use the **Priority** box to cause the task executer to work on higher priority tasks sequences before lower priority task sequences.

 

By default, all task sequences are assigned a priority of 0, but you can enter in any number in the **Priority** box. By default, Task executers will prioritize task sequences to perform higher priority task sequences first through their **Queue Strategy** property. For example, imagine you create a task sequence with a priority number of 1 and another with a priority number of 2. If both task sequences are waiting in a task executer's queue to be performed next, the task executer will work on the task sequence with the priority number 2 first.

  

#### Plan Out Your Priority Levels

 

If you decide to prioritize your task sequences, you might want to plan out what your priority levels will be in advance to save editing time down the road. Or you could reference values in a Global Table to easily define and edit priority levels later.

  

### Preemption

 

The **Preemption** menu sets the preemption value of the task sequence. *Preemption* is when a task executer stops its current task sequence to work on the preempting task sequence. You can use the **Preemption** menu to determine if and how preemption occurs.

 

For example, Operator A's most important responsibility is to repair machines. When there are no machines to repair, however, it should also transport material throughout the model. If a machine breaks down while Operator A is in the middle of transporting a flow item somewhere, then the Operator should stop whatever it is doing and repair the machine, instead of finishing the transport operation. To simulate this behavior in your model, you'll have to make sure that the Operator's machine repair task sequence is set to preempt the task sequence for transporting flow items.

 

The **Preemption** menu has the following options:

 

- **no preempt** - The current task sequence will not preempt any other task sequences.
- **preempt** - The current task sequence will preempt any active task sequences. The active task sequence (the one that is being preempted) will go back into the task executer's queue to be finished later.
- **preempt, aborting active** - The current task sequence will preempt any active task sequences. The active task sequence (the one that is being preempted) will be deleted and will not be finished later.
- **preempt, aborting all** - The current task sequence will preempt any active task sequences. The active task sequence and all other task sequences in the task executer's queue will be deleted and will not be finished later.

 

### Wait for Finish State

 

The *Wait for Finish State* defines a state to go into when the task executer who is performing the task sequence has nothing left to do in the task sequence, but must wait because the task sequence has not been finished with a Finish Task Sequence activity.

 

### Assign To

 

Assigns a reference on the specified label/node to the created task sequence. See the Assign To section of Common Properties for more information.

 

### Dispatch and Wait

 

When this box is checked, the token will wait on this activity until a task executer begins executing this task sequence.

 

### Assign Task Executer To

 

This field is only available if you check the Dispatch and Wait box. When the task executer begins executing the task sequence, this field will assign the task executer to a label. For example, if you dispatch the task sequence to a Dispatcher, then the task will eventually be given to a task executer. That task executer will be assigned to the label specified in this field.
