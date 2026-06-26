---
id: excelreadstr
name: "excelreadstr"
kind: command
module: ""
signature: "excelreadstr(num row, num column [, num rowchar, num colchar]) -> str"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Read the text value from a cell of the active sheet of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelreadstr

```flexscript
excelreadstr(num row, num column [, num rowchar, num colchar]) -> str
```

Read the text value from a cell of the active sheet of the currently open Excel workbook

Reads a text value from a cell defined by row and column of the active sheet in the currently open Excel workbook.

## Example

```flexscript
excelreadstr(5,1)
```

