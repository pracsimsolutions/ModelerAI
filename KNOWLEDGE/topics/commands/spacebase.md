---
id: spacebase
name: "spacebase"
kind: command
module: ""
signature: "spacebase([num view/object])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: spacebase."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# spacebase

```flexscript
spacebase([num view/object])
```

Set the OpenGL matrix to the scaled space of the current object/view. If the parameter is 0, then the object's base space matrix will be loaded. If the parameter is 1, then the view's space will be loaded, meaning any subsequent draw commands will face the viewer directly and not change scale. This command can only be used in OnDraw event and Custom Draw Code trigger. See drawtoobjectscale() when scaling the OpenGL space by the object's size.

## Example

```flexscript
spacebase(0);
```

