---
id: angle
name: "angle"
kind: command
module: ""
signature: "angle(num dx, num dy) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Math.atan2"
description: "Deprecated, use Math.atan2()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# angle

> ⚠ **DEPRECATED.** Use `Math.atan2` instead.

```flexscript
angle(num dx, num dy) -> num
```

Deprecated, use Math.atan2()

This command is deprecated. Use Math.atan2() instead.

Returns the angle in degrees, rotated clockwise from the positive x axis, of the vector (dx, dy). Calling angle(dx, dy) is the same as calling -Math.degrees(Math.atan2(dy, dx)).

## Example

```flexscript
double myangle = angle(distx, disty)
```

