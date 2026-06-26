---
id: movetasksequence
name: "movetasksequence"
kind: command
module: ""
signature: "movetasksequence(obj tasksequence, obj dispatcher)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "TaskSequence.move"
description: "Moves tasksequence into the dispatcher's tasksequence queue"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# movetasksequence

> ⚠ **DEPRECATED.** Use `TaskSequence.move` instead.

```flexscript
movetasksequence(obj tasksequence, obj dispatcher)
```

Moves tasksequence into the dispatcher's tasksequence queue

This command is deprecated. Use TaskSequence.move() instead. Moves the specified tasksequence into the specified dispatcher's tasksequence queue. WARNING!! This command should never be called on a tasksequence that is already being executed by a TaskExecuter.

## Example

```flexscript
movetasksequence(tasksequence, current.centerObjects[1]);
```

