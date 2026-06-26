---
id: ntravelto
name: "ntravelto"
kind: command
module: ""
signature: "ntravelto(obj traveler, obj spline, num endx, num endy, num endz, num vmax, num acc, num dec, num startspeed, num endspeed)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "kinematics"
description: "Deprecated, use kinematics instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ntravelto

> ⚠ **DEPRECATED.** Use `kinematics` instead.

```flexscript
ntravelto(obj traveler, obj spline, num endx, num endy, num endz, num vmax, num acc, num dec, num startspeed, num endspeed)
```

Deprecated, use kinematics instead

This command is deprecated. Use kinematics instead.

Causes the object to travel to a given rz and return the simulation time that it will arrive. Similar to the travelto command, but includes a start and end speed definition. Used in the development of the TaskExecuter.

## Example

```flexscript
ntravelto(current, 0, 5, 8, 0, 1, 1, 1, 0, 1)
```

