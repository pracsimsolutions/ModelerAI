---
id: model
name: "model"
kind: command
module: ""
signature: "model() -> node"
aliases: []
tags: ["command", "group-tree-access"]
deprecated: false
replacedBy: null
description: "Reference the model's root node"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# model

```flexscript
model() -> node
```

Reference the model's root node

This command returns a reference to the model node in the main tree. It is used as a fast way of referencing the model.

## Example

```flexscript
treenode SourceObj = Model.find("Source1");
This sets the variable SourceObj so that it references the node called Source1 in the model.
```

