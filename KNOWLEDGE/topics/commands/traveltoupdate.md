---
id: traveltoupdate
name: "traveltoupdate"
kind: command
module: ""
signature: "traveltoupdate(obj object [, obj spline])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "kinematics"
description: "Deprecated, use kinematics"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# traveltoupdate

> ⚠ **DEPRECATED.** Use `kinematics` instead.

```flexscript
traveltoupdate(obj object [, obj spline])
```

Deprecated, use kinematics

This command is deprecated. Use kinematics instead.

Update the object's position based on it's current travel parameters. Explicitly set an object's position based on the time and travel path parameters as calculated from the travelto() command on object, using spline as the path geometry if desired.

## Example

```flexscript
traveltoupdate(current)
Updates the current object's position based on the travel parameters set earlier by the traveto() command. This command is usually executed from the Custom Draw Code field.
```

