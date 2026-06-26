---
id: closenodeedge
name: "closenodeedge"
kind: command
module: ""
signature: "closenodeedge(obj object, num edgenum)"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Closes entry to the networknode's edge"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closenodeedge

```flexscript
closenodeedge(obj object, num edgenum)
```

Closes entry to the networknode's edge

Closes entry to the networknode's specified edge number. The edge's color will turn orange indicating it is closed. Open or close status of an edge will only be checked by a traveler when traveling from one node to the next (not when entering a node from an offset)

## Example

```flexscript
closenodeedge(current.centerObjects[1], 3);
```

