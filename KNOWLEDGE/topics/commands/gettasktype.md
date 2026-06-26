---
id: gettasktype
name: "gettasktype"
kind: command
module: ""
signature: "gettasktype(obj tasksequence, num taskrank) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets the task type of the task at rank taskrank of tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettasktype

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettasktype(obj tasksequence, num taskrank) -> num
```

Gets the task type of the task at rank taskrank of tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. Returns the task type for the nth ranked task of the specified task sequence. The ranking is based on the initial ranking of the task, when the task sequence was initially created and tasks were added. It is not based on the ranking at a given time in the simulation, as tasks may have already been completed and subsequently destroyed.
You can also pass a reference to the TaskExecuter as the first parameter, and the value 0 as the second parameter to get the task type of the object's currently active task.

## Example

```flexscript
if(gettasktype(gettasksequence(current.centerObjects[1], 0), 2) == TASKTYPE_LOAD)...
```

