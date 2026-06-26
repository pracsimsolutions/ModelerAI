---
id: applytoedits
name: "applytoedits"
kind: command
module: ""
signature: "applytoedits(obj view, obj editednode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: applytoedits."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# applytoedits

```flexscript
applytoedits(obj view, obj editednode)
```

For developer use. Applies programmatic changes to a node if that node has been selected in a view. This is usually only needed for a table view. This is the opposite of applyeditstonode().

## Example

```flexscript
applytoedits(ownerview(c), current)
```

