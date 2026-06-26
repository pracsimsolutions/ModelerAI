---
id: setkinematicsrotoffset
name: "setkinematicsrotoffset"
kind: command
module: ""
signature: "setkinematicsrotoffset(node infonode, num rx, num ry, num rz)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: setkinematicsrotoffset."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setkinematicsrotoffset

```flexscript
setkinematicsrotoffset(node infonode, num rx, num ry, num rz)
```

Sets the initial kinematics rotational offset for a kinematics operation. For more information, refer to the help documentation on kinematics.

## Example

```flexscript
setkinematicsrotoffset(current.labels["kinematics"], 0, 0, 180);
```

