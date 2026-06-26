---
id: switch_expanded
name: "switch_expanded"
kind: command
module: ""
signature: "switch_expanded(node thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Expand/collaps sub-level nodes of thenode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_expanded

```flexscript
switch_expanded(node thenode[, num off/on]) -> num
```

Expand/collaps sub-level nodes of thenode

Expand or collapse sub-level nodes of thenode in the tree view. This is analogous to clicking the + and - sign of thenode in the tree view. A value of 1 is expand, a value of 0 is collapse, a value of -1 gets the current value.

## Example

```flexscript
switch_expanded(so(),1)
```

