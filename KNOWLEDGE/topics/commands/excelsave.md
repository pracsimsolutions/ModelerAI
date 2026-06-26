---
id: excelsave
name: "excelsave"
kind: command
module: ""
signature: "excelsave([str filename])"
aliases: []
tags: ["command", "group-communication"]
deprecated: false
replacedBy: null
description: "Saves the currently open Excel file"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# excelsave

```flexscript
excelsave([str filename])
```

Saves the currently open Excel file

Saves the currently open Excel file. If a filename is passed to this command, the file will be saved with that name. This is the equivalent of selecting Save As from Excel's File menu. If no parameters are passed to this command, the file will be saved with its current name, and the original file will be overwritten. This is the equivalent of selecting Save from Excel's File menu.

## Example

```flexscript
excelsave("runresults_001.xls")
```

