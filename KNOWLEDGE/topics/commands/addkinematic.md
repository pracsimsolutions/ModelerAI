---
id: addkinematic
name: "addkinematic"
kind: command
module: ""
signature: "addkinematic(node infonode, num x, num y, num z, num targetspeed, num acc, num dec, num startspeed, num endspeed, num starttime, num type ) -> num"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Adds one kinematic operation to a set of kinematics"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addkinematic

```flexscript
addkinematic(node infonode, num x, num y, num z, num targetspeed, num acc, num dec, num startspeed, num endspeed, num starttime, num type ) -> num
```

Adds one kinematic operation to a set of kinematics

Adds one kinematic operation to a set of kinematics. For more information, refer to the user manual documentation on kinematics.

## Example

```flexscript
addkinematic(current.labels["kinematics"], 1, 0, 0, 5,0,0, 0,0, time(), KINEMATIC_TRAVEL);
```

