---
id: reassignnetnode
name: "reassignnetnode"
kind: command
module: ""
signature: "reassignnetnode(obj taskexecuter, obj networknode)"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "FlexScript command: reassignnetnode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# reassignnetnode

```flexscript
reassignnetnode(obj taskexecuter, obj networknode)
```

TaskExecuters using a NetworkNode system are always associated with a specific NetworkNode in the system (red line graphic) which continually changes as they travel around. This command reassigns the specified taskexecuter to networknode.Warning!!! Do not execute this command unless the TaskExecuter is on a network, and unless you know the TaskExecuter is not currently executing a travel task.

## Example

```flexscript
reassignnetnode(current, current.centrObjects[5]);
Reassign the current TaskExecuter to the NetworkNode connected to its fifth center port.
```

