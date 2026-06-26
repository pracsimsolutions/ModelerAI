---
id: drawdisk
name: "drawdisk"
kind: command
module: ""
signature: "drawdisk(num x, num y, num z, num innerradius, num outerradius, num startangle, num sweepangle, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draw a 3D disk"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawdisk

```flexscript
drawdisk(num x, num y, num z, num innerradius, num outerradius, num startangle, num sweepangle, num rx, num ry, num rz, num red {0-255}, num green {0-255}, num blue {0-255} [, num opacity {0-1}, num textureindex])
```

Draw a 3D disk

Draws a flat planar disk with its origin (center of radius) positioned at x, y, z. The disk's dimensions are defined by an inner radius, an outer radius, a start angle, and a sweep angle. A sweep angle of 360 makes a complete circle. An innerradius greater than 0 will create a disk with a hole in the middle of it. The disk can be rotated in degrees about the x, y, and z axis by rx, ry, and rz. The color is defined by the red, green, blue color components ranging from 0 to 255.

An optional opacity parameter will change the disk from clear (0) to opaque (1). If a valid textureindex number is entered, then the associated image will be used as texturing across the face of the disk.

The location, size and rotation of the disk are by default relative to the object it is drawn on. The optional parameters shown in brackets [ ] must ALL be used or ALL not used. The textureindex is a numeric value that can be learned using the gettextureindex() command or through the Tools | Media Files menu.

## Example

```flexscript
drawdisk(0,0,0, .5,2, 90,270, 0,0,0, 175,175,175, 1,3);
```

