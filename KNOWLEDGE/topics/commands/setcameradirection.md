---
id: setcameradirection
name: "setcameradirection"
kind: command
module: ""
signature: "setcameradirection(obj view, num x, num y, num z, num rx, num ry, num rz)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Sets the viewpoint direction of the 3D view"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setcameradirection

```flexscript
setcameradirection(obj view, num x, num y, num z, num rx, num ry, num rz)
```

Sets the viewpoint direction of the 3D view

Set the viewpoint direction of an orthographic or perspective view. (x,y,z) is the location of the focus point of the camera. (rx, ry, rz) is the rotation of the camera. The associated view attribute names are: viewpointx, viewpointy, viewpointradius, viewpointrx, viewpointry, viewpointrz respectively.

## Example

```flexscript
setcameradirection(sv(), 0, 0, 0, -40, 0, 25);
```

