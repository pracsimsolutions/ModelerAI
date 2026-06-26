---
id: rankfromnetnode
name: "rankfromnetnode"
kind: command
module: ""
signature: "rankfromnetnode(obj netnode) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Returns the member rank of network node netnode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rankfromnetnode

```flexscript
rankfromnetnode(obj netnode) -> num
```

Returns the member rank of network node netnode

Takes a network node reference and returns the member rank of that network node. Each network node has a specific "member rank", which is used as a
row/column reference in the network's distance and next node tables. The first network node that is added to a model gets member rank 1, the second gets member rank
2, and so on.

## Example

```flexscript
int memberrank = rankfromnetnode(current);
```

