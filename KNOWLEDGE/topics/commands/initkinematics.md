---
id: initkinematics
name: "initkinematics"
kind: command
module: ""
signature: "initkinematics(node infonode, obj/num object/x[, num flags/y, num z, num rx, num ry, num rz, num flags, num unused])"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Initializes a kinematic operation"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# initkinematics

```flexscript
initkinematics(node infonode, obj/num object/x[, num flags/y, num z, num rx, num ry, num rz, num flags, num unused])
```

Initializes a kinematic operation

Initialize a kinematic operation. Infonode is a blank node that can hold kinematic information. Object is the object that you want
kinematics to be performed on. flags defines various flags for how you want it to behave. The unused parameter deprecated, i.e. you can define this
through the flags parameter. Refer to kinematics documentation in the user manual for more information.

## Example

```flexscript
initkinematics(current.labels["kinematics"], current)
```

