---
id: setsdtvalue
name: "setsdtvalue"
kind: command
module: ""
signature: "setsdtvalue(node sdtNode, str valName, num/str/node toVal) -> node"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: "only."
description: "For developer use"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setsdtvalue

```flexscript
setsdtvalue(node sdtNode, str valName, num/str/node toVal) -> node
```

For developer use

Primarily for developer use only. Sets the value of the named class member of a node that has "SimpleDataType" data. We
discourage using this command in model code because you risk breaking forward-compatibility. Future versions may redefine class members
and their names, so model code that accesses these values by name may not work in future versions.

