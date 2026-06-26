---
id: textureaxis_t
name: "textureaxis_t"
kind: command
module: ""
signature: "textureaxis_t(num x, num y, num z, num a)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: textureaxis_t."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# textureaxis_t

```flexscript
textureaxis_t(num x, num y, num z, num a)
```

Once we have a texture bound in an OpenGL call sequence, we can specify the texture coordinates. OpenGL uses the concept of texture coordinates for mapping. This function is used to set the texture T axis. Once set, the texture axes will be used to render any polygons drawn subsequently.

## Example

```flexscript
textureaxis_t(0,0,1,0)
```

