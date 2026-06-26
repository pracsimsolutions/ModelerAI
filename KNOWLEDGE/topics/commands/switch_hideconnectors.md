---
id: switch_hideconnectors
name: "switch_hideconnectors"
kind: command
module: ""
signature: "switch_hideconnectors(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Hide Connectors\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hideconnectors

```flexscript
switch_hideconnectors(obj object[, num off/on]) -> num
```

Get or set the "Hide Connectors" flag of the object

Get or set the "Hide Connectors" flag of the object. A value of 1 is hide, a value of 0 is show, a value of -1 gets the current value. Hiding the connectors will turn off the display of ports and their connections both to and from the object. Note that view windows have an attribute named hideallconnectors that controls the display of connectors for ALL objects in their view.

## Example

```flexscript
switch_hideconnectors(so(),1)
```

