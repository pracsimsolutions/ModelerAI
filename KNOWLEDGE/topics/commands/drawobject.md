---
id: drawobject
name: "drawobject"
kind: command
module: ""
signature: "drawobject(obj view, num shapeindex, num textureindex[, num animation, treenode shapedata])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws a 3D shape"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawobject

```flexscript
drawobject(obj view, num shapeindex, num textureindex[, num animation, treenode shapedata])
```

Draws a 3D shape

Draws a 3d object with the specified shape and texture indices.

## Example

```flexscript
drawtomodelscale(current); drawobject(view,getshapeindex("fs3d//texturebox.wrl"),gettextureindex("flexsim.bmp"));

Set the drawing scale to that of the model, then draw the texturebox.wrl shape using flexsim.bmp as a texture.
```

