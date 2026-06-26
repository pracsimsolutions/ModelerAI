---
id: spacetranslate
name: "spacetranslate"
kind: command
module: ""
signature: "spacetranslate(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: spacetranslate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# spacetranslate

```flexscript
spacetranslate(num x, num y, num z)
```

Apply translation to the OpenGL space. This corresponds to the following OpenGL code (keep in mind that OpenGL coordinate systems have y pointing in the direction of FlexSim's z axis, and have z pointing in the direction of FlexSim's -y axis):

glTranslatef(x,z,-y);

## Example

```flexscript
spacetranslate(current.size.x, -current.size.y, 0)
This translates to the bottom left corner of the object assuming no translation has previously been applied. Subsequent draw commands will be performed from this new location.
```

