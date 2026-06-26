---
id: findownerobject
name: "findownerobject"
kind: command
module: ""
signature: "findownerobject(node thenode) -> obj"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: findownerobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# findownerobject

```flexscript
findownerobject(node thenode) -> obj
```

Like ownerobject(), this command returns the object node containing thenode. Unlike ownerobject(), this command does not
cache the result, but searches in the tree for the "owner" node. For example when a TaskExecuter transfers from one NetworkNode to another on a network path, a coupling node is transferred to the new node to keep track of the TaskExecuter's current node, and findownerobject() is used to get access to the correct "owner" node.

## Example

```flexscript
findownerobject(c)
```

