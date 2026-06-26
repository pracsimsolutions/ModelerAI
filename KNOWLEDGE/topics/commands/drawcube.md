---
id: drawcube
name: "drawcube"
kind: command
module: ""
signature: "drawcube(num x, num y, num z, num sx, num sy, num sz, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a 3D cube"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawcube

```flexscript
drawcube(num x, num y, num z, num sx, num sy, num sz, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex, num textureXrepeat, num textureYrepeat])
```

Draws a 3D cube

Draws a cube with its origin (bottom left corner) positioned at x, y, z. Its length, width, and height is defined by sx, sy, and sz respectively. It can be rotated in degrees about the x, y, and z axis by rx, ry, and rz. The color is defined by the red, green, blue color components ranging from 0 to 255.

An optional opacity parameter will change the cube from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be repeated textureXrepeat and textureYrepeat times on each face of the cube.

The location, size and rotation of the cube are by default relative to the object it is drawn on. The optional parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawcube(0,0,0, 5,3,2, 0,0,0, 175,175,175, .6, 3, 1,1);
```

