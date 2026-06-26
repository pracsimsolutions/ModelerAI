---
id: rackdrawfilledcell
name: "rackdrawfilledcell"
kind: command
module: ""
signature: "rackdrawfilledcell(obj rack, num bay, num level, num x, num y, num z, num sx, num sy, num sz, num red, num green, num blue)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Draws a box in the rack's given bay and level"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackdrawfilledcell

```flexscript
rackdrawfilledcell(obj rack, num bay, num level, num x, num y, num z, num sx, num sy, num sz, num red, num green, num blue)
```

Draws a box in the rack's given bay and level

Draws a box in the rack's given bay and level. x,y, and z are numbers between 0 and 1 representing the percentage into the cell to start drawing the box. sx, sy, and sz are numbers between 0 and 1 representing the size percentage of the box. Red, green and blue specify the color of the box.
To ensure that the boxes are drawn correctly, you should disabled OpenGL textures with fglDisable(GL_TEXTURE_2D) before calling this command in a draw trigger.

## Example

```flexscript
fglDisable(GL_TEXTURE_2D);
for (int bay = 1; bay <= 5; bay++) {
for (int level = 1; level <= 3; level++) {
rackdrawfilledcell(current, bay, level, 0,0,0, 0.5,1,1, 255,0,0);
}
}
fglEnable(GL_TEXTURE_2D);

This example draws red boxes in bays 1-5, levels 1-3, that fill half of each cell's volume (sx = 0.5, sy =1, sz = 1).
```

