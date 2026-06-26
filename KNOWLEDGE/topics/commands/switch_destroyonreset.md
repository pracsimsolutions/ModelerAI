---
id: switch_destroyonreset
name: "switch_destroyonreset"
kind: command
module: ""
signature: "switch_destroyonreset(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"destroy on reset\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_destroyonreset

```flexscript
switch_destroyonreset(obj thenode[, num off/on]) -> num
```

Get or set the "destroy on reset" flag of the object

Get or set a node's "destroy on reset" flag. If the flag is 1, the node will be deleted when the model is reset. An off/on value of 0 or 1 sets the flag, and -1 gets the current value.

## Example

```flexscript
switch_destroyonreset(so(),1)
```

