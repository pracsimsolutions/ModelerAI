---
id: fglinfo
name: "fglinfo"
kind: command
module: ""
signature: "fglinfo(num operation, obj view) -> num"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Gets information about a 3D view's draw state"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglinfo

```flexscript
fglinfo(num operation, obj view) -> num
```

Gets information about a 3D view's draw state

This command gives you access to various information about the draw state of a 3d view.
Logic/data that is performed/returned is based on the operation parameter, as follows:

FGL_INFO_SHADERTYPE - returns what part of the draw pass is happening,
such as rendering the shadow map (SHADERTYPE_RENDER_SHADOWS), blurring the shadow map (SHADERTYPE_BLUR),
or rendering the objects to the screen (SHADERTYPE_DEFAULT).

## Example

```flexscript
/*do not draw this object when rendering the shadow map*/
if (fglinfo(FGL_INFO_SHADERTYPE, view) == SHADERTYPE_RENDER_SHADOWS)
return 0;
```

