---
id: taskexecuter
name: "TaskExecuter"
kind: class
module: ""
signature: "TaskExecuter"
aliases: []
tags: ["class"]
deprecated: false
replacedBy: null
description: "A class that represents a Dispatcher that can also execute TaskSequences."
seeAlso: []
source: "help\\FlexScriptAPIReference\\TaskExecuter\\TaskExecuter.xml"
---

# TaskExecuter

A class that represents a Dispatcher that can also execute TaskSequences.

## Properties

### activeTask

```flexscript
TaskSequence.Task TaskExecuter.activeTask (readonly)
```

Accesses the task that is currently being executed by the object.

TaskExecuter obj = Model.find("Operator1");
int taskType = obj.activeTask.type;
...

### activeTaskSequence

```flexscript
TaskSequence TaskExecuter.activeTaskSequence (readonly)
```

Accesses the task sequence that is currently being executed by the object.

TaskExecuter obj = Model.find("Operator1");
double tsPriority = obj.activeTaskSequence.priority;
...

### DefaultEndSpeed

```flexscript
int TaskExecuter.DefaultEndSpeed (readonly)
```

A property to enter for a task's end speed parameter. If this value is entered, then the task's end speed will be whatever the default for that task and/or navigator is.

This is an alias for the value 0. For travel tasks, by default an end speed value
of 0 generally means end at max speed, whereas for offset tasks (load, unload, travel relative, travel to
loc, etc.) the value 0 means zero end speed. This alias essentially means use whatever is the
default for the task.

### MaxEndSpeed

```flexscript
int TaskExecuter.MaxEndSpeed (readonly)
```

A property to enter for a task's end speed parameter. If this value is entered, then the task's end speed will be the task executer's max speed.

### navigator

```flexscript
Navigator TaskExecuter.navigator (readonly)
```

Accesses the task executer's navigator, i.e. the object that manages travel tasks.

### ZeroEndSpeed

```flexscript
int TaskExecuter.ZeroEndSpeed (readonly)
```

A property to enter for a task's end speed parameter. If this value is entered, then the task's end speed will always be zero.

