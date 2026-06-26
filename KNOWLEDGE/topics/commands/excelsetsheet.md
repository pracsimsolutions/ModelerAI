---
id: excelsetsheet
name: "excelsetsheet"
kind: command
module: ""
signature: "excelsetsheet(str sheetname) -> num"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Sets the active worksheet of the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelsetsheet

```flexscript
excelsetsheet(str sheetname) -> num
```

Sets the active worksheet of the currently open Excel workbook

This command sets the active worksheet in the currently open Excel workbook. Whenever excelreadnum() or excelreadstr() are called, the value read will come from the active worksheet. The active worksheet may be set many times for the same open workbook.

## Example

```flexscript
See the example for excelopen().
```

