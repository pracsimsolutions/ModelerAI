---
id: att_s
name: "att_s"
kind: command
module: ""
signature: "att_s(obj object, str name)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "getvarnode"
description: "Deprecated, use getvarnode()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# att_s

> ⚠ **DEPRECATED.** Use `getvarnode` instead.

```flexscript
att_s(obj object, str name)
```

Deprecated, use getvarnode()

This command is deprecated. Use getvarnode() instead.

Get the attribute node of an object by name. An attribute node is one which resides inside the "variables" node of the structure of an object. This function will return the pointer to the node called name inside the variables node of object. var_s supercedes this command.

## Example

```flexscript
att_s(current,"myatt")
```

