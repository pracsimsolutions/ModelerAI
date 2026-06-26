---
id: drawtext
name: "drawtext"
kind: command
module: ""
signature: "drawtext(obj view, str text, num x, num y, num z [, num width, num height, num thickness, num rx, num ry, num rz, num red {0-1}, num green {0-1}, num blue {0-1} , num opacity {0-1}, num flags])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws text"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawtext

```flexscript
drawtext(obj view, str text, num x, num y, num z [, num width, num height, num thickness, num rx, num ry, num rz, num red {0-1}, num green {0-1}, num blue {0-1} , num opacity {0-1}, num flags])
```

Draws text

This command draws text at a location defined by x, y, and z.
The width parameter is not used. The text width and height is determined by the height parameter.
The rotation is defined by rx, ry, and rz. The color is defined by the red, green, blue color components ranging from 0 to 1. You can
define the flags parameter as follows: 0x1 = bold, 0x2 = italic, 0x4 = underline. The flags parameter only applies to non-3d views
The opacity if used should be a number ranging between 0 (full transparency) to 1 (opaque).
Call drawfont() command to change font type before calling drawtext() if desired.
This command can be used in a planar, 3D, graph or panel view.

## Example

```flexscript
drawtext(view, "Hello", 0,0,0, 0,.3,.1, 90,0,0, 1,0,0, .8);
This example draws "Hello" at location (0,0,0) with size 0.3, thickness 0.1, rotation (90,0,0), an rgb color (1,0,0), and opacity of 0.8.
```

