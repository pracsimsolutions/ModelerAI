---
id: dbsettablecell
name: "dbsettablecell"
kind: command
module: ""
signature: "dbsettablecell(num row, num col, str value)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Database.Connection"
description: "Deprecated. Use Database.Connection"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dbsettablecell

> ⚠ **DEPRECATED.** Use `Database.Connection` instead.

```flexscript
dbsettablecell(num row, num col, str value)
```

Deprecated. Use Database.Connection

This command is deprecated. Use Database.Connection instead.

## Example

```flexscript
for (int x=1; x<= model().subnodes.length; x++) dbsettablecell(x,1,model().subnodes[x].name);
This places the names of all of the objects in the model into the first column of the current database.
```

