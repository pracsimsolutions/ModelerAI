---
id: switch_hidecontents
name: "switch_hidecontents"
kind: command
module: ""
signature: "switch_hidecontents(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Show Contents\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hidecontents

```flexscript
switch_hidecontents(obj object[, num off/on]) -> num
```

Get or set the "Show Contents" flag of the object

Get or set the "Show Contents" flag of the object. A value of 1 is hide, a value of 0 is show, a value of -1 gets the current value. Hiding the contents of an objects will stop the display of objects (e.g. flowitems) contained within it.

## Example

```flexscript
switch_hidecontents(so(),1)
```

