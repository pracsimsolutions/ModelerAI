---
id: deactivatekinematics
name: "deactivatekinematics"
kind: command
module: ""
signature: "deactivatekinematics(node kinematicsnode)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "Causes kinematics to do nothing when updatekinematics() is called"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# deactivatekinematics

```flexscript
deactivatekinematics(node kinematicsnode)
```

Causes kinematics to do nothing when updatekinematics() is called

Causes kinematics to do nothing when updatekinematics() is subsequently called instead of updating the object. Call this in the reset trigger of an object to free the object to move it around in the ortho view.

## Example

```flexscript
deactivatekinematics(current.labels["kinLabel"]);
```

