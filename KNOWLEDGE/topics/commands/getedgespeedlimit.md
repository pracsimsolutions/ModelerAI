---
id: getedgespeedlimit
name: "getedgespeedlimit"
kind: command
module: ""
signature: "getedgespeedlimit(obj netnode, num edgenum) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the speed limit of the edge of a network node"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getedgespeedlimit

```flexscript
getedgespeedlimit(obj netnode, num edgenum) -> num
```

Gets the speed limit of the edge of a network node

Returns the speed limit of the specified edge of the network node.

## Example

```flexscript
getedgespeedlimit(current, 1)
```

