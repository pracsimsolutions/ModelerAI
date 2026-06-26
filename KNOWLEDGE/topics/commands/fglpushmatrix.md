---
id: fglpushmatrix
name: "fglPushMatrix"
kind: command
module: ""
signature: "fglPushMatrix()"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: "glPushMatrix"
description: "Pushes the current OpenGL drawing context onto the matrix stack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglPushMatrix

```flexscript
fglPushMatrix()
```

Pushes the current OpenGL drawing context onto the matrix stack

Pushes the current OpenGL drawing context onto the matrix stack which can later be retrieved with glPopMatrix(). If you are drawing more than one object and you don't want the glTranslated, glRotated or glScaled to affect the next objects, you should use glPushMatrix and glPopMatrix. The OpenGL stack stores the space matrices in a LIFO fashion. You can refer to standard OpenGL documentation for more information if needed.

## Example

```flexscript
fglPushMatrix();
```

