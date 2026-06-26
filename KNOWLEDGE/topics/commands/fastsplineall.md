---
id: fastsplineall
name: "fastsplineall"
kind: command
module: ""
signature: "fastsplineall(obj starttree {model}, num res)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: fastsplineall."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fastsplineall

```flexscript
fastsplineall(obj starttree {model}, num res)
```

For developer use. Cache all splines at once. The geometry for all splines in the model are evaluated and "locked" in order to accelerate the rendering and evaluation of spline geometry. This command will set the values to be returned by subsequent calls to the splinex(), spliney(), splinez(), splinery(), splinery(), splinery(), splinelength(), splineposobject() commands whenever calling fastspline().

## Example

```flexscript
fastsplineall(model(),20)
```

