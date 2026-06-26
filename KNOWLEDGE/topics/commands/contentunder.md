---
id: contentunder
name: "contentunder"
kind: command
module: ""
signature: "contentunder(node thenode [, num objectsonly) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: contentunder."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# contentunder

```flexscript
contentunder(node thenode [, num objectsonly) -> num
```

This command returns the number of nodes in the tree that are inside the node specified as thenode plus the number of nodes that may be contained in any sub-trees that begin at that layer. If thenode does not contain any other nodes, this command returns 0. If objectsonly is not specified or 0, any node in the FlexSim tree can be passed as thenode. If objectsonly is not 0, any node passed in as thenode that isn't an object will return 0.

## Example

```flexscript
if (contentunder(current) >= getvarnum(current,"maxcontent")) { current.input.close; }
This closes the inputs of current if the number of objects contained under current is greater than or equal to the variable called maxcontent of the object referenced by thisobjectdata.
```

