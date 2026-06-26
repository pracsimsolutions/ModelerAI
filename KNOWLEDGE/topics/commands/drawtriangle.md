---
id: drawtriangle
name: "drawtriangle"
kind: command
module: ""
signature: "drawtriangle(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num x3, num y3, num z3, num red {0-1}, num green {0-1}, num blue {0-1})"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a planar triangle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawtriangle

```flexscript
drawtriangle(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num x3, num y3, num z3, num red {0-1}, num green {0-1}, num blue {0-1})
```

Draws a planar triangle

This command draws a planar triangle with vertices defined by xloc1, yloc1, zloc1, xloc2, yloc2, zloc2, xloc3, yloc3, and zloc3. The color is defined by the red, green, blue color components ranging from 0 to 1.

## Example

```flexscript
drawtriangle(view, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0);
This example draws a triangle with vertices defined by the following three points: (0,0,0), (1,0,0), and (0,1,0). The triangle will be red in color.
```

