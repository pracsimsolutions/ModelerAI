---
id: switch_hideshape
name: "switch_hideshape"
kind: command
module: ""
signature: "switch_hideshape(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Show 3D Shape\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hideshape

```flexscript
switch_hideshape(obj object[, num off/on]) -> num
```

Get or set the "Show 3D Shape" flag of the object

Get or set the "Show 3D Shape" flag of the object. A value of 1 is hide, a value of 0 is show, and a value of -1 gets the current value. Note that view windows also have an attribute called hidealldrawcontent that controls the display of 3D shapes for ALL objects in their view.

## Example

```flexscript
switch_hideshape(so(),1)
```

