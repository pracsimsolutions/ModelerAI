---
id: getnetworkdistance
name: "getnetworkdistance"
kind: command
module: ""
signature: "getnetworkdistance(num fromnetnoderank, num tonetnoderank) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: "rankfromnetnode"
description: "Gets the network distance between two network nodes"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnetworkdistance

```flexscript
getnetworkdistance(num fromnetnoderank, num tonetnoderank) -> num
```

Gets the network distance between two network nodes

Returns the network distance between the network nodes. Note that the
network nodes are referenced by their member ranks, not by the pointer to the network node.
Use rankfromnetnode() to get a network node's member rank.

## Example

```flexscript
getnetworkdistance(rankfromnetnode(current), rankfromnetnode(current.outObjects[1]));
```

