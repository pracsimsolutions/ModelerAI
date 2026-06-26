---
id: dispatchtasksequence
name: "dispatchtasksequence"
kind: command
module: ""
signature: "dispatchtasksequence(obj tasksequence)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "TaskSequence.dispatch"
description: "Dispatches the task sequence, initiating handling/execution of the task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dispatchtasksequence

> ⚠ **DEPRECATED.** Use `TaskSequence.dispatch` instead.

```flexscript
dispatchtasksequence(obj tasksequence)
```

Dispatches the task sequence, initiating handling/execution of the task sequence

This command is deprecated. Use TaskSequence.dispatch()
instead. Tells the dispatcher/taskexecuter in which the specified task sequence resides to handle that task sequence. Users should create the task sequence using the createemptytasksequence command, then add tasks using the inserttask command, then cause the task sequence to be handled using the dispatchtasksequence command.

## Example

```flexscript
dispatchtasksequence(newtasksequence);
```

