---
id: excelgetrangesheet
name: "excelgetrangesheet"
kind: command
module: ""
signature: "excelgetrangesheet(str rangename) -> str"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Gets the name of the worksheet associated with rangename of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelgetrangesheet

```flexscript
excelgetrangesheet(str rangename) -> str
```

Gets the name of the worksheet associated with rangename of the currently open Excel workbook

Returns the name of the worksheet where the specified range is located.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).

## Example

```flexscript
pt(excelgetrangesheet("MyRange"));
```

