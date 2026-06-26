---
id: meshaddindex
name: "meshaddindex"
kind: command
module: ""
signature: "meshaddindex(node meshnode, int index) -> num"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Adds an indexed vertex to a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshaddindex

```flexscript
meshaddindex(node meshnode, int index) -> num
```

Adds an indexed vertex to a mesh

Adds an indexed vertex to the mesh associated with meshnode.
This command is ignored if the mesh does not have a MESH_INDEXED flag.

## Example

```flexscript
int newindex = meshaddindex(node("/mesh",model()), 4)
```

