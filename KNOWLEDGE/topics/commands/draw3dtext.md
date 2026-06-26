---
id: draw3dtext
name: "draw3dtext"
kind: command
module: ""
signature: "draw3dtext(str text[, num asBitmap])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws text in a 3d view"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# draw3dtext

```flexscript
draw3dtext(str text[, num asBitmap])
```

Draws text in a 3d view

This command is similar to drawtext(), but can only be used in a 3D view, and its parameter set is stripped down to
provide direct access to the text rendering procedure. Whereas drawtext() does some automatic scaling based on the current object,
as well as some OpenGL state changes, etc., draw3dtext() calls the text drawing procedure directly, so any positioning, scaling,
color settings, etc., should be done with OpenGL commands, i.e. glTranslated(), glScaled(), glColor3d(), etc.

## Example

```flexscript
draw3dtext(current.name);
```

