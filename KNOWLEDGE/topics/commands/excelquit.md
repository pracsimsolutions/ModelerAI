---
id: excelquit
name: "excelquit"
kind: command
module: ""
signature: "excelquit()"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: "excelclose"
description: "Close Microsoft Excel"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelquit

```flexscript
excelquit()
```

Close Microsoft Excel

This command closes Microsoft Excel if it is open. After this command is called, no other commands to Excel should be called. Any Excel files that were open should be closed before this command is called. You may get an Excel prompt to save changes to any workbooks that were left open. Use excelclose() to avoid this. This command must be called if excellaunch() was called previously.

## Example

```flexscript
see excellaunch();
```

