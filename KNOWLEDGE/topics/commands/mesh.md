---
id: mesh
name: "mesh"
kind: command
module: ""
signature: "mesh(node meshnode, int pervertexattribs, int flags)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Creates an empty mesh and assigns it to meshnode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# mesh

```flexscript
mesh(node meshnode, int pervertexattribs, int flags)
```

Creates an empty mesh and assigns it to meshnode

Creates an empty mesh and assigns it to meshnode. The pervertexattribs takes a bit-wise ORed combination of the following attributes:
MESH_POSITION (Added by default)
MESH_TEX_COORD2
MESH_NORMAL
MESH_DIFFUSE
MESH_DIFFUSE4
MESH_AMBIENT_AND_DIFFUSE
MESH_AMBIENT_AND_DIFFUSE4

Flags can be any of the following:
MESH_STATIC_DRAW
MESH_DYNAMIC_DRAW
MESH_INDEXED

## Example

```flexscript
mesh(Model.find("mesh"), MESH_NORMAL | MESH_DIFFUSE, MESH_STATIC_DRAW)
```

