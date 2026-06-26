---
id: fglmatrixmode
name: "fglMatrixMode"
kind: command
module: ""
signature: "fglMatrixMode(num mode)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Specifies which matrix stack is the target for subsequent matrix operations"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglMatrixMode

```flexscript
fglMatrixMode(num mode)
```

Specifies which matrix stack is the target for subsequent matrix operations

Specifies which matrix stack is the target for subsequent matrix operations. A matrix is just a way to specify a whole bunch of n-dimensional transformations in one convenient package. Three values are accepted: GL_MODELVIEW, GL_PROJECTION, and GL_TEXTURE. The initial value is GL_MODELVIEW which is used when moving objects around the scene. You can refer to standard OpenGL documentation for more information if needed.

## Example

```flexscript
fglMatrixMode(GL_MODELVIEW);
```

