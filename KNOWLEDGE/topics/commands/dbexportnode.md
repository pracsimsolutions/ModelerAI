---
id: dbexportnode
name: "dbexportnode"
kind: command
module: ""
signature: "dbexportnode(obj tablenode, num startrow, num startcol, num numrows, num numcols)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "FlexScript command: dbexportnode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbexportnode

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbexportnode(obj tablenode, num startrow, num startcol, num numrows, num numcols)
```

This command is deprecated. Use Database.Connection instead.The starting row and column in the table are specified as well as how many rows and columnsto export.

## Example

```flexscript
dbexportnode(so(),1,1,4,5);
```

