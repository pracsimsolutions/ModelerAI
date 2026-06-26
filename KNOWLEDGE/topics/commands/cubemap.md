---
id: cubemap
name: "cubemap"
kind: command
module: ""
signature: "cubemap(int cmd, int FACEa, int FACEb, int FACEc, int FACEd, int FACEe, int FACEf)"
aliases: []
tags: ["command"]
deprecated: false
replacedBy: "the"
description: "Attaches 2D images to vertices of a 3D model"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cubemap

```flexscript
cubemap(int cmd, int FACEa, int FACEb, int FACEc, int FACEd, int FACEe, int FACEf)
```

Attaches 2D images to vertices of a 3D model

This command can be used to attach 2D images to vertices of a 3D model and the texture image is wallpapered onto the geometry of the 3D model. The six faces can be defined by FACEa, FACEb, FACEc, FACEd, FACEe, and FACEf. We can directly use the number of the reference texture after importing into media files.
The parameter cmd will take the following values whenever writing OpenGL call sequences.

1- CUBEMAP_INIT : To initialises the texture if nothing defined earlier
2- CUBEMAP_BEGIN : Enables texture generation and begin map
3- CUBEMAP_END : Disable texture generation and end map
4- CUBEMAP_SHUT : Delete map

## Example

```flexscript
cubemap(2, 39,39,39,39,39,39);
```

