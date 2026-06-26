---
id: drawsphere
name: "drawsphere"
kind: command
module: ""
signature: "drawsphere(num x, num y, num z, num radius, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draw a 3D sphere"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawsphere

```flexscript
drawsphere(num x, num y, num z, num radius, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])
```

Draw a 3D sphere

Draws a hollow sphere with its origin (center of sphere) positioned at x, y, z. The sphere's dimensions are defined by a single radius. The color is defined by the red, green, blue color components ranging from 0 to 255.

An optional opacity parameter will change the sphere from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be wrapped over the surface of the sphere as a texture.

The location, size and rotation of the sphere are by default relative to the object it is drawn on. The optional parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawsphere(0,0,0,5,175,175,175,1,3);
```

