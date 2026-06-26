---
id: searchfornode
name: "searchfornode"
kind: command
module: ""
signature: "searchfornode(str/node searchstr, node startnode, num findnr[, num searchobjects])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: searchfornode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# searchfornode

```flexscript
searchfornode(str/node searchstr, node startnode, num findnr[, num searchobjects])
```

For developer use. Recursively searches the startnode for a node with a name containing the searchstr.
The findnr parameter is useful when the search finds multiple nodes containing the given name, specifying a number i here will give you
the ith node found. Set searchobjects to 1 to search the object's data nodes.

