---
id: gettaskvariable
name: "gettaskvariable"
kind: command
module: ""
signature: "gettaskvariable(obj tasksequence, num taskrank, num variablenum) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets the tasksequence variable for the task at rank taskrank of tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettaskvariable

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettaskvariable(obj tasksequence, num taskrank, num variablenum) -> num
```

Gets the tasksequence variable for the task at rank taskrank of tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. Returns a reference to one of the four task variables for the nth ranked task of the specified task sequence. The ranking is based on the initial ranking of the task, when the task sequence was initially created and tasks were added. It is not based on the ranking at a given time in the simulation, as tasks may have already been completed and subsequently destroyed. The third parameter defines which task variable it is (1 - 4)
You can also pass a reference to the TaskExecuter as the first parameter, and the value 0 as the second parameter to get the task variable of the object's currently active task.

## Example

```flexscript
double myvar = gettaskvariable(gettasksequence(current.centerObjects[1], 0), 2, 1);
```

