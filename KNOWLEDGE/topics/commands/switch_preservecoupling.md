---
id: switch_preservecoupling
name: "switch_preservecoupling"
kind: command
module: ""
signature: "switch_preservecoupling(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"preserve coupling\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_preservecoupling

```flexscript
switch_preservecoupling(obj thenode[, num off/on]) -> num
```

Get or set the "preserve coupling" flag of the object

Get or set a node's "preserve coupling" flag. If the flag is 1, when the coupling node is copied, the copy will also point to its partner. For one-way couplings, the original and the copy will both point at the same partner node. For two-way couplings, a new partner node will be created. An off/on value of 0 or 1 sets the flag, and -1 gets the current value.

## Example

```flexscript
switch_preservecoupling(so(),1)
```

