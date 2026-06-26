---
id: spline
name: "spline"
kind: command
module: ""
signature: "spline(obj spline, num perc)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: spline."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# spline

```flexscript
spline(obj spline, num perc)
```

For developer use. Calculate spline mapping and set the current spline parameters using the spline object spline. This command will set the values to be returned by subsequent calls to the splinex(), spliney(), splinez(), splinerx(), splinery(), splinerz(), splinelength(), splineposobject() commands. It will not set the precalculated values for the spline object given in spline, as in fastspline().The parameter perc is the position along the spline of interest. This command does not lock precalculated values as when the fastsplineall() or fastsplineupdate() commands are used. A spline can be defined as any chain of objects that are connected from output ports to input ports. Pass the first object in the output port to input port chain as parameter 1.

## Example

```flexscript
spline(splinestartobject,0.6)
```

