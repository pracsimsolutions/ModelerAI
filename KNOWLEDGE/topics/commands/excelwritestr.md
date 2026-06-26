---
id: excelwritestr
name: "excelwritestr"
kind: command
module: ""
signature: "excelwritestr(num row, num column, str value [, num rowchar, num colchar])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Write the string value to a cell of the active sheet of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelwritestr

```flexscript
excelwritestr(num row, num column, str value [, num rowchar, num colchar])
```

Write the string value to a cell of the active sheet of the currently open Excel workbook

Writes string value to a cell defined by row and column of the active sheet in the currently open Excel workbook.

## Example

```flexscript
excelwritestr(5,1,"My String")
```

