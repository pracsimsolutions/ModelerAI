---
id: nodejoin
name: "nodejoin"
kind: command
module: ""
signature: "nodejoin(node a, node b)"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: nodejoin."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nodejoin

```flexscript
nodejoin(node a, node b)
```

For developer use. Join two pointer nodes to form a bi-directional coupling. A bi-directional coupling in FlexSim is a pair of nodes who's data points to each other. FlexSim automatically manages the coupling. If either one of the endpoints is deleted or nullified, FlexSim cleans up to prevent dangling pointers. The two nodes a and b must have coupling data.

## Example

```flexscript
nodejoin(so(),so().next)
```

