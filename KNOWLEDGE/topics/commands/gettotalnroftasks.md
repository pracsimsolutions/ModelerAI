---
id: gettotalnroftasks
name: "gettotalnroftasks"
kind: command
module: ""
signature: "gettotalnroftasks(obj tasksequence) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Gets the total number of tasks that were ever added to the tasksequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettotalnroftasks

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
gettotalnroftasks(obj tasksequence) -> num
```

Gets the total number of tasks that were ever added to the tasksequence

This command is deprecated. Use the TaskSequence and TaskSequence.Task class methods and properties instead. Returns the total number of tasks that were ever added to the tasksequence.

## Example

```flexscript
gettotalnroftasks(mytasksequence)
```

