---
id: distfromviewpoint
name: "distfromviewpoint"
kind: command
module: ""
signature: "distfromviewpoint(obj object, obj view) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "How far from the viewpoint object is in the window view"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# distfromviewpoint

```flexscript
distfromviewpoint(obj object, obj view) -> num
```

How far from the viewpoint object is in the window view

Returns how far from the viewpoint the object specified as object is in the view window specified as view. This value is only meaningful if view is a reference to a Perspective window.

## Example

```flexscript
double dist = distfromviewpoint(model().subnodes[2],sv());
This sets dist to the distance from the first object in the model to the viewer's eye.
```

