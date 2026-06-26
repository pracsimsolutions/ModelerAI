---
id: tonode
name: "tonode"
kind: command
module: ""
signature: "tonode(thing) -> node"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: null
description: "Assert type as a node pointer"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# tonode

```flexscript
tonode(thing) -> node
```

Assert type as a node pointer

Assert type as a node pointer. Only serves a real purpose in C++.

## Example

```flexscript
tonode(getnodenum(couplingnode))
Converts the numeric value of couplingnode to a treenode
```

