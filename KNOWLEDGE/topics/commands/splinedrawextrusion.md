---
id: splinedrawextrusion
name: "splinedrawextrusion"
kind: command
module: ""
signature: "splinedrawextrusion(obj object, num step, num wide, num high, num top, num side)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: splinedrawextrusion."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# splinedrawextrusion

```flexscript
splinedrawextrusion(obj object, num step, num wide, num high, num top, num side)
```

For developer use. Draw a spline based on the current spline geometry as calculate by the spline() with a rectangular extrusion. Using parameters of spline object.

## Example

```flexscript
splinedrawextrusion(current,20,1,2,1,1)
```

