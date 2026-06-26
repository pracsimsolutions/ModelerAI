---
id: insertdeallocatetask
name: "insertdeallocatetask"
kind: command
module: ""
signature: "insertdeallocatetask(obj tasksequence, num allocationkey [, num noblock] ) -> num"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Inserts a deallocate task into the coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# insertdeallocatetask

```flexscript
insertdeallocatetask(obj tasksequence, num allocationkey [, num noblock] ) -> num
```

Inserts a deallocate task into the coordinated task sequence

Inserts a Deallocate Task into the specified coordinated task sequence to deallocate the resource specified by allocationkey. By default this task will block, but if noblock is 1, it will not block.

## Example

```flexscript
insertdeallocatetask(newts,forkkey);
```

