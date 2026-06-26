---
id: dbgettablecell
name: "dbgettablecell"
kind: command
module: ""
signature: "dbgettablecell(num row, num col) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgettablecell

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgettablecell(num row, num col) -> str
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
double tablevalue = stringtonum(dbgettablecell(1,1));
This sets tablevalue to the number that is stored in the first row and first column of the database.
```

