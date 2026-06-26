---
id: activedocumentnode
name: "activedocumentnode"
kind: command
module: ""
signature: "activedocumentnode() -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets the currently active document window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# activedocumentnode

```flexscript
activedocumentnode() -> node
```

Gets the currently active document window

Returns the currently active document window. If the active document window is a tabbed window, it will return the active tab page node.

## Example

```flexscript
treenode activedoc = activedocumentnode();
```

