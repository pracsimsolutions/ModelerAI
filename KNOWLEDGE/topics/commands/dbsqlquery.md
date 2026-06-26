---
id: dbsqlquery
name: "dbsqlquery"
kind: command
module: ""
signature: "dbsqlquery(str query[, num forwardOnlyCursor])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbsqlquery

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbsqlquery(str query[, num forwardOnlyCursor])
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
dbsqlquery("select processtime from timetable where type='1'");
This returns a new table based on the SQL query passed.
dbsqlquery("update timetable set processtime=type*2");
This updates the database based on the query passed.
```

