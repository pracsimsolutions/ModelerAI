---
id: drawline
name: "drawline"
kind: command
module: ""
signature: "drawline(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num red {0-1}, num green {0-1}, num blue {0-1})"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a planar line"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawline

```flexscript
drawline(obj view, num x1, num y1, num z1, num x2, num y2, num z2, num red {0-1}, num green {0-1}, num blue {0-1})
```

Draws a planar line

This command draws a planar line with a starting point of xloc1, yloc1, zloc1, and an ending point of xloc2, yloc2, zloc2. The color is defined by the red, green, blue color components ranging from 0 to 1.

## Example

```flexscript
fglDisable(GL_LIGHTING);
drawline(view, 0, 0, 0, 2, 2, 2, 1, 0, 0);
fglEnable(GL_LIGHTING);

This example draws a line from point (0,0,0) to point (2,2,2). The line will be red in color.
```

