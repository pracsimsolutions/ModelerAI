---
id: nodebreak
name: "nodebreak"
kind: command
module: ""
signature: "nodebreak(node a)"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: nodebreak."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nodebreak

```flexscript
nodebreak(node a)
```

For developer use. Breaks a coupling pointer on a node that has coupling data that is pointing to another node. If the node is part of a bi-directional coupling, the other coupling is not affected.

## Example

```flexscript
nodebreak(so())
```

