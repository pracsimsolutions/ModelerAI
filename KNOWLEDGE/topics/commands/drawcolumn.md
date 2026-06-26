---
id: drawcolumn
name: "drawcolumn"
kind: command
module: ""
signature: "drawcolumn(num x, num y, num z, num totalsides, num baseradius, num topradius, num height, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a 3D column"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawcolumn

```flexscript
drawcolumn(num x, num y, num z, num totalsides, num baseradius, num topradius, num height, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])
```

Draws a 3D column

A very versatile command that can draw anything with multiple sides and a height such as a square, a pentagon, a pyramid, and even a cylinder (if the total number of sides is large).
Draws a column with its origin (center of base) positioned at x, y, z. The column is then defined by the number of sides, a base radius, a top radius, and a height. The column can be rotated in degrees about each axis with rx, ry, rz. The color is defined by the red, green, blue color components ranging from 0 to 255.

An optional opacity parameter will change the column from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be repeated textureXrepeat and textureYrepeat times on each face of the column.

The location, size and rotation of the column are by default relative to the object it is drawn on. The parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawcolumn(0,0,0, 5 ,4,2, 6 ,0,0,0, 175,175,175, .6, 3, 5,1);
```

