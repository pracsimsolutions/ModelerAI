---
id: fglpopmatrix
name: "fglPopMatrix"
kind: command
module: ""
signature: "fglPopMatrix()"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Pops the current OpenGL drawing context from the matrix stack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglPopMatrix

```flexscript
fglPopMatrix()
```

Pops the current OpenGL drawing context from the matrix stack

Pops the current OpenGL drawing context from the matrix stack, or in other words returns to the space on the top of the stack. The OpenGL stack stores the space matrix in a LIFO fashion. You can refer to standard OpenGL documentation for more information if needed.

## Example

```flexscript
fglPopMatrix();
```

