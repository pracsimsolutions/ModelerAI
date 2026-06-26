---
id: excelrangewrite
name: "excelrangewrite"
kind: command
module: ""
signature: "excelrangewrite(treenode source, num row, num col, num flags)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "of"
description: "Reads data from a source table node and writes it to the active Excel sheet"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelrangewrite

> ⚠ **DEPRECATED.** Use `of` instead.

```flexscript
excelrangewrite(treenode source, num row, num col, num flags)
```

Reads data from a source table node and writes it to the active Excel sheet

This function will write all the of the data from the source table node to the active Excel sheet. If the treenode has bundle data,
this will read the bundle and write it to the sheet, otherwise it will look for child nodes with data on them, like a Global Table. The row and col passed in
specify where on the Excel sheet to begin writing the data. All of the data on the source node will always be written. Flags can be passed in to change the use
of headers. Those flags are EXCEL_USE_COLUMNHEADERS and EXCEL_USE_ROWHEADERS.

EXCEL_USE_COLUMNHEADERS will write any existing column headers as a separate row in the Excel table.

EXCEL_USE_ROWHEADERS will write any existing row headers as a separate column in the Excel table.

## Example

```flexscript
treenode myTable = node("Tools/GlobalTables/GlobalTable1>variables/data", model());

excelrangewrite(myTable, 1, 1, EXCEL_USE_ROWHEADERS | EXCEL_USE_COLUMNHEADERS);

This will take all of the data in GlobalTable1 and write it to the active Excel sheet, starting at cell A1. Before it writes any of the actual data,
there will be a row of column headers and column of row headers.
```

