---
id: tasksequence
name: "TaskSequence"
kind: class
module: ""
signature: "TaskSequence"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class representing a sequence of tasks to be performed by a TaskExecuter."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TaskExecuter\\TaskSequence.xml"
---

# TaskSequence

A class representing a sequence of tasks to be performed by a TaskExecuter.

See Task Sequence Concepts for
more information on task sequences.

## Methods

### create

```flexscript
TaskSequence (static) TaskSequence.create(treenode obj, double priority = 0, int preempt = 0, int waitForFinishState = 0)
```

Creates a new task sequence.

**Parameters:**
- `obj` *(treenode)* — Specifies the object in which to create the task sequence. Generally this should be a Dispatcher or TaskExecuter. In such a case, the task sequence will be created in the task sequence queue of the object. If the object is not a Dispatcher or TaskExecuter, then the task sequence will be created as a subnode of that object/node.
- `priority` *(double, default 0)* — Optional. Defines the priority value of the task sequence.
- `preempt` *(int, default 0)* — Optional. Defines the preempt value of the task sequence. If not zero, it should be one of the following values: PREEMPT_ONLY, PREEMPT_AND_ABORT_ACTIVE, or PREEMPT_AND_ABORT_ALL.
- `waitForFinishState` *(int, default 0)* — Optional. Defines the state to go into if the task sequence reaches the end of its set of tasks, with no additional tasks added. If you pass 0 (default), the task sequence will simply be finished and destroyed when it reaches the end. When non-zero, the task sequence will add a TASKTYPE_WAITFORTASKS task and subsequently go into the defined state, waiting for a task to be added. If you pass a non-zero value here, then you must later call TaskSequence.finish() to notify the task sequence no longer needs to wait for new tasks.

**Returns:** The created task sequence.

TaskSequence ts = TaskSequence.create(Model.find("Operator1"));
ts.addTask(TASKTYPE_LOAD, item, current);
...
ts.dispatch();

### addTask

```flexscript
TaskSequence.Task TaskSequence.addTask(int type, treenode involved1 = null, treenode involved2 = null, Variant var1 = nullvar, Variant var2 = nullvar, Variant var3 = nullvar, Variant var4 = nullvar)
```

Adds a new task to the task sequence.

**Parameters:**
- `type` *(int)*
- `involved1` *(treenode, default null)* — The first involved object of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.
- `involved2` *(treenode, default null)* — The second involved object of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.
- `var1` *(Variant, default nullvar)* — The first variable of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.
- `var2` *(Variant, default nullvar)* — The second variable of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.
- `var3` *(Variant, default nullvar)* — The third variable of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.
- `var4` *(Variant, default nullvar)* — The fourth variable of the task. The meaning of this is dependent on the task type. See Task Type Quick Reference for more information.

**Returns:** The added task.

TaskSequence ts = TaskSequence.create(Model.find("Operator1"));
ts.addTask(TASKTYPE_LOAD, item, current);
...
ts.dispatch();

### dispatch

```flexscript
void TaskSequence.dispatch()
```

Dispatches the task sequence.

Calling this method initiates Dispatcher logic for receiving the task sequence
and dispatching it as needed. If the object on whom the task sequence was created is a
TaskExecuter and the TaskExecuter is not already executing another task sequence, then
the object will immediately begin execution of the task sequence.

TaskSequence ts = TaskSequence.create(Model.find("Operator1"));
ts.addTask(TASKTYPE_LOAD, item, current);
...
ts.dispatch();

### finish

```flexscript
void TaskSequence.finish()
```

Marks the task sequence as finished.

This method resets the waitForFinishState value of the task sequence to 0, and finishes the task sequence
if it is currently waiting for more tasks to be added. Calling this method is only required if you previously passed
a non-zero waitForFinishState value into
TaskSequence.create().

### move

```flexscript
void TaskSequence.move(Dispatcher dispatcher)
```

Moves the task sequence into a new dispatcher's task sequence queue.

**Parameters:**
- `dispatcher` *(Dispatcher)* — The dispatcher to move the task sequence to.

## Properties

### currentTask

```flexscript
TaskSequence.Task TaskSequence.currentTask (readonly)
```

Accesses the first task in the task sequence that has not yet finished execution. This task may be currently executing, or it may be waiting for execution because the task sequence itself is waiting for execution.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int curTaskType = ts.currentTask.type;
...

### labelProperties

```flexscript
Variant TaskSequence.labelProperties
```

Use your own named properties to get/set label values.

The TaskSequence class allows you to get and set custom named properties that will apply to labels on the task sequence. For example,
if you want to set a label named "MyLabel" on the task sequence ts to a value of 5, you can write the following code:
ts.MyLabel = 5;
This will set the label's value to 5. If a label of that name does not exist, then it will be added to the object's labels.
If you want to return the value stored in ts's "MyLabel" label, you can execute the following code:
return ts.MyLabel;
This works the same as the treenode class's dynamic property mechanism. See
treenode.labelProperties for more information

### preempt

```flexscript
int TaskSequence.preempt
```

Gets/sets the preempt value of the task sequence.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int preempt = ts.preempt;
...

### priority

```flexscript
double TaskSequence.priority
```

Gets/sets the priority value of the task sequence.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
double priority = ts.priority;
...

### rank

```flexscript
int TaskSequence.rank
```

Gets/sets the rank of the task sequence in its task sequence queue.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int tsRank = ts.rank; // should be 1
...

### tasks

```flexscript
TaskSequence.tasks (readonly)
```

Accesses the set of tasks in the task sequence. This member can be used like an array of TaskSequence.Tasks.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int numTasks = ts.tasks.length;
for (int i = 1; i <= numTasks; i++) {
Task task = ts.tasks[i];
}

