---
id: getnetnodespline
name: "getnetnodespline"
kind: command
module: ""
signature: "getnetnodespline(obj netnode, num edgenr) -> obj"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: "spline"
description: "Gets a reference to the spline head of netnode's edge edgenr"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnetnodespline

```flexscript
getnetnodespline(obj netnode, num edgenr) -> obj
```

Gets a reference to the spline head of netnode's edge edgenr

Returns a reference to the spline head for the specified edge of the network node. You can
use spline() commands to then access location data for that spline. Note that the spline head may not be
inside the network node you're passing, but rather may be in the network node on the other side of the edge.
You can use the isedgeforward() command to query whether the spline is inside the network node. If not, then
spline percentages disances passed to the spline() command will be backwards.

## Example

```flexscript
treenode splinehead = getnetnodespline(netnode, 2);
```

