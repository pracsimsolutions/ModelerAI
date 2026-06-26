---
id: unloadimage
name: "unloadimage"
kind: command
module: ""
signature: "unloadimage(int textureid) -> num"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "FlexScript command: unloadimage."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# unloadimage

```flexscript
unloadimage(int textureid) -> num
```

For developer use. Unload an image from the Media List by index.

## Example

```flexscript
treenode view = node("VIEW:/1>Documents/Dashboard/1+/~/GraphPanel/1/1");
repaintview(view);
viewtofile(view, modeldir() + "test.png");
unloadimage(gettextureindex(modeldir() + "test.png"));
int newindex = loadimage(modeldir() + "test.png", modeldir() + "test.png");
setobjecttextureindex(Model.find("Plane1"), newindex);

This example renders a dashboard chart to an image file, unloads that image from the media list, reloads the image back into the media list, and sets the texture index of a plane to that image.
```

