---
id: nametomodelpath
name: "nametomodelpath"
kind: command
module: ""
signature: "nametomodelpath(obj node, obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "treenode.getPath"
description: "Deprecated, use treenode.getPath()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# nametomodelpath

> ⚠ **DEPRECATED.** Use `treenode.getPath` instead.

```flexscript
nametomodelpath(obj node, obj object)
```

Deprecated, use treenode.getPath()

This command is deprecated. Use treenode.getPath() instead.

Sets the name of a node to the path of the object relative to the model.

## Example

```flexscript
nametomodelpath(curlistnode, current);
```

