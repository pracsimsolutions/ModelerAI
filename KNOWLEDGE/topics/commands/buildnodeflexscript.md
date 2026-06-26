---
id: buildnodeflexscript
name: "buildnodeflexscript"
kind: command
module: ""
signature: "buildnodeflexscript(node codenode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: buildnodeflexscript."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# buildnodeflexscript

```flexscript
buildnodeflexscript(node codenode)
```

For developer use. This command builds Flexscript trees for the node specified as node if it is designated as a Flexscript node. A Flexscript node must have a Flexscript tree made before it can run.

## Example

```flexscript
buildnodeflexscript(Table("mytable").cell(1,2));
This builds a Flexscript tree for the node at row 1 column 2 of the GlobalTable named "mytable", if it is a Flexscript node.
```

