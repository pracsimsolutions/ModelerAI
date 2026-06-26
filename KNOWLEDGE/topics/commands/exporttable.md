---
id: exporttable
name: "exporttable"
kind: command
module: ""
signature: "exporttable(node exportnode, str filename[, num exportcolumnnames = 0, num exportrownames = 0, str listseparator = 0, str decimalseparator = 0])"
aliases: []
tags: ["command", "group-tables"]
deprecated: false
replacedBy: null
description: "Export a table in .CSV format"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exporttable

```flexscript
exporttable(node exportnode, str filename[, num exportcolumnnames = 0, num exportrownames = 0, str listseparator = 0, str decimalseparator = 0])
```

Export a table in .CSV format

Export a table in .CSV (Comma Separated Value) format.

## Example

```flexscript
exporttable(so(),"mytable.csv",1, 1)
This exports the table designated as so(), including row and column headers.
```

