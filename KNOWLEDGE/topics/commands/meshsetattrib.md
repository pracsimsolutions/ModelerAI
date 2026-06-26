---
id: meshsetattrib
name: "meshsetattrib"
kind: command
module: ""
signature: "meshsetattrib(node meshnode, int attribid, double param1 [, double param2, double param3, double param4])"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Adds/sets the value of attribid to the vertindex of a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshsetattrib

```flexscript
meshsetattrib(node meshnode, int attribid, double param1 [, double param2, double param3, double param4])
```

Adds/sets the value of attribid to the vertindex of a mesh

Sets the value of attribid to be applied to the entire mesh associated with meshnode. The following attributes can be used as the attribid:
MESH_NORMAL
MESH_AMBIENT
MESH_DIFFUSE
MESH_DIFFUSE4
MESH_AMBIENT_AND_DIFFUSE
MESH_AMBIENT_AND_DIFFUSE4
MESH_SPECULAR
MESH_SHININESS
MESH_EMISSIVE

## Example

```flexscript
meshsetattrib(Model.find("mesh"), 1, MESH_DIFFUSE4, 1, 0, 0, 1)
```

