---
id: getnextnetnodeoutport
name: "getnextnetnodeoutport"
kind: command
module: ""
signature: "getnextnetnodeoutport(num fromnn, num tonn) -> num"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the output port of the network node fromnn in order to get to tonn"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnextnetnodeoutport

```flexscript
getnextnetnodeoutport(num fromnn, num tonn) -> num
```

Gets the output port of the network node fromnn in order to get to tonn

Returns the output port, or edge number, to go to in getting from a source network node
to a destination network node.

## Example

```flexscript
int outport = getnextnetnodeoutport(rankfromnetnode(current), rankfromnetnode(Model.find("NN5")));
```

