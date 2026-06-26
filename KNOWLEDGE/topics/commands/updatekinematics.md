---
id: updatekinematics
name: "updatekinematics"
kind: command
module: ""
signature: "updatekinematics(node infonode, obj object [, num updatetime])"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Updates the location of the object based on its kinematics"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# updatekinematics

```flexscript
updatekinematics(node infonode, obj object [, num updatetime])
```

Updates the location of the object based on its kinematics

Updates the location of the object based on the kinematics operation and the current time. For more information, refer to the help documentation on kinematics.

## Example

```flexscript
updatekinematics(current.labels["kinematics"], current, time());
```

