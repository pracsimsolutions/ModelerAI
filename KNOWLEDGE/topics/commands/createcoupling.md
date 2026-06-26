---
id: createcoupling
name: "createcoupling"
kind: command
module: ""
signature: "createcoupling(node container1, node container2) -> node"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Creates a pointer coupling inside the two container nodes"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createcoupling

```flexscript
createcoupling(node container1, node container2) -> node
```

Creates a pointer coupling inside the two container nodes

## Example

```flexscript
createcoupling(current.labels["items"], item.labels["owner"]);
```

