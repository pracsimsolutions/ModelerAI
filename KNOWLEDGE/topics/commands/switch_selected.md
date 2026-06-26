---
id: switch_selected
name: "switch_selected"
kind: command
module: ""
signature: "switch_selected(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Selected\" flag of thenode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_selected

```flexscript
switch_selected(obj thenode[, num off/on]) -> num
```

Get or set the "Selected" flag of thenode

Get or set the "Selected" flag of thenode (red highlight). A value of 1 is selected, a value of 0 is unselected, a value of -1 gets the current value.

## Example

```flexscript
switch_selected(so(),1)
```

