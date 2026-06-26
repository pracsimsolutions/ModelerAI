---
id: switch_cppfunc
name: "switch_cppfunc"
kind: command
module: ""
signature: "switch_cppfunc(node thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"C++\" flag of thenode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_cppfunc

```flexscript
switch_cppfunc(node thenode[, num off/on]) -> num
```

Get or set the "C++" flag of thenode

Get or set the "C++" flag of thenode. A value of 1 is C++, a value of 0 is not C++, a value of -1 gets the current value. If a node is toggled as C++ it will be compiled as a C++ function at time of compile.

## Example

```flexscript
switch_cppfunc(so(),1)
```

