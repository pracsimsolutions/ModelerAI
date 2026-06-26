---
id: excelreadnum
name: "excelreadnum"
kind: command
module: ""
signature: "excelreadnum(num row, num column [, num rowchar, num colchar]) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Read the numeric value from a cell of the active sheet of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelreadnum

```flexscript
excelreadnum(num row, num column [, num rowchar, num colchar]) -> num
```

Read the numeric value from a cell of the active sheet of the currently open Excel workbook

Reads a numeric value from a cell defined by row and column of the active sheet of the currently open Excel workbook.

## Example

```flexscript
excelreadnum(5,1)
```

