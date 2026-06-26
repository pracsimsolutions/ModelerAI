---
id: fastsplineupdate
name: "fastsplineupdate"
kind: command
module: ""
signature: "fastsplineupdate(obj spline, num res)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: fastsplineupdate."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fastsplineupdate

```flexscript
fastsplineupdate(obj spline, num res)
```

For developer use. Cache a spline's shape to increase it's speed. The geometry for all splines in the model are re-evaluated and "locked" in order to accelerate the rendering and evaluation of spline geometry. This command will set the values to be returned by subsequent calls to the splinex(), spliney(), splinez(), splinerx(), splinery(), splinerz(), splinelength(), splineposobject() commands whenever calling fastspline().

## Example

```flexscript
fastsplineupdate(so(),20)
```

