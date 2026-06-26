---
id: gettaskstate
name: "gettaskstate"
kind: command
module: ""
signature: "gettaskstate(obj tasksequence, num taskrank) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets the state of the task at rank taskrank of tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettaskstate

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettaskstate(obj tasksequence, num taskrank) -> num
```

Gets the state of the task at rank taskrank of tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. Gets the state of the task at rank taskrank of tasksequence. The following are task states:TASKSTATE_UNFINISHEDTASKSTATE_ACTIVETASKSTATE_FINISHEDTASKSTATE_COORDINATED

## Example

```flexscript
if(gettaskstate(gettasksequence(current.centerObjects[1], 0), 2) == TASKSTATE_ACTIVE)...
```

