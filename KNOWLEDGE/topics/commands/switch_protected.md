---
id: switch_protected
name: "switch_protected"
kind: command
module: ""
signature: "switch_protected(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Protected\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_protected

```flexscript
switch_protected(obj object[, num off/on]) -> num
```

Get or set the "Protected" flag of the object

Get or set the "Protected" flag of the node object (the object node will have a gray highlight in the treeview to indicate protected). A value of 1 is protected, a value of 0 is unprotected, and a value of -1 gets the current value. Protecting an object keeps it from being moved, rotated, or resized with the mouse, but not from being double-clicked to access the Parameters window for the object. View windows have an attribute named viewignoreobjects which can be used to ignore ALL the objects in their view which means the objects can not be manipulated in any way with the mouse including double-clicking.

## Example

```flexscript
switch_protected(so(),1)
```

