---
id: workingwithtasks_howtaskexecutershandle_howtaskexecutershandle
name: "How Task Executers Handle Tasks"
kind: manual
breadcrumb: ["WorkingWithTasks","HowTaskExecutersHandle","HowTaskExecutersHandle"]
tags: ["manual","workingwithtasks","howtaskexecutershandle","howtaskexecutershandle"]
source: "manual/WorkingWithTasks/HowTaskExecutersHandle/HowTaskExecutersHandle.html"
---

# How Task Executers Handle Tasks

## Introduction to How Task Executers Handle Tasks

 

This topic will explain how task executers receive and execute task sequences. If you are using complex task logic in your simulation model, you might need to understand the concepts in this topic so that you are aware of how FlexSim handles task logic behind the scenes. This knowledge will help you understand and troubleshoot potential task logic problems---especially related to task dispatching logic and its related issues, such as task priorities and preemption.

 

 

 

## Task Sequences and Tasks

 

Sometimes you might see the terms *task* and *task sequence* used interchangeably in this manual and in other FlexSim resources. While loose terminology usage isn't terribly problematic, there are very specific, and unique, technical meanings of the terms *task* and *task sequence* which are important to understand.

 

### Definitions

 

**Task** - A single instruction or action that is performed by a task executer. Examples are load, unload, and travel.

 

**Task Sequence** - A collection of one or more tasks that are meant to be performed in sequence, generally uninterrupted, by a task executer. In addition to containing a sequence of *tasks* to perform, a task sequence holds values for priority and preemption, and can also store user-defined label values.

 

 

By default, a task executer will work on all the tasks in an entire task sequence from start to finish. Then, once the task executer has finished every task in the task sequence, it will finish/destroy the task sequence, and then look for a next task sequence to execute.

 

### Task Sequences vs. Tasks: What This Means for Dispatching

 

Comprehending the difference between *task sequences* and *tasks* is important to understanding and controlling dispatching logic in FlexSim. If we think of a *task* as the most basic unit of work in FlexSim, a *task sequence* is the most basic unit of dispatching. Task sequences, not individual tasks, can be dispatched and moved around to different queues or lists. They can be prioritized and selected based on custom criteria, etc. Further, a task sequence enforces a specific ordering of task execution.

  

### Task Sequences vs. Tasks: Process Flow Confusion

 

The Process Flow tool provides a very useful mechanism that lets you dispatch individual tasks directly to task executers. When you add a task sequence activity like Load, you can directly specify a task executer in the Executer / Task Sequence field. While this appears to dispatch a *task* directly to the task executer, what is really happening behind the scenes is the activity is creating a *task sequence* with a single *task* inside it, and dispatching that *task sequence* to the task executer. As described above, you can only dispatch *task sequences*, not *tasks*, to task executers.

 

Often, using this process flow feature is not problematic. However, problems can arise specifically when you have other mechanisms (standard 3D object logic, other parts of process flow, etc.) that are also dispatching task sequences to the same object. Since this feature creates a task sequence with a single task, there is no constraint forcing the task executer to perform multiple tasks in the order that you define them in a process flow. It could perform one task from the process flow, then jump to another, completely separate task sequence, before performing the next task you give it.

 

If or when these problems arise, you can use the Create Task Sequence activity to explicitly create a task sequence. Once created, you add tasks to the task sequence, instead of directly to the task executer. This will correlate each added task with that single task sequence, and thus enforce a specific ordering by which to perform the tasks. Once you're finished with the task sequence, add the Finish Task Sequence activity. This will finish the task sequence, releasing the task executer to other task sequences.

  

 

 

## How Task Executers Handle Task Sequences

 

Task executers primarily keep track of two things:

 

1. **Active task sequence** - The task sequence the task executer is currently working on.
1. **Task sequence queue** - A queue of task sequences the task executer has been assigned to work on.

 

You can view a Task Executer's active task sequence and task sequence queue by right-clicking on it and selecting **View Task Sequence Queue**.

 

 

During a simulation run, when a task executer receives a task sequence:

 

1. The new task sequence will be added to the task executer's task sequence queue.
1. If the task executer does not currently have an active task sequence, it will select a task sequence from its queue. Usually, a task executer will complete task sequences in the order they are received. In other words, it will do the tasks using FIFO logic (first in, first out). However, if you use task sequences with different priorities, the task executer will complete the task sequences in the order of their priority instead.
1. The selected task sequence will become the active task sequence.
1. The task executer will complete the tasks in the task sequence, generally uninterrupted, in the order they were added to the task sequence.
1. After the active task sequence is complete, the task executer will select another task sequence from the queue and the process will repeat itself.

  

#### Exceptions to the "Execute Uninterrupted" Rule

 

While, generally, a task executer will execute all of the tasks in a task sequence uninterrupted, there are some exceptions to this rule, as follows.

 

- **Preemption** - If, while executing a task sequence, the task executer receives another task sequence whose preemption value is set to preempt, the task executer will immediately stop executing its current task sequence, and start the new task sequence. See Task Sequence Priorities and Preemption for more information.
- **Break Tasks** - If the task executer reaches a break task in the task sequence, it can break away from the current task sequence to perform some other task sequence. This is used primarily for simulating multi-item capacity. The task executer will perform part of one task sequence that loads one item, then if it still has load capacity, it can break to another task sequence that loads another item, before finishing the original task sequence that tells it to unload the original item.
- **Call-Subtasks Tasks** - If the task executer reaches a call-subtasks task in the task sequence, it will break from the current task sequence, fully execute another task sequence, then return back to the original task sequence. While this can be useful when using the Task Sequence API, generally process flow will allow you to do everything you need without using call-subtasks.

  

### Dispatchers vs. Task Executers

 

A dispatcher object is similar to a task executer, except that it does not have an active task sequence. It only has a task sequence queue. This is what distinguishes the Dispatcher class from the TaskExecuter and its sub-classes.

 

 

 

## Task Executer Events and Triggers

 

Task executers have a specific series of events and triggers that they go through while working on tasks. (See Key Concepts About Events for information about events and triggers.) A few of the important events are:

 

| Trigger | Explanation |
| --- | --- |
| On Start Task | Fires whenever the task executer begins performing a task |
| On Finish Task | Fires whenever the task executer finishes performing a task. |
| On Resource Available | Fires whenever the task executer has finished a task sequence, and is ready to execute another task sequence. |
| On Load | Fires when a task executer loads an item as part of a load task |
| On Unload | Fires when a task executer unloads an item as part of an unload task |

 

You can set custom animations or process flow activities to begin when any of these triggers are fired. You could also use triggers to design look-for-work systems in which task executers pull task sequences from a global list when their On Resource Available trigger fires. See Task Executer Concepts - Events for a more thorough explanation of the available events and triggers for task executers.
