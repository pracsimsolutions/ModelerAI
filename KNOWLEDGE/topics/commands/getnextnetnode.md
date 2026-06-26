---
id: getnextnetnode
name: "getnextnetnode"
kind: command
module: ""
signature: "getnextnetnode(num fromnn, num tonn) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: "rankfromnetnode"
description: "Gets the rank of the next network node from fromnn in order to get to tonn"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnextnetnode

```flexscript
getnextnetnode(num fromnn, num tonn) -> num
```

Gets the rank of the next network node from fromnn in order to get to tonn

Returns the rank of the next network node to go to from one network node in getting to
the destination network node. The return value and parameters will all be network node member
ranks. Use rankfromnetnode() to get the member rank of a network node, and netnodefromrank()
to get the network node from the member rank.

## Example

```flexscript
treenode nextnode = netnodefromrank(getnextnetnode(rankfromnetnode(current), rankfromnetnode(Model.find("NN5"))));
```

