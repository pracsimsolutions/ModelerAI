---
id: pickcolornode
name: "pickcolornode"
kind: command
module: ""
signature: "pickcolornode(obj colornode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: pickcolornode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickcolornode

```flexscript
pickcolornode(obj colornode)
```

For developer use. Open a color picker window to set the rgb color values of the color attribute node of an object.

## Example

```flexscript
pickcolornode(color(current));
Opens a color picker window and then sets the color of the current object to the chosen color.
```

