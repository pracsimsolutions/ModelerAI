---
id: textureaxis_s
name: "textureaxis_s"
kind: command
module: ""
signature: "textureaxis_s(num x, num y, num z, num a)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: textureaxis_s."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# textureaxis_s

```flexscript
textureaxis_s(num x, num y, num z, num a)
```

Once we have a texture bound in an OpenGL call sequence, we can specify the texture coordinates. OpenGL uses the concept of texture coordinates for mapping. This function is used to set the texture S axis. Once set, the texture axes will be used to render any polygons drawn subsequently.

## Example

```flexscript
textureaxis_s(1,0,0,0)
```

