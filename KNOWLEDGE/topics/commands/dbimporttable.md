---
id: dbimporttable
name: "dbimporttable"
kind: command
module: ""
signature: "dbimporttable(str tablename, num startrow, num startcol, num numrows, num numcols)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbimporttable

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbimporttable(str tablename, num startrow, num startcol, num numrows, num numcols)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbimportnode("MyTable4",2,2,3,3);
```

