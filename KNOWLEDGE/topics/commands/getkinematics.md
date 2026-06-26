---
id: getkinematics
name: "getkinematics"
kind: command
module: ""
signature: "getkinematics(node infonode, num querytype[, num kinematicindex, num updatetime/traveldist]) -> num"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Gets information from the specified kinematics operation"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getkinematics

```flexscript
getkinematics(node infonode, num querytype[, num kinematicindex, num updatetime/traveldist]) -> num
```

Gets information from the specified kinematics operation

Gets information from the specified kinematics operation. For more information, refer to the help documentation on kinematics.

## Example

```flexscript
getkinematics(current.labels["kinematics"], KINEMATIC_ARRIVALTIME, 1, current.size.x);
```

