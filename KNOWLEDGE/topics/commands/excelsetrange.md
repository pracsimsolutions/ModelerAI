---
id: excelsetrange
name: "excelsetrange"
kind: command
module: ""
signature: "excelsetrange(str rangename, num startrow, num startcolumn, num endrow, num endcolumn)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Creates or changes a named range of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelsetrange

```flexscript
excelsetrange(str rangename, num startrow, num startcolumn, num endrow, num endcolumn)
```

Creates or changes a named range of the currently open Excel workbook

Creates or changes a named range. The range will be on the active sheet. The range's upper-left cell is defined by startrow and startcolumn. The bottom-right cell is defined by endrow and endcolumn.This command does nothing if FlexSim is set to communicate with Excel using DDE (by calling maintenance(1000,1), not FlexSim's default setting).

## Example

```flexscript
excelsetrange("MyRange",5,5,10,15);
```

