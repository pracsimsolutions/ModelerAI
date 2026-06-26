---
id: meshsetvertexattrib
name: "meshsetvertexattrib"
kind: command
module: ""
signature: "meshsetvertexattrib(node meshnode, int vertindex, int attribid, double param1 [, double param2, double param3, double param4])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Sets the value of attribid at the vertindex of a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshsetvertexattrib

```flexscript
meshsetvertexattrib(node meshnode, int vertindex, int attribid, double param1 [, double param2, double param3, double param4])
```

Sets the value of attribid at the vertindex of a mesh

Sets the value of attribid at the vertindex of the mesh associated with meshnode. The following attributes can be used as the attribid:
MESH_POSITION
MESH_TEX_COORD2
MESH_NORMAL
MESH_DIFFUSE
MESH_DIFFUSE4
MESH_AMBIENT_AND_DIFFUSE
MESH_AMBIENT_AND_DIFFUSE4
Note: The attribid must be defined previously when creating the mesh.

## Example

```flexscript
mesh(Model.find("mesh"), MESH_NORMAL | MESH_DIFFUSE4, MESH_STATIC_DRAW)
meshsetvertexattrib(Model.find("mesh"), 1, MESH_DIFFUSE4, 1, 0, 0, 1)
```

