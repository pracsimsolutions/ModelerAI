---
id: drawspheres
name: "drawspheres"
kind: command
module: ""
signature: "drawspheres(obj object, obj view)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: drawspheres."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawspheres

```flexscript
drawspheres(obj object, obj view)
```

For developer use. Draws the collision spheres assigned to the specified object. TaskExecuters allow users to add collision spheres and draw collision spheres through the object's Parameters window, but commands are needed to add and draw collision spheres to other objects when needed

## Example

```flexscript
drawspheres(current,view);
```

