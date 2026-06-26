---
id: switch_orphancoupling
name: "switch_orphancoupling"
kind: command
module: ""
signature: "switch_orphancoupling(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"orphan coupling\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_orphancoupling

```flexscript
switch_orphancoupling(obj thenode[, num off/on]) -> num
```

Get or set the "orphan coupling" flag of the object

Get or set a node's "orphan coupling" flag. If the flag is 1, when the coupling node is copied, the copy won't be destroyed if it no longer points to a valid node. An off/on value of 0 or 1 sets the flag, and -1 gets the current value.

## Example

```flexscript
switch_orphancoupling(so(),1)
```

