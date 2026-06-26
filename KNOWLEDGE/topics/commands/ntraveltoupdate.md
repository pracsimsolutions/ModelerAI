---
id: ntraveltoupdate
name: "ntraveltoupdate"
kind: command
module: ""
signature: "ntraveltoupdate(obj object)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "kinematics"
description: "Deprecated, use kinematics instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ntraveltoupdate

> ⚠ **DEPRECATED.** Use `kinematics` instead.

```flexscript
ntraveltoupdate(obj object)
```

Deprecated, use kinematics instead

This command is deprecated. Use kinematics instead.

Updates the rz of an object after a ntravelto() command was called on it. Used in the OnPreDraw of the TaskExecuter to update it's position.

## Example

```flexscript
ntraveltoupdate(current)
```

