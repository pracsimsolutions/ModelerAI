---
id: drawtomodelscale
name: "drawtomodelscale"
kind: command
module: ""
signature: "drawtomodelscale(obj object)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Gets the current drawing scale of the model"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawtomodelscale

```flexscript
drawtomodelscale(obj object)
```

Gets the current drawing scale of the model

Returns the current drawing scale to that of the model after a previous call of drawtoobjectscale(). This is equivalent to the following OpenGL function:

glScalef(1/object.location.x,1/object.location.y,1/object.location.z);

## Example

```flexscript
drawtomodelscale(current);
```

