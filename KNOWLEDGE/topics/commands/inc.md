---
id: inc
name: "inc"
kind: command
module: ""
signature: "inc(node thenode, num amount) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated, use += operator"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inc

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
inc(node thenode, num amount) -> num
```

Deprecated, use += operator

This command is deprecated. Use += operator instead. myNode.value += 2;

Increments the value of a node by a defined amount.

## Example

```flexscript
inc(current.mylabel,1)
```

