---
id: combocurrentnode
name: "combocurrentnode"
kind: command
module: ""
signature: "combocurrentnode(obj combonode) -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: combocurrentnode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# combocurrentnode

```flexscript
combocurrentnode(obj combonode) -> node
```

For developer use. Returns the current node of the specified picklist combobox.

## Example

```flexscript
createview( "VIEW:/standardviews/picklisttemplate","", combocurrentnode(c.find("../pickoptions").getPath(0, 1) );
```

