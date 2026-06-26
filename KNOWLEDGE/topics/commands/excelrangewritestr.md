---
id: excelrangewritestr
name: "excelrangewritestr"
kind: command
module: ""
signature: "excelrangewritestr(str rangename, num row, num column, str value)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: "excelrangewrite."
description: "Write the string value to a cell of the specified range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelrangewritestr

```flexscript
excelrangewritestr(str rangename, num row, num column, str value)
```

Write the string value to a cell of the specified range of the currently open Excel workbook

Writes string value to a cell defined by row and column in the specified named range. The upper-left cell of the range is row 1, column 1.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).
For writing a whole table of values to Excel, use excelrangewrite.

## Example

```flexscript
excelwritestr("MyRange",5,1,"My String")
```

