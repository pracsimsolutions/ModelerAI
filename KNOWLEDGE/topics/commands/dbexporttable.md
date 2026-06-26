---
id: dbexporttable
name: "dbexporttable"
kind: command
module: ""
signature: "dbexporttable(str tablename, num startrow, num startcol, num numrows, num numcols)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbexporttable

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbexporttable(str tablename, num startrow, num startcol, num numrows, num numcols)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbexportnode("MyTable4",2,2,3,3);
```

