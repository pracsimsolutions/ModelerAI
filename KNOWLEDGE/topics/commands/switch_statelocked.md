---
id: switch_statelocked
name: "switch_statelocked"
kind: command
module: ""
signature: "switch_statelocked(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_statelocked."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_statelocked

```flexscript
switch_statelocked(obj object[, num off/on]) -> num
```

Get or set the "is code state locked" flag of the node object. A value of 1 is enable, a value of 0 is disable, a value of -1 gets the current value. If enabled, the node will be locked as either C++ or flexscript depending on what it currently is (the model will not change it when "Make All Code C++/Flexscript" is chosen through the main menu.

## Example

```flexscript
switch_statelocked(so(),1)
```

