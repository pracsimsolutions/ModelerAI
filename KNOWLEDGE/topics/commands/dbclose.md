---
id: dbclose
name: "dbclose"
kind: command
module: ""
signature: "dbclose()"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbclose

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbclose()
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
if (current.donegatheringdata == 1) dbclose();
If the label donegatheringdata on current is set to 1, the current database is closed.
```

