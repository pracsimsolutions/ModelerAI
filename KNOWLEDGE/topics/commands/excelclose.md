---
id: excelclose
name: "excelclose"
kind: command
module: ""
signature: "excelclose(num save [1/0])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Close the currently open Excel workbook"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelclose

```flexscript
excelclose(num save [1/0])
```

Close the currently open Excel workbook

This command closes the currently open Excel workbook. If save is 1, the workbook will be saved before it closes. If save is 0, changes made to the workbook will be discarded.

## Example

```flexscript
See the example for excelopen().
```

