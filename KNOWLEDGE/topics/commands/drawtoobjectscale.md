---
id: drawtoobjectscale
name: "drawtoobjectscale"
kind: command
module: ""
signature: "drawtoobjectscale(obj object)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Scale subsequent draw commands to the size of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawtoobjectscale

```flexscript
drawtoobjectscale(obj object)
```

Scale subsequent draw commands to the size of the object

This command causes all subsequent draw commands to be scaled according to the size of the object. This is equivalent to the following OpenGL function:

glScalef(object.location.x,object.location.y,object.location.z);

By default, the scale is originally set to that of the object.

## Example

```flexscript
drawtoobjectscale(current);
```

