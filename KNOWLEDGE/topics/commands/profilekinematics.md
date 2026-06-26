---
id: profilekinematics
name: "profilekinematics"
kind: command
module: ""
signature: "profilekinematics(node infonode, num kinematicindex)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Prints information about the specified kinematics operation to the output console"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# profilekinematics

```flexscript
profilekinematics(node infonode, num kinematicindex)
```

Prints information about the specified kinematics operation to the output console

Prints information about the specified kinematics operation to the output console. For more information, refer to the help documentation on kinematics.

## Example

```flexscript
printkinematics(current.labels["kinematics"], 0);
```

