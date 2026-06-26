---
id: dbgetfieldname
name: "dbgetfieldname"
kind: command
module: ""
signature: "dbgetfieldname(num column) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgetfieldname

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgetfieldname(num column) -> str
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
string ColName = dbgetfieldname(1);
This sets ColName to the name of the first column in the current database.
```

