---
id: meshinfo
name: "meshinfo"
kind: command
module: ""
signature: "meshinfo(node meshnode, int type[,int index, int param]) -> num"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Gets the specified information from a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshinfo

```flexscript
meshinfo(node meshnode, int type[,int index, int param]) -> num
```

Gets the specified information from a mesh

Gets the specified information from the mesh associated with meshnode. Index is the index of the requested vertex (base 0). Type is specified as one of the following for the mesh (index and param ignored):
MESH_PER_VERTEX_ATTRIBS
MESH_PER_MESH_ATTRIBS
MESH_FLAGS
MESH_NR_VERTS
MESH_GET_INDEX - Returns the vertex index for the given index (only valid with MESH_INDEXED flag)

The following are vertex specific types:
MESH_POSITION
MESH_TEX_COORD2
MESH_NORMAL
MESH_DIFFUSE
MESH_DIFFUSE4
MESH_AMBIENT_AND_DIFFUSE
MESH_AMBIENT_AND_DIFFUSE4

If specifying a vertex, the following are valid for param:
MESH_COLOR_RED
MESH_COLOR_GREEN
MESH_COLOR_BLUE
MESH_COLOR_ALPHA
MESH_POS_X
MESH_POS_Y
MESH_POS_Z
MESH_TEX_S
MESH_TEX_T

## Example

```flexscript
double xposition = meshinfo(Model.find("mesh"), MESH_POSITION, 6, MESH_POS_X)
```

