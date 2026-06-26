---
id: spacescale
name: "spacescale"
kind: command
module: ""
signature: "spacescale(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: spacescale."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# spacescale

```flexscript
spacescale(num x, num y, num z)
```

Apply scale transform to the OpenGL space. This corresponds to the following OpenGL code (keep in mind that OpenGL coordinate systems have y pointing in the direction of FlexSim's z axis, and have z pointing in the direction of FlexSim's -y axis):

glScalef(x,z,y);

## Example

```flexscript
spacescale(2, 1, 1)
This scales the current coordinate space by 2 times in the FlexSim x direction. Subsequent draw commands will be performed from this new scaling.
```

