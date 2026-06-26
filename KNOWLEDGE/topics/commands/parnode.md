---
id: parnode
name: "parnode"
kind: command
module: ""
signature: "parnode(num index) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "param"
description: "Deprecated, use param()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# parnode

> ⚠ **DEPRECATED.** Use `param` instead.

```flexscript
parnode(num index) -> node
```

Deprecated, use param()

This command is deprecated. Use param instead.
This command is used inside a function that is called by the nodefunction() command. It returns the parameter
passed to nodefunction specified by index as a node (or treenode). The first additional parameter passed to nodefunction() is
parameter 1, the second is parameter 2, etc. Parameters can also be retrieved using parval() and parstr() for casting them as
numbers and strings respectively.

## Example

```flexscript
If a trigger/field is called with: nodefunction(thefuncnode, item) then within the trigger/field, getting access to
the item passed as the first additional parameter would be done with:treenode item = parnode(1);
```

