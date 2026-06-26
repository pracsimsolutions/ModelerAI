---
id: drawrectangle
name: "drawrectangle"
kind: command
module: ""
signature: "drawrectangle(num x, num y, num z, num length, num width, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draw a 3D rectangle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawrectangle

```flexscript
drawrectangle(num x, num y, num z, num length, num width, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])
```

Draw a 3D rectangle

Draws a flat planar rectangle with a user-defined location, length, width, rotation, and color. The drawquad() command must be used to draw a rectangle that is not parallel with the x, y or z plane.

An optional opacity parameter will change the rectangle from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be repeated textureXrepeat and textureYrepeat times across the face of the rectangular plane as texturing.

The location, size and rotation of the rectangle are by default relative to the object it is drawn on. The optional parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawrectangle(0,0,0, 2,3, 0,0,0, 175,175,175, .6, 1, 2,3);
```

