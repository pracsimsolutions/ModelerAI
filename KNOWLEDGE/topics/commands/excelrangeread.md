---
id: excelrangeread
name: "excelrangeread"
kind: command
module: ""
signature: "excelrangeread(str rangename[treenode target], num row, num col[, num numRows, num numCols, num flags])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "excelrangereadnum"
description: "Reads an Excel range and dumps it to a treenode"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelrangeread

> ⚠ **DEPRECATED.** Use `excelrangereadnum` instead.

```flexscript
excelrangeread(str rangename[treenode target], num row, num col[, num numRows, num numCols, num flags])
```

Reads an Excel range and dumps it to a treenode

This function is overloaded so that it has two functions depending on which arguments you pass in.
The first (where you pass in a string) is deprecated. Use excelrangereadnum() and excelrangereadstr() instead.

The second (where you pass in a treenode) will read a range in Excel and assign all of the data in that range to the target treenode. If the treenode
has bundle data, the excel data will be set on the bundle. Otherwise, subnodes will be inserted under the target treenode that will have the Excel
data set on them as either string or number data, in the same format as Global Tables.

The origin of the range is specified by the row and col passed in, and the second pair of numbers will specify the size of the range. If 0 is passed in for any one of these,
Flexsim will automatically choose the boundary in that direction of the Excel range that has data.

Flags can be passed in to change exactly what is read from Excel. Those flags are EXCEL_DATA_AUTOMATIC, EXCEL_DATA_NUMBER, EXCEL_DATA_STRING,
EXCEL_DATE_AS_NUM, EXCEL_USE_COLUMNHEADERS and EXCEL_USE_ROWHEADERS.

EXCEL_DATA_AUTOMATIC will assign string or number data based on what the datatype of the Excel cell is. This is the default behavior.

EXCEL_DATA_NUMBER will force all Excel data to be read as a number.

EXCEL_DATA_STRING will force all Excel data to be read as a string.

EXCEL_DATE_AS_NUM will read any Excel dates as numbers readable by Flexsim as dates. Otherwise, the data will come in as a string.

EXCEL_USE_COLUMNHEADERS will take the first row of data and use it as names for the table columns instead of data.

EXCEL_USE_ROWHEADERS will take the first column of data and use it as names for the table rows instead of data.

## Example

```flexscript
treenode myTable = node ("Tools/GlobalTables/GlobalTable1>variables/data", model());

excelrangeread(myTable, 1, 1, 5, 0, EXCEL_DATA_AUTOMATIC | EXCEL_USE_COLUMNHEADERS);

This will read data from the active sheet starting at cell A1. It will read the first five rows, and read as many columns as have data in them.
The first row will be used as names for the columns of the Global Table we're writing to. Therefore, if the Excel sheet has 10 columns with data in them, a total of 40 nodes
will be inserted under the data variable of the Global Table. Each of those nodes will have the datatype of their corresponding Excel cell.
```

