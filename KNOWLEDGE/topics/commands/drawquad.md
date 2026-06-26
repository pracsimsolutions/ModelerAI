---
id: drawquad
name: "drawquad"
kind: command
module: ""
signature: "drawquad(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num x3, num y3, num z3, num x4, num y4, num z4, num red {0-1}, num green {0-1}, num blue {0-1})"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a planar quadrilateral"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawquad

```flexscript
drawquad(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num x3, num y3, num z3, num x4, num y4, num z4, num red {0-1}, num green {0-1}, num blue {0-1})
```

Draws a planar quadrilateral

This command draws a planar quadrilateral with vertices defined by x1, y1, z1, x2, y2, z2, x3, y3, z3, x4, y4, and z4. The color is defined by the red, green, blue color components ranging from 0 to 1. The resultant shape and lighting is dependent on the order or sequence in which the vertices are defined. If vertices are all in one plane, consider using drawrectangle() command.

## Example

```flexscript
drawquad(view, 0, 0, 1, 0, 0, 0, 2, 2, 2, 4, 5, 6, 1, 0, 0);
This example draws a quadrilateral with vertices defined by the following four points: (0,0,0), (1,0,0), (2,2,2), and (4,5,6). The quadrilateral will be red in color.
```

