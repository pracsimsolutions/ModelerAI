---
id: fastspline
name: "fastspline"
kind: command
module: ""
signature: "fastspline(obj spline, num perc [0-1])"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: "the"
description: "FlexScript command: fastspline."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fastspline

```flexscript
fastspline(obj spline, num perc [0-1])
```

For developer use. Get a spline mapping from a cached spline. This command will set the values to be returned by subsequent calls to the splinex(), spliney(), splinez(), splinerx(), splinery(), splinerz(), splinelength(), splineposobject() commands. It will use the precalculated values for the spline object given in spline. The parameter perc is the position along the spline of interest. The precalculated values are evaluated when the fastsplineall() or fastsplineupdate() commands are used.

## Example

```flexscript
fastspline(splinestartnode,1)
This activates the spline pointed to splinestartnode at its furthest point (100 percent along the spline).
```

