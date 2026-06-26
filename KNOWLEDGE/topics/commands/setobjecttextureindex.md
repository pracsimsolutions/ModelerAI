---
id: setobjecttextureindex
name: "setobjecttextureindex"
kind: command
module: ""
signature: "setobjecttextureindex(obj object, num index)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: "the"
description: "Sets the index number of the image assigned as a texture for the object's 3D shape"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setobjecttextureindex

```flexscript
setobjecttextureindex(obj object, num index)
```

Sets the index number of the image assigned as a texture for the object's 3D shape

Sets the index number of the bmp or jpeg image to be assigned as the texture for the object's 3D shape. The index number must be a valid index number of a currently imported bmp or jpeg image. To view currently imported images, or to import another bmp or jpeg image, use the Import Media option in the ToolBox. Note that this command does not change the object's imageobject attribute so the original texture will return when the model is reset.

## Example

```flexscript
setobjecttextureindex(item,gettextureindex("bitmaps\\flowitempicture.bmp"));
```

