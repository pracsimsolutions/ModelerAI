---
id: transfernode
name: "transfernode"
kind: command
module: ""
signature: "transfernode(node movenode, node container)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "treenode.up"
description: "Deprecated, use treenode.up"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# transfernode

> ⚠ **DEPRECATED.** Use `treenode.up` instead.

```flexscript
transfernode(node movenode, node container)
```

Deprecated, use treenode.up

This command is deprecated. Use treenode.up to set the node's parent (container).

This command moves movenode into the container node. This is like the moveobject command, except no events (ie. OnSend, OnReceive) are executed.

## Example

```flexscript
transfernode(current.labels["inactivequeue"].first, current.labels["activequeue"]);
```

