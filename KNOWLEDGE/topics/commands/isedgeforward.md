---
id: isedgeforward
name: "isedgeforward"
kind: command
module: ""
signature: "isedgeforward(obj netnode, num edgenr) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Returns 1 if the network node's edge is forward, 0 otherwise"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# isedgeforward

```flexscript
isedgeforward(obj netnode, num edgenr) -> num
```

Returns 1 if the network node's edge is forward, 0 otherwise

Returns 1 if the network node's edge is "forward" meaning when created, the connection was
dragged FROM this network node to the other network node. If the edge is forward, then the spline head is
contained by this network node, and the spline distance values go forward from this network node.

## Example

```flexscript
isedgeforward(current, 2)
```

