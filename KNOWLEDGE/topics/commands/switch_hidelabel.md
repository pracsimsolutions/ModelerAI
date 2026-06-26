---
id: switch_hidelabel
name: "switch_hidelabel"
kind: command
module: ""
signature: "switch_hidelabel(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"Show Name\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hidelabel

```flexscript
switch_hidelabel(obj object[, num off/on]) -> num
```

Get or set the "Show Name" flag of the object

Get or set the "Show Name" flag of the object. A value of 1 will hide the object's name and stats from being displayed on the screen, a value of 0 will show the object's name and possibly stats depending on the viewhidealllabels attribute of the view window. A value of -1 gets the current value for the switch. The viewhidealllabels attribute of the view window has the following possible values which will apply to the label display of all objects in the window: 0=show name and stats, 1=hide name and stats, 2=show just names.

## Example

```flexscript
switch_hidelabel(so(),1)
```

