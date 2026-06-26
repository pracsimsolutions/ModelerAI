---
id: excelimporttable
name: "excelimporttable"
kind: command
module: ""
signature: "excelimporttable(str/node/num table, num startrow, num startcol, num numrows, num numcols)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Imports data from the currently open Excel workbook to a table in FlexSim"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelimporttable

```flexscript
excelimporttable(str/node/num table, num startrow, num startcol, num numrows, num numcols)
```

Imports data from the currently open Excel workbook to a table in FlexSim

Imports data from Excel to a table in FlexSim. The table may be referenced by the name of a GlobalTable or by using a treenode reference to a table node. The type of data (string or number) to be exported is determined by the datatype of the cells in the FlexSim table. The user specifies the starting row and column number in Excel, and the total number of rows and columns to export.

## Example

```flexscript
excelimporttable("MyTable",1,1,5,3);
```

