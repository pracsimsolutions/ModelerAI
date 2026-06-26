---
id: netnodefromrank
name: "netnodefromrank"
kind: command
module: ""
signature: "netnodefromrank(num rank) -> obj"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Returns the network node member at rank rank"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# netnodefromrank

```flexscript
netnodefromrank(num rank) -> obj
```

Returns the network node member at rank rank

Takes a network node member rank and returns the actual reference to the network node.

## Example

```flexscript
treenode netnode = netnodefromrank(5);
```

