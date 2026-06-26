---
id: tasksequence__task
name: "TaskSequence::Task"
kind: class
module: "TaskSequence"
signature: "TaskSequence::Task"
aliases: []
tags: ["class", "tasksequence-module"]
deprecated: false
replacedBy: null
description: "A class representing an individual task in a TaskSequence."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TaskExecuter\\TaskSequence.Task.xml"
---

# TaskSequence::Task

A class representing an individual task in a TaskSequence.

## Properties

### involved1

```flexscript
treenode TaskSequence::Task.involved1
```

Gets/sets the involved1 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
treenode involved1 = ts.currentTask.involved1;
...

### involved2

```flexscript
treenode TaskSequence::Task.involved2
```

Gets/sets the involved2 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
treenode involved2 = ts.currentTask.involved2;
...

### rank

```flexscript
int TaskSequence::Task.rank
```

Gets/sets the rank of the task inside its owning task sequence.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int rank = ts.currentTask.rank;
...

### state

```flexscript
int TaskSequence::Task.state (readonly)
```

Gets the state of the task.

The state of a task will be one of the following values:

TASKSTATE_UNFINISHED: The task is not yet finished, and it is not being actively executed.
TASKSTATE_ACTIVE: The task is currently being executed.
TASKSTATE_COORDINATED: Deprecated. The task is currently being executed as part of a coordinated task sequence.
TASKSTATE_FINISHED: The task has been finished.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int state = ts.currentTask.state;
...

### type

```flexscript
int TaskSequence::Task.type
```

Gets/sets the type of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
int curTaskType = ts.currentTask.type;
...

### var1

```flexscript
Variant TaskSequence::Task.var1
```

Gets/sets the var1 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
Variant var1 = ts.currentTask.var1;
...

### var2

```flexscript
Variant TaskSequence::Task.var2
```

Gets/sets the var2 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
Variant var2 = ts.currentTask.var2;
...

### var3

```flexscript
Variant TaskSequence::Task.var3
```

Gets/sets the var3 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
Variant var3 = ts.currentTask.var3;
...

### var4

```flexscript
Variant TaskSequence::Task.var4
```

Gets/sets the var4 value of the task.

TaskExecuter op = Model.find("Operator1");
TaskSequence ts = op.taskSequences[1];
Variant var4 = ts.currentTask.var4;
...

