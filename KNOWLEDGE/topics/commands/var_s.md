---
id: var_s
name: "var_s"
kind: command
module: ""
signature: "var_s(obj object, str name) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "getvarnode"
description: "Deprecated, use getvarnode()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# var_s

> ⚠ **DEPRECATED.** Use `getvarnode` instead.

```flexscript
var_s(obj object, str name) -> node
```

Deprecated, use getvarnode()

This command is deprecated. Use getvarnode() instead.

Get the node of an object in 'variables' by name.

## Example

```flexscript
var_s(current,"myatt")
```

