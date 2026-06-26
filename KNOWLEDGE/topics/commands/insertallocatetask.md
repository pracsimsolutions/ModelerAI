---
id: insertallocatetask
name: "insertallocatetask"
kind: command
module: ""
signature: "insertallocatetask(obj tasksequence, obj dispatcher, num priority, num preempt [, num noblock] ) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Inserts an allocate task into the coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# insertallocatetask

```flexscript
insertallocatetask(obj tasksequence, obj dispatcher, num priority, num preempt [, num noblock] ) -> num
```

Inserts an allocate task into the coordinated task sequence

Inserts an Allocate Task into the specified coordinated task sequence. By default the allocation task is a blocking task, but if noblock is 1, then the allocation does not block further execution of the task sequence.

## Example

```flexscript
insertallocatetask(newts, current.centerObjects[2], 0, 0);
```

