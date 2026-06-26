---
id: dbgetnumcols
name: "dbgetnumcols"
kind: command
module: ""
signature: "dbgetnumcols() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgetnumcols

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgetnumcols() -> num
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
for (int x=1; x<=dbgetnumcols(); x++)
Table("MyTable")[1][x] = stringtonum(dbgettablecell(1,x));

This copies the values from a column in the database to a column in a GlobalTable.
```

