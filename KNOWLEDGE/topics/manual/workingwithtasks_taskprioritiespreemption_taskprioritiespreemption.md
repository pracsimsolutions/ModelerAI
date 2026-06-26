---
id: workingwithtasks_taskprioritiespreemption_taskprioritiespreemption
name: "Task Priorities and Preemption"
kind: manual
breadcrumb: ["WorkingWithTasks","TaskPrioritiesPreemption","TaskPrioritiesPreemption"]
tags: ["manual","workingwithtasks","taskprioritiespreemption","taskprioritiespreemption"]
source: "manual/WorkingWithTasks/TaskPrioritiesPreemption/TaskPrioritiesPreemption.html"
---

# Task Priorities and Preemption

## Overview

 

By default, task executers handle tasks using FIFO logic (first in, first out). They simply complete tasks in the order in which they were received and added to the task executer's task queue. However, you can make task executers work on more important tasks before less important ones using priority levels, preemption, labels, lists and/or other conditional logic.

 

*Priority levels* are numbers or labels you can assign to a task to make one task more important than another.

 

*Preemption* determines whether a task executer will be interrupted from its current task to work on a more important task or not. It also affects what happens to the active task that was interrupted. For example, the preemption can determine whether the active task is restarted from the beginning after the preempting task is complete or whether the task should be deleted entirely.

 

The following are examples of some scenarios in which priorities and preemption systems might be needed:

 

- **Look For Work Systems** - If you design a system in which task executers look for work to do by pulling tasks from a list, you can use priority levels to help the task executers decide which tasks to work on first.
- **Machine Breakdowns** - If a machine breaks down and a task executer is needed to repair it, that task executer's normal tasks could be interrupted while it works on the repairs.
- **Emergencies or Urgent Request** - If an emergency occurs that requires immediate attention or if an urgent request suddenly arrives, you'd need to use priorities and preemption to pull task executers away from their tasks to handle the emergency.

 

The rest of this topic will explain important concepts and different methods for creating priority and preemption system.

  

#### There's a Tutorial For That

 

The Task Logic Tools tutorial goes into depth about how to create priority and preemption system using the different tools for creating task logic.

  

 

 

## Priorities With Standard 3D Logic

 

The following sections will explain how to add task priorities if you are using standard 3D logic:

 

### How Priorities Work in Standard Logic

 

By default, all task sequences have a priority value of 0 unless you assign a different priority level to them. Task sequecnes with larger priority numbers will be completed before task sequences with a lower priority number. For example, a task sequence with a priority of 10 will be completed before a task sequence with a priority of 1.

 

The deeper meaning or strategy behind the priority numbers is entirely up to you. If you plan to use priority levels extensively in your simulation model, you might want to consider tracking and managing your priority levels. Consider writing down the levels and their meanings in your model's documentation for reference.

 

### Assigning Priorities

 

If you are creating tasks using the standard logic on a 3D object, look for properties that assign priority and/or preemption values in the same area where you created the task:

 

 

 

 

## Priorities With Process Flow Activities

 

You can't add priority levels or preemption values to individual process flow task activities. Instead you can use a Create Task Sequence activity to define a priority on a task sequence that you create. Alternately, you can handle prioritization within process flow, using lists, resources, etc. and only generate task sequences after job/resource selection has been accomplished.

 

For example, imagine you were modeling a system in which task executers needed to transport rush order items before transporting other items. To create this kind of system, you could:

 

1. Add an On Creation trigger to a source that assigns a label named *RushOrder* to flow items. A certain percentage of the flow items could be assigned a value of 1 to indicate that they are a rush order item. (All other items will be assigned a value of 0.)
1. When items are ready for transport, they get pushed to a list.
1. Task executers can query the list and pull items that have a *RushOrder* value of 1 and pull those items for transport first.

 

Try completing Task Logic Tutorial 1.3 for a hands-on example of this system.

 

 

 

## Strategies for Mixing Methods

 

If you have a model that uses both standard logic and process flow to assign tasks to task executers, you'll need to use priority and preemption strategies that are compatible with both systems. You can add priority and preemption to task sequences in process flow using a Create Task Sequence activity. Then, you can push the task sequence to a task sequence list. See Using Lists for Task Logic for an example of doing this. Alternately, you can dispatch the task sequence directly to a task executer or dispatcher, and allow the standard task sequence queue logic to be used.

 

 

 

## Preemption

 

Preemption determines whether a task executer will be interrupted away from its active task to work on a preempting task. It also determines what will happen to the active task when an operator is preempted away from it.

 

For example, the interrupted task could simply be re-added at the end of the task executer's task list. The task executer would then work on the interrupted task when it finished the preempting task. Alternatively, the active task could be entirely aborted so that it will never be worked on by the task executer.

 

The following table explains the different available preemption options:

 

| Value | Name | Description |
| --- | --- | --- |
| 0 | No preempt | The task will not preempt a task executer. |
| 1 | Preempt only | The task executer will preempt the currently active task sequence and put it back in its task sequence queue to be finished later. When a task sequence is preempted, it is automatically placed at the front of the task sequence queue. When the TaskExecuter eventually comes back to the original task sequence, the current task in that task sequence will be done over again, since it was not finished. |
| 2 | Preempt and abort active | The task executer will stop the currently active task sequence and destroy it, so that it will never come back to that original task. |
| 3 | Preempt and abort all | The task executer will stop the currently active task sequence, destroy it, and destroy all tasks in its task sequence queue. |

 

### Multiple Preempting Task Sequences

 

It's possible that a task executer could be working on a preempting task at the same time it receives another preempting task. When that happens, the task logic will use the following rules:

 

- **Priority Levels** - Task executers will use the priority value of the task sequence to determine which task sequence will take precedence.
- **When the new task has a higher priority value** - The task executer will preempt its current task sequence and execute the new one.
- **When the new task has a lesser or equal priority value** - The task executer will not preempt the active task sequence, but will queue up the new task sequence just like any other task sequence it receives.
- **Queuing tasks** - If the preempting tasks is added to the task executers task queue, it will not take the preempt value into account for its queueing logic unless you explicitly tell it to in the queue strategy. If you want the preempting task to be at the front of the queue, you could either assign a higher priority to the preempting task than all other task sequences or take preempting into account in the queue strategy.
