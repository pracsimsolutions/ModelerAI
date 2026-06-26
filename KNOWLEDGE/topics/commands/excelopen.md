---
id: excelopen
name: "excelopen"
kind: command
module: ""
signature: "excelopen(str workbook) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Open the Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelopen

```flexscript
excelopen(str workbook) -> num
```

Open the Excel workbook

This command opens the file called workbook in Excel. Only one workbook can be open at a time. The user must call excelclose() before trying to open another workbook, opening a new model, compiling the current model, or closing FlexSim. Before reading from the open file, excelsetsheet() must also be called.

## Example

```flexscript
excelopen("mydata.xls"); excelsetsheet("Sheet1"); double myvalue = excelreadnum(1,2); excelclose(0);
This first creates a link between FlexSim and an Excel workbook called "mydata.xls", then sets the active sheet in the workbook to "Sheet1", sets a local variable called myvalue to the number read from row 1 column 2, then closes the workbook without saving.
```

