---
id: var
name: "var"
kind: command
module: ""
signature: "var(obj object, num index) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "getvarnode"
description: "Deprecated, use getvarnode()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# var

> ⚠ **DEPRECATED.** Use `getvarnode` instead.

```flexscript
var(obj object, num index) -> node
```

Deprecated, use getvarnode()

This command is deprecated. Use getvarnode() instead.

Get the node of an object in 'variables' by index.

## Example

```flexscript
var(so(),1)
```

