---
id: gettaskinvolved
name: "gettaskinvolved"
kind: command
module: ""
signature: "gettaskinvolved(obj tasksequence, num taskrank, num involvednum) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets a reference to the first or second involved object of the task of the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettaskinvolved

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettaskinvolved(obj tasksequence, num taskrank, num involvednum) -> node
```

Gets a reference to the first or second involved object of the task of the tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task classes instead. Returns a reference to either the first or second involved object for the nth ranked task of the specified task sequence. The ranking is based on the initial ranking of the task, when the task sequence was initially created and tasks were added. It is not based on the ranking at a given time in the simulation, as tasks may have already been completed and subsequently destroyed. The third parameter defines which involved object it is (1 or 2)
You can also pass a reference to the TaskExecuter as the first parameter, and the value 0 as the second parameter to get the involved of the object's currently active task.

## Example

```flexscript
treenode loaditem = getinvolved(gettasksequence(current.centerObjects[1], 0), 2, 1);
```

