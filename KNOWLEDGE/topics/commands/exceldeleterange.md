---
id: exceldeleterange
name: "exceldeleterange"
kind: command
module: ""
signature: "exceldeleterange(str rangename)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Deletes the specified range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exceldeleterange

```flexscript
exceldeleterange(str rangename)
```

Deletes the specified range of the currently open Excel workbook

Deletes the specified range. Does not affect the data in the range.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).

## Example

```flexscript
exceldeleterange("MyRange");
```

