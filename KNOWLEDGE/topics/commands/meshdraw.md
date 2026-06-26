---
id: meshdraw
name: "meshdraw"
kind: command
module: ""
signature: "meshdraw(node meshnode, int drawmode, int offset, int count [, int stride])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshdraw

```flexscript
meshdraw(node meshnode, int drawmode, int offset, int count [, int stride])
```

Draws a mesh

Draws the mesh associated with meshnode. The value for drawmode can be any of the following:
GL_POINTS
GL_LINES
GL_LINE_LOOP
GL_TRIANGLES
GL_TRIANGLE_STRIP
GL_TRIANGLE_FAN

Offset specifies which vertex index to begin at. Count specifies how many vertexes to set the drawmode.
Stride specifies how many vertex indexes to skip between.

For an indexed meshed, stride is ignored.

## Example

```flexscript
meshdraw(node("/mesh",model()), GLTRIANGLES, 0, 3, 0)
```

