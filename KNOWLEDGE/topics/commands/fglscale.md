---
id: fglscale
name: "fglScale"
kind: command
module: ""
signature: "fglScale(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Scales the current OpenGL drawing context by x,y,z"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglScale

```flexscript
fglScale(num x, num y, num z)
```

Scales the current OpenGL drawing context by x,y,z

Scales the current OpenGL drawing context by the given scale parameters. See glVertex3d for definition of the OpenGL xyz
coordinate system. Anything drawn after this command is based on the change in scale of the axes. You can refer to standard OpenGL
documentation for more information if needed.

## Example

```flexscript
fglScale(2,4,1);
```

