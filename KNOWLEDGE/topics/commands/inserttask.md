---
id: inserttask
name: "inserttask"
kind: command
module: ""
signature: "inserttask(obj tasksequence, num type [, obj involved1, obj involved2 , num var1, num var2, num var3, num var4] )"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "TaskSequence.addTask"
description: "Inserts a new task to the task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inserttask

> ⚠ **DEPRECATED.** Use `TaskSequence.addTask` instead.

```flexscript
inserttask(obj tasksequence, num type [, obj involved1, obj involved2 , num var1, num var2, num var3, num var4] )
```

Inserts a new task to the task sequence

This command is deprecated. Use TaskSequence.addTask() instead. Inserts a task into the specified tasksequence of the specified type. Depending on the task type, the remaining parameters will have a different meaning. Refer to the Task Type Quick Reference guide in the Users Manual for an explanation of the parameters associated with each task type.

## Example

```flexscript
inserttask(ts, TASKTYPE_TRAVEL, current, NULL);
```

