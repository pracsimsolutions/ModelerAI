---
id: fglrotate
name: "fglRotate"
kind: command
module: ""
signature: "fglRotate(num degrees, num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Rotates the current OpenGL drawing context by degrees around axis x,y,z"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglRotate

```flexscript
fglRotate(num degrees, num x, num y, num z)
```

Rotates the current OpenGL drawing context by degrees around axis x,y,z

Rotates the current OpenGL drawing context by degrees around an axis defined by the vertex (x,y,z). Anything drawn after
a rotation is based on the new rotation of the axes. See glVertex3d for definition of the OpenGL xyz coordinate system. You can refer
to standard OpenGL documentation for more information if needed.

## Example

```flexscript
fglRotate(90,1,0,0);
```

