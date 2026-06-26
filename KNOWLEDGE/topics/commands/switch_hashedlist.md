---
id: switch_hashedlist
name: "switch_hashedlist"
kind: command
module: ""
signature: "switch_hashedlist(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Hashed List\" flag of thenode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hashedlist

```flexscript
switch_hashedlist(obj thenode[, num off/on]) -> num
```

Get or set the "Hashed List" flag of thenode

Get or set the "Hashed List" flag of the thenode. A value of 1 sets the node to store a hashed list of its subnodes for fast lookup by name, a value of 0 sets the node to not store a hashed list of subnodes, a value of -1 gets the current value.

## Example

```flexscript
switch_hashedlist(so(),1)
```

