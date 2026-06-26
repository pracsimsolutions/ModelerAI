---
id: createcoordinatedtasksequence
name: "createcoordinatedtasksequence"
kind: command
module: ""
signature: "createcoordinatedtasksequence(obj dispatcher) -> node"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Create a new, empty coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createcoordinatedtasksequence

```flexscript
createcoordinatedtasksequence(obj dispatcher) -> node
```

Create a new, empty coordinated task sequence

Create a new, empty coordinated task sequence on the specified Dispatcher. Must be followed at some point by dispatchcoordinatedtasksequence.

## Example

```flexscript
createcoordinatedtasksequence(current.centerObjects[1]);
```

