---
id: hidelabel
name: "hidelabel"
kind: command
module: ""
signature: "hidelabel(obj object[, num off/on])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "switch_hidelabel"
description: "Deprecated, use switch_hidelabel()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# hidelabel

> ⚠ **DEPRECATED.** Use `switch_hidelabel` instead.

```flexscript
hidelabel(obj object[, num off/on])
```

Deprecated, use switch_hidelabel()

This command is deprecated. Use switch_hidelabel() instead.

This command sets the specified object to show or hide its name and basic statistics on the screen. A value of 1 is show and a value of 0 is hide. If the name and statistics are hidden for an object, they will not be visible in any view window. View windows also have an attribute that controls this connector display of all objects in the view.

## Example

```flexscript
hidelabel(current,0);
This sets the object referenced by current so that its name and statistics will not be drawn.
```

