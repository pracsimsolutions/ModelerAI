---
id: so
name: "so"
kind: command
module: ""
signature: "so() -> node"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Gets a reference to the last node designated as so()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# so

```flexscript
so() -> node
```

Gets a reference to the last node designated as so()

This command returns a reference to the last node that was designated the selected object. This command is only used in the scripting window. To designate a node as so, right-click on the node in the tree and select Edit|Designate This Node (so). Any node in the tree can be designated the selected object.

## Example

```flexscript
getnodename(so())
This returns the name of the object that is currently designated the selected object. This command would generally only be used in the scripting window. The final semicolon (;) is not needed in the scripting window.
```

