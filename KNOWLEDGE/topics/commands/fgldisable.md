---
id: fgldisable
name: "fglDisable"
kind: command
module: ""
signature: "fglDisable(num setting)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Disables an OpenGL draw setting"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglDisable

```flexscript
fglDisable(num setting)
```

Disables an OpenGL draw setting

Disables an OpenGL draw setting. See glEnable() for list of draw settings.
You can refer to standard OpenGL documentation for more information if needed.

## Example

```flexscript
fglDisable(GL_LIGHTING);
```

