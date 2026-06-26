---
id: travelto
name: "travelto"
kind: command
module: ""
signature: "travelto(obj object, obj spline, num x, num y, num z, num vmax, num acc, num dec)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "kinematics"
description: "Deprecated, use kinematics"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# travelto

> ⚠ **DEPRECATED.** Use `kinematics` instead.

```flexscript
travelto(obj object, obj spline, num x, num y, num z, num vmax, num acc, num dec)
```

Deprecated, use kinematics

This command is deprecated. Use kinematics instead.

Calculate current travel parameters for the object. A travel path will be calculated. Once set, the object's position will be calculated based on the travel path as defined by this command. This function returns the time of travel end. This function can be used in conjunction with a spline to create curved travel paths.

## Example

```flexscript
travelto(traveler,NULL,100,100,0,5,1,6)
```

