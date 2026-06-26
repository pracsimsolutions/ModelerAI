---
id: ownerobject
name: "ownerobject"
kind: command
module: ""
signature: "ownerobject(node thenode) -> obj"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Gets the object that \"owns\" the executing node, usually the current object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ownerobject

```flexscript
ownerobject(node thenode) -> obj
```

Gets the object that "owns" the executing node, usually the current object

This command returns the node with object data attached to it that is the start of the sub-tree that thenode is in.
In other words, it returns the object node that contains thenode. The result of this command is cached on thenode, so that subsequent calls
to ownerobject(thenode) do not search in the tree, but return the cached result. If thenode subsequently changes parent, ownerobject(thenode) will
return the cached result.
This command is used in most code fields and trigger
fields to set the access variable current. In these fields, c references the node that contains the code, and ownerobject(c)
references the object that "owns" that node.

## Example

```flexscript
string objname = ownerobject(c).name;
This sets objname to the name of the object that contains the node referenced by c. The c usually refers to the node where the code is being written.
treenode current = ownerobject(c);
This example is present in most code fields in FlexSim, and retrieves access to the "current" object.
```

