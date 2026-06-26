---
id: drawbox
name: "drawbox"
kind: command
module: ""
signature: "drawbox(num x, num y, num z, num sx, num sy, num sz, num glbegin)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a simple box"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawbox

```flexscript
drawbox(num x, num y, num z, num sx, num sy, num sz, num glbegin)
```

Draws a simple box

Draws a simple box. If glbegin is 1, then it will call glBegin before drawing.
This command is a shortcut way to draw an OpenGL type cube that allows more control over lighting,
coloring, and texturing than the drawcube() command.

## Example

```flexscript
drawbox(5,0,0, 2,1,3, 1);
```

