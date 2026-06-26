---
id: excelreadcell
name: "excelreadcell"
kind: command
module: ""
signature: "excelreadcell(num row, num col[, num flags]) -> var"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Returns the value of a cell in the active worksheet as a variant"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelreadcell

```flexscript
excelreadcell(num row, num col[, num flags]) -> var
```

Returns the value of a cell in the active worksheet as a variant

Returns the value of a cell in the active worksheet as a variant. Flags can be specified to specify what exactly is returned from Excel.
Those flags are EXCEL_DATA_NUMBER, EXCEL_DATA_STRING, and EXCEL_DATE_AS_NUM.

EXCEL_DATA_NUMBER will read the Excel data as a number.

EXCEL_DATA_STRING will read the Excel data as a string.

EXCEL_DATE_AS_NUM will interpret any Excel dates as a Flexsim readable date number.

## Example

```flexscript
Variant value = excelreadcell(1, 5, EXCEL_DATA_NUMBER | EXCEL_DATE_AS_NUM);

This returns the value of the 5 column in the first row as a number. If it's a date, the number returned will be Flexsim readable as a date.
```

