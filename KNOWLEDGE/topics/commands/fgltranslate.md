---
id: fgltranslate
name: "fglTranslate"
kind: command
module: ""
signature: "fglTranslate(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Translates the current OpenGL drawing context by x,y,z"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglTranslate

```flexscript
fglTranslate(num x, num y, num z)
```

Translates the current OpenGL drawing context by x,y,z

Translates the current OpenGL drawing context by the given translation parameters. See glVertex3d for definition of the
OpenGL xyz coordinate system. Anything drawn after a translation is based on the new location of the axes. You can refer to standard
OpenGL documentation for more information if needed.

## Example

```flexscript
fglTranslate(15, 4, 0);
```

