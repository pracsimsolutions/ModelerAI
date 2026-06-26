---
id: dbgetmode
name: "dbgetmode"
kind: command
module: ""
signature: "dbgetmode() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgetmode

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgetmode() -> num
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
if (dbgetmode() != DBMODE_CLOSED) ColName = dbgetfieldname(1);
If the database is open, ColName is set to the name of the first column in the database.
```

