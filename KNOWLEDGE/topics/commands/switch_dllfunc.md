---
id: switch_dllfunc
name: "switch_dllfunc"
kind: command
module: ""
signature: "switch_dllfunc(node dllnode[, num on/off/get]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_dllfunc."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_dllfunc

```flexscript
switch_dllfunc(node dllnode[, num on/off/get]) -> num
```

Switches the node's dll toggle on or off. 1 switches it on, 0 off, and -1 gets the current state.

## Example

```flexscript
switch_dllfunc(getvarnode(current, "exittrigger"), 1);
```

