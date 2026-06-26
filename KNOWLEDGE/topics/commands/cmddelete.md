---
id: cmddelete
name: "cmddelete"
kind: command
module: ""
signature: "cmddelete(node thenode)"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "destroyobject"
description: "FlexScript command: cmddelete."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cmddelete

```flexscript
cmddelete(node thenode)
```

For developer use. Use destroyobject().
Delete thenode.

## Example

```flexscript
cmddelete(model().subnodes[4]))
```

