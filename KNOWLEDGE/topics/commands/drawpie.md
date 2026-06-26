---
id: drawpie
name: "drawpie"
kind: command
module: ""
signature: "drawpie(obj view, num x, num y, num z, num x2, num y2, num z2, num startangle, num endangle, num red {0-1}, num green {0-1}, num blue {0-1})"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a flat pie shape"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawpie

```flexscript
drawpie(obj view, num x, num y, num z, num x2, num y2, num z2, num startangle, num endangle, num red {0-1}, num green {0-1}, num blue {0-1})
```

Draws a flat pie shape

Draws a flat pie shape with its origin (center of radius) positioned at the midpoint between [x1,y1,z1] and [x2,y2,z2]. The angles are measured in degrees counterclockwise from the positive x axis. Negative angles are measured clockwise. The pie is filled from the start angle to the endangle counterclockwise. The color is defined by the red, green, blue color components ranging from 0 to 1.

## Example

```flexscript
drawpie(view,3,3,0,0,0,0,45,90,0.5,0,0);
```

