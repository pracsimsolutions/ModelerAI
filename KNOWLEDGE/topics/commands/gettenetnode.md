---
id: gettenetnode
name: "gettenetnode"
kind: command
module: ""
signature: "gettenetnode(obj taskexecuter) -> obj"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the current network node that a taskexecuter is attached to with an A Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# gettenetnode

```flexscript
gettenetnode(obj taskexecuter) -> obj
```

Gets the current network node that a taskexecuter is attached to with an A Connection

Returns the current network node that a taskexecuter (TE) is attached to with a red line.

## Example

```flexscript
treenode netnode = gettenetnode(current);
```

