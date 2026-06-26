---
id: spacerotate
name: "spacerotate"
kind: command
module: ""
signature: "spacerotate(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "FlexScript command: spacerotate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# spacerotate

```flexscript
spacerotate(num x, num y, num z)
```

Apply rotation to the OpenGL space. This corresponds to the following OpenGL code (keep in mind that OpenGL coordinate systems have y pointing in the direction of FlexSim's z axis, and have z pointing in the direction of FlexSim's -y axis):

glRotatef( z, 0.0f, 1.0f, 0.0f);
glRotatef(-y, 0.0f, 0.0f, 1.0f);
glRotatef( x, 1.0f, 0.0f, 0.0f);

## Example

```flexscript
spacerotate(90, 0, 0)
This rotates the current coordinate space by 90 degrees around the FlexSim x-axis. Subsequent draw commands will be performed from this new rotation.
```

