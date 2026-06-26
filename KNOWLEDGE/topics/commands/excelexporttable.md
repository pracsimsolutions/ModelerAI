---
id: excelexporttable
name: "excelexporttable"
kind: command
module: ""
signature: "excelexporttable(str/node/num table, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Exports data from a table within FlexSim to the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelexporttable

```flexscript
excelexporttable(str/node/num table, num startrow, num startcol, num numrows, num numcols [, num rowchar, num colchar])
```

Exports data from a table within FlexSim to the currently open Excel workbook

Exports data from a table within FlexSim to Excel. The table may be referenced by the name of a GlobalTable or by using a treenode reference to a table node. The type of data (string or number) to be exported is determined by the datatype of the cells in the FlexSim table. The user specifies the starting row and column number in Excel, and the total number of rows and columns to export.

## Example

```flexscript
excelexporttable("Table1",1,1,5,3);
```

