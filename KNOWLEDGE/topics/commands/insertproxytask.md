---
id: insertproxytask
name: "insertproxytask"
kind: command
module: ""
signature: "insertproxytask(obj tasksequence, num executerkey, num type [, num/obj involved1, num/obj involved2 , num var1, num var2, num var3, num var4]) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Inserts a proxy task into the coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# insertproxytask

```flexscript
insertproxytask(obj tasksequence, num executerkey, num type [, num/obj involved1, num/obj involved2 , num var1, num var2, num var3, num var4]) -> num
```

Inserts a proxy task into the coordinated task sequence

Inserts a proxy task into the specified coordinated task sequence.

## Example

```flexscript
insertproxytask(newts, dispatchkey, TASKTYPE_TRAVEL, current.centerObjects[1], NULL,0,0,0,0);
```

