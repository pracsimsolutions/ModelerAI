---
id: getnroftasks
name: "getnroftasks"
kind: command
module: ""
signature: "getnroftasks(obj tasksequence) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets the number of uncompleted tasks in the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnroftasks

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
getnroftasks(obj tasksequence) -> num
```

Gets the number of uncompleted tasks in the tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. Returns the number of tasks that still need to be completed in the tasksequence (including the current task).

## Example

```flexscript
if(getnroftasks(mytasksequence) > 5) pt("There are more than 5 tasks in the tasksequence queue.");
```

