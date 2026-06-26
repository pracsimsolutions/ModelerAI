---
id: drawcylinder
name: "drawcylinder"
kind: command
module: ""
signature: "drawcylinder(num x, num y, num z, num baseradius, num topradius, num height, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: "the"
description: "Draws a 3D clyinder"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawcylinder

```flexscript
drawcylinder(num x, num y, num z, num baseradius, num topradius, num height, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])
```

Draws a 3D clyinder

This command draws a hollow cylinder with its origin (center of base) positioned at x, y, z. The cylinder's dimensions are defined by a base radius, a top radius and a height. Use the drawdisk() command to cap off the ends of the cylinder. The cylinder can be rotated in degrees about the x, y, and z axis by rx, ry, and rz. The color is defined by the red, green, blue color components ranging from 0 to 255.

An optional opacity parameter will change the cylinder from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be used as texturing around the outside circumference of the cylinder.

The location, size and rotation of the cylinder are by default relative to the object it is drawn on. The optional parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawcylinder(0,0,0, 1,1, 5, 0,0,0, 175,175,175, .6,3);
```

