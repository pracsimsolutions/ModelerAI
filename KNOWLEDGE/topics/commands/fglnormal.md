---
id: fglnormal
name: "fglNormal"
kind: command
module: ""
signature: "fglNormal(num x, num y, num z)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Sets the normal vector for shading subsequent vertices in OpenGL"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fglNormal

```flexscript
fglNormal(num x, num y, num z)
```

Sets the normal vector for shading subsequent vertices in OpenGL

Sets the normal vector for shading subsequent vertices in OpenGL drawing.
Polygons will be flat shaded unless normals are applied to them.
A normal is a vector that is perpendicular to the polygon and points in the direction the
polygon is facing. Remember polygons with vertices defined in a counter-clockwise winding
points forward per the "right-hand rule". Polygon shading is based on the angle between
the normals and the light source. Normals are applied to each vertex in the same way
colors are, but is usually just defined at the start of each polygon definition. When a
polygon's normals point directly at the light source, they are brightly shaded; and are
darker shaded when the angles are skewed. You can refer to standard OpenGL documentation
for more information if needed.

## Example

```flexscript
fglNormal(1,0,0);
```

