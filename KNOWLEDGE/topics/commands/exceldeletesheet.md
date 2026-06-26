---
id: exceldeletesheet
name: "exceldeletesheet"
kind: command
module: ""
signature: "exceldeletesheet(str name [, int allowwarning])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Delete the worksheet in the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# exceldeletesheet

```flexscript
exceldeletesheet(str name [, int allowwarning])
```

Delete the worksheet in the currently open Excel workbook

Deletes the specified worksheet. If allowwarning is 1, you will be warned before you delete a sheet with data.

## Example

```flexscript
exceldeletesheet("MyNewSheet");
```

