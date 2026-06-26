---
id: scalekinematicstotime
name: "scalekinematicstotime"
kind: command
module: ""
signature: "scalekinematicstotime(node kinematics, int kinematicnr, double scaletotime) -> num"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Scales the kinematics to a defined time"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# scalekinematicstotime

```flexscript
scalekinematicstotime(node kinematics, int kinematicnr, double scaletotime) -> num
```

Scales the kinematics to a defined time

Scales the kinematic(s) to a defined time. This can be used to preserve the characteristics of the kinematics, i.e. accelerate->peak speed->decelerate, but have the total time it takes to perform the kinematics be defined by some other mechanism such as a distribution. If kinematicnr is 0, then the entire set of kinematics will be scaled to the given time. If kinematicnr is greater than 0, then only that kinematic will be scaled. The return value is the new end time of the kinematic(s).

## Example

```flexscript
scalekinematicstotime(kinematics, 0, exponential(0, 10));
```

