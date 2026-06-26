---
id: meshaddvertex
name: "meshaddvertex"
kind: command
module: ""
signature: "meshaddvertex(node meshnode) -> num"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Adds a vertex to a mesh"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# meshaddvertex

```flexscript
meshaddvertex(node meshnode) -> num
```

Adds a vertex to a mesh

Adds a vertex to the mesh associated with meshnode.
Returns the index of the added vertex.

## Example

```flexscript
meshaddvertex(Model.find("mesh"))
```

