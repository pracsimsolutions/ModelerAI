---
id: excelgetrangeparam
name: "excelgetrangeparam"
kind: command
module: ""
signature: "excelgetrangeparam(str rangename, num param) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Retrieves a location parameter about the named range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelgetrangeparam

```flexscript
excelgetrangeparam(str rangename, num param) -> num
```

Retrieves a location parameter about the named range of the currently open Excel workbook

Retrieves a location parameter about the named range. Valid values for param are:1 = Starting Row Number2 = Starting Column Number3 = Ending Row Number4 = Ending Column Number5 = Width6 = HeightThis command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).

## Example

```flexscript
int width = excelgetrangeparam("MyRange",5);
```

