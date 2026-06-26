---
id: glisenabled
name: "glIsEnabled"
kind: command
module: ""
signature: "glIsEnabled(num setting)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Returns 1 if the OpenGL setting is enabled, 0 otherwise"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# glIsEnabled

```flexscript
glIsEnabled(num setting)
```

Returns 1 if the OpenGL setting is enabled, 0 otherwise

Returns 1 if the OpenGL setting is enabled, 0 otherwise. See fglEnable() for list of draw settings.
Note that this is an extremely slow operation and is discouraged.
You can refer to standard OpenGL documentation for more information if needed.

## Example

```flexscript
glIsEnabled(GL_LIGHTING)
```

