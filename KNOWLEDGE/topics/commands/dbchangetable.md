---
id: dbchangetable
name: "dbchangetable"
kind: command
module: ""
signature: "dbchangetable(str tablename)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbchangetable

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbchangetable(str tablename)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbchangetable("Inventory"); This closes the currently open table and opens a table called Inventory.
```

