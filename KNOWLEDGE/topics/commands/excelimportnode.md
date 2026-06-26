---
id: excelimportnode
name: "excelimportnode"
kind: command
module: ""
signature: "excelimportnode(obj node, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "excelimporttable"
description: "Deprecated, use excelimporttable()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelimportnode

> ⚠ **DEPRECATED.** Use `excelimporttable` instead.

```flexscript
excelimportnode(obj node, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])
```

Deprecated, use excelimporttable()

This command is deprecated. Use excelimporttable() instead

## Example

```flexscript
excelimportnode(so(),1,1,5,3);
```

