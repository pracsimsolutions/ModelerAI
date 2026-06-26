---
id: getedgedist
name: "getedgedist"
kind: command
module: ""
signature: "getedgedist(obj netnode, num edgenum) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the distance of the edge of a network node"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getedgedist

```flexscript
getedgedist(obj netnode, num edgenum) -> num
```

Gets the distance of the edge of a network node

Returns the distance of the specified edge of the network node.

## Example

```flexscript
getedgedist(current, 1)
```

