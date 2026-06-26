---
id: enablecode
name: "enablecode"
kind: command
module: ""
signature: "enablecode(node codenode [, num mode])"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Makes the node a code node"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# enablecode

```flexscript
enablecode(node codenode [, num mode])
```

Makes the node a code node

If mode is 0, then the node is set to FlexScript unless the node is already a code node and then it leaves the node as is. If mode is non-zero, then it forces a given toggle: 1-FlexScript, 2-C++, 3-DLL.
This command is used in the code edit window to toggle the node as FlexScript or DLL.

## Example

```flexscript
enablecode(getvarnode(current, "exittrigger"));
```

