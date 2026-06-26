---
id: excelwritenum
name: "excelwritenum"
kind: command
module: ""
signature: "excelwritenum(num row, num column, num value [, num rowchar, num colchar])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Write the numeric value to a cell of the active sheet of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelwritenum

```flexscript
excelwritenum(num row, num column, num value [, num rowchar, num colchar])
```

Write the numeric value to a cell of the active sheet of the currently open Excel workbook

Writes a numeric value to a cell defined by row and column of the active sheet in the currently open Excel workbook.

## Example

```flexscript
excelwritenum(5,1,99.87)
```

