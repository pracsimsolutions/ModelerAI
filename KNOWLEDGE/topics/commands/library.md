---
id: library
name: "library"
kind: command
module: ""
signature: "library() -> node"
aliases: []
tags: ["command", "group-tree-access"]
deprecated: false
replacedBy: null
description: "Reference to the library node in the main tree"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# library

```flexscript
library() -> node
```

Reference to the library node in the main tree

This command returns a reference to the library node in the main tree. It is used as a fast way of referencing the library.

## Example

```flexscript
treenode SourceObj = library().find("?Source");
This sets the variable SourceObj so that it references the node called Source in the library.
```

