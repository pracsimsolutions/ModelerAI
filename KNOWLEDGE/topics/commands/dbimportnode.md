---
id: dbimportnode
name: "dbimportnode"
kind: command
module: ""
signature: "dbimportnode(obj tablenode, num startrow, num startcol, num numrows, num numcols)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbimportnode

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbimportnode(obj tablenode, num startrow, num startcol, num numrows, num numcols)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbimportnode(so(),1,1,4,5);
```

