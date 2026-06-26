---
id: dbgettablenum
name: "dbgettablenum"
kind: command
module: ""
signature: "dbgettablenum(num row, num col) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgettablenum

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgettablenum(num row, num col) -> num
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
double val = dbgettablenum(2,3);
```

