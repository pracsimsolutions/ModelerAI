---
id: transfernodeobj
name: "transfernodeobj"
kind: command
module: ""
signature: "transfernodeobj(node movenode, node container)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Moves movenode into the container node's object data"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# transfernodeobj

```flexscript
transfernodeobj(node movenode, node container)
```

Moves movenode into the container node's object data

This command moves movenode into the container node's object data. This is like the moveobject command, except no events (ie. OnSend, OnReceive) are executed.

## Example

```flexscript
transfernodeobj(current.attrs.find("body"), current.next);
```

