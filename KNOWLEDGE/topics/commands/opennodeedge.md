---
id: opennodeedge
name: "opennodeedge"
kind: command
module: ""
signature: "opennodeedge(obj networknode, num edgenum)"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Opens entry to the networknode's edge"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# opennodeedge

```flexscript
opennodeedge(obj networknode, num edgenum)
```

Opens entry to the networknode's edge

Opens entry to the networknode's specified edge number. Closing a node's edge with closenodeedge() blocks travelers from entry onto the edge. This command opens the edge up again.

## Example

```flexscript
opennodeedge(current.centerObjects[1], 3);
Opens node edge 3 of a NetworkNode connected to center port 1 of the current object.
```

