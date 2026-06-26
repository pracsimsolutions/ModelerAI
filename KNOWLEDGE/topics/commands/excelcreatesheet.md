---
id: excelcreatesheet
name: "excelcreatesheet"
kind: command
module: ""
signature: "excelcreatesheet(str name)"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Creates a new worksheet in the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelcreatesheet

```flexscript
excelcreatesheet(str name)
```

Creates a new worksheet in the currently open Excel workbook

Creates a new worksheet. The new sheet will be given the specified name.

## Example

```flexscript
excelcreatesheet("MyNewSheet");
```

