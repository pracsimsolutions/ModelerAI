---
id: changetask
name: "changetask"
kind: command
module: ""
signature: "changetask(obj tasksequence, num taskrank, num type [, obj involved1, obj involved2 , num var1, num var2, num var3, num var4] )"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Dynamically change a task in tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# changetask

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
changetask(obj tasksequence, num taskrank, num type [, obj involved1, obj involved2 , num var1, num var2, num var3, num var4] )
```

Dynamically change a task in tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. This command lets you change a task dynamically. For example, if you want an operator to travel to object B instead of traveling to object A, but a task has already been created for him to travel to object A, you can use this command to change the task.
The first parameter (tasksequence) specifies the tasksequence containing the task. The second parameter (taskrank) specifies the rank of the task in the tasksequence. All other parameters are the same as for the inserttask command.
This command should only be executed if the task is not the currently active task in the task sequence!

## Example

```flexscript
changetask(gettasksequence(current, 0), 4, TASKTYPE_TRAVEL, current.centerObjects[5]);
This example changes the fourth task in the current object's active task sequence to a travel task to travel to the object connected to the 5th center port of the current object.
```

