---
id: drawrect
name: "drawrect"
kind: command
module: ""
signature: "drawrect(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num red {0-1}, num green {0-1}, num blue {0-1})"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a planar rectangle"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawrect

```flexscript
drawrect(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num red {0-1}, num green {0-1}, num blue {0-1})
```

Draws a planar rectangle

This command draws a planar rectangle with opposite corners defined by x1, y1, z1, and x2, y2, and z2. Both points must lie in a plane parallel to the xy plane.
The color is defined by the red, green, blue color components ranging from 0 to 1.
This command can be called in a planar, 3D, graph, or panel view. See also drawquad()

## Example

```flexscript
drawrect(view, 0,0,.5, 1, 1, .5, 1, 0, 0);
Draws a red rectangle with opposite corners (0,0,.5) and (1,1,.5).
```

