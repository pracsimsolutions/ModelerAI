---
id: nodeinsertafter
name: "nodeinsertafter"
kind: command
module: ""
signature: "nodeinsertafter(node targetnode) -> node"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Creates a new node immediately after targetnode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nodeinsertafter

```flexscript
nodeinsertafter(node targetnode) -> node
```

Creates a new node immediately after targetnode

This command creates a new node in the tree immediately after the targetnode. The new node has no data attached to it. Data is attached using the nodeadddata() command. Any node in the tree can be passed as targetnode.

## Example

```flexscript
nodeinsertafter(item.find(">labels").last).dataType = DATATYPE_NUMBER;
This creates a new label on the object referenced by item that holds number data.
```

