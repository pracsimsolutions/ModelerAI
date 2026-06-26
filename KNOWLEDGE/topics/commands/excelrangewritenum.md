---
id: excelrangewritenum
name: "excelrangewritenum"
kind: command
module: ""
signature: "excelrangewritenum(str rangename, num row, num column, num value)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: "excelrangewrite."
description: "Write the numeric value to a cell of the specified range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelrangewritenum

```flexscript
excelrangewritenum(str rangename, num row, num column, num value)
```

Write the numeric value to a cell of the specified range of the currently open Excel workbook

Writes a numeric value to a cell defined by row and column in the specified named range. The upper-left cell of the range is row 1, column 1.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).
For writing a whole table of values to Excel, use excelrangewrite.

## Example

```flexscript
excelwritenum("MyRange",5,1,99.87)
```

