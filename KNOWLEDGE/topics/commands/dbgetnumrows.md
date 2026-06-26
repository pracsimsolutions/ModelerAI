---
id: dbgetnumrows
name: "dbgetnumrows"
kind: command
module: ""
signature: "dbgetnumrows() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbgetnumrows

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbgetnumrows() -> num
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
for (int x=1; x<=dbgetnumrows(); x++) Table("MyTable")[x][1] = stringtonum(dbgettablecell(x,1));
This copies the values from a row in the database to a row in a GlobalTable.
```

