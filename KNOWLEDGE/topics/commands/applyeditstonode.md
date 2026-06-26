---
id: applyeditstonode
name: "applyeditstonode"
kind: command
module: ""
signature: "applyeditstonode(obj view, obj editednode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: applyeditstonode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# applyeditstonode

```flexscript
applyeditstonode(obj view, obj editednode)
```

For developer use. Applies view changes to a node. This is usually only needed for a table view, and is essentially the same as mouse-clicking off of the table's currently active cell. This is the opposite of applytoedits.

## Example

```flexscript
applyeditstonode(ownerview(c), current)
```

