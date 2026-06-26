---
id: excelrangereadstr
name: "excelrangereadstr"
kind: command
module: ""
signature: "excelrangereadstr(str rangename, num row, num column [, num rowchar, num colchar]) -> str"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Read the text value from a cell of the specified range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelrangereadstr

```flexscript
excelrangereadstr(str rangename, num row, num column [, num rowchar, num colchar]) -> str
```

Read the text value from a cell of the specified range of the currently open Excel workbook

Reads a text value from a cell defined by row and column in the specified named range. The upper-left cell of the range is row 1, column 1.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).

## Example

```flexscript
excelreadstr("MyRange",5,1)
```

