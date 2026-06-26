---
id: switch_hidebase
name: "switch_hidebase"
kind: command
module: ""
signature: "switch_hidebase(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated, do not use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hidebase

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
switch_hidebase(obj object[, num off/on]) -> num
```

Deprecated, do not use

This command is deprecated. Do not use. Get or set the "Show 2D Shape" flag of the object. A value of 1 is hide, a value of 0 is show, and a value of -1 gets the current value. The 2D shape (base) of an object is visible in the Planar view window. Note that view windows also have an attribute named hideallbases that controls the display of 2D shapes (bases) for ALL objects in their view.

## Example

```flexscript
switch_hidebase(so(),1)
```

