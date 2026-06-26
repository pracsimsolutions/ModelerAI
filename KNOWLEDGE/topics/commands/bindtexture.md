---
id: bindtexture
name: "bindtexture"
kind: command
module: ""
signature: "bindtexture(num textureid)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: bindtexture."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bindtexture

```flexscript
bindtexture(num textureid)
```

This function is used in an OpenGL call sequence. When OpenGL commands are being used to draw a scene, this command will bind the texture loaded into FlexSim with the index textureid. Once bound, a texture will be used to render any polygons drawn subsequently. Texture mapping is the application (or mapping) of an image (or texture) to the surface of a polygon or other graphics primitive.

## Example

```flexscript
bindtexture(10)

Use loaded texture with index number 10 for rendering any subsequent polygons
```

