---
id: dbopen
name: "dbopen"
kind: command
module: ""
signature: "dbopen(str databasename, str command {SQL string or Tablename}, num usetable [, num showlogin]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbopen

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbopen(str databasename, str command {SQL string or Tablename}, num usetable [, num showlogin]) -> num
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbopen("itemdrive","timetable",1);
This opens a database with the alias "itemdrive" and opens the table called "timetable" in Table mode.
dbopen("warehouse","select * from inventory",0);
This opens a database with the alias "warehouse" and performs a SQL query on it. The database is open in SQL mode.
```

