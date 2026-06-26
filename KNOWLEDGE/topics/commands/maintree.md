---
id: maintree
name: "maintree"
kind: command
module: ""
signature: "maintree() -> node"
aliases: []
tags: ["command", "group-tree-access"]
deprecated: false
replacedBy: null
description: "Reference to the main tree"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# maintree

```flexscript
maintree() -> node
```

Reference to the main tree

Returns a reference to the main tree. When defining string paths to nodes in the main tree, MAIN can be used as in the following example node("MAIN:/project/exec/globals/cppfunctions");

## Example

```flexscript
maintree().find("project/exec/globals/cppfunctions");
```

