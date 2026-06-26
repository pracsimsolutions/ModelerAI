---
id: excelexportnode
name: "excelexportnode"
kind: command
module: ""
signature: "excelexportnode(node table, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "excelexporttable"
description: "Deprecated, use excelexporttable()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelexportnode

> ⚠ **DEPRECATED.** Use `excelexporttable` instead.

```flexscript
excelexportnode(node table, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])
```

Deprecated, use excelexporttable()

This command is deprecated. Use excelexporttable() instead.

## Example

```flexscript
excelexportnode(reftable("Table1"),1,1,5,3);
```

