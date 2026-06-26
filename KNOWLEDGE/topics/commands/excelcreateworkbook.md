---
id: excelcreateworkbook
name: "excelcreateworkbook"
kind: command
module: ""
signature: "excelcreateworkbook() -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Create an Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelcreateworkbook

```flexscript
excelcreateworkbook() -> num
```

Create an Excel workbook

This command creates a workbook in Excel. Only one workbook can be open at a time. The user must call excelclose() before trying to open or create another workbook, opening a new model, compiling the current model, or closing FlexSim. Before reading from the open file, excelsetsheet() must also be called.

## Example

```flexscript
excelcreateworkbook(); excelsetsheet("Sheet1"); excelwritenum(1,1,1); excelsave("NewWorkBook.xlsx"); excelclose(0);
This first creates a link between FlexSim and a new Excel workbook, sets the active sheet in the workbook to "Sheet1", writes a 1 to row 1 column 1, saves the workbook, then closes the workbook.
```

