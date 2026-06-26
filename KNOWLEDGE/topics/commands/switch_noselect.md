---
id: switch_noselect
name: "switch_noselect"
kind: command
module: ""
signature: "switch_noselect(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"No Select\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_noselect

```flexscript
switch_noselect(obj object[, num off/on]) -> num
```

Get or set the "No Select" flag of the object

Get or set the "No Select" flag of the node object (the object node will have a gray highlight in the treeview to indicate protected). A value of 1 is unselectable, a value of 0 is selectable, and a value of -1 gets the current value. Setting no select on an object keeps it from being clicked on with the mouse in the 3D view.

## Example

```flexscript
switch_noselect(so(),1)
```

