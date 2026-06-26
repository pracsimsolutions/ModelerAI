---
id: getcoordinatedtasksequence
name: "getcoordinatedtasksequence"
kind: command
module: ""
signature: "getcoordinatedtasksequence(obj dispatcher, num tasksequencerank) -> node"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets a reference to a coordinated task sequence"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getcoordinatedtasksequence

```flexscript
getcoordinatedtasksequence(obj dispatcher, num tasksequencerank) -> node
```

Gets a reference to a coordinated task sequence

Returns a reference to the coordinated task sequence that is associated with allocation request with the specified rank in the specified dispatcher's task sequence queue. If the specified rank is zero, then the command assumes that the dispatcher is actually a TaskExecuter, and it returnsa reference to the coordinated task sequence that it is currently allocated for.

## Example

```flexscript
treenode coordtasksequence = getcoordinatedtasksequence(current.centerObjects[1], 5);
```

