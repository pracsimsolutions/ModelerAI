---
id: systemwindow
name: "systemwindow"
kind: command
module: ""
signature: "systemwindow(num windowtype) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: systemwindow."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# systemwindow

```flexscript
systemwindow(num windowtype) -> num
```

For developer use. Get HWND handle to a system window: 1-Output 2-System 3-Compiler 4-Trace 5-Commands 6-Attributes 7-Completions 8-app 9-appsb 10-Output Edit 11-Compiler Edit 12-Trace Edit 13-System Edit

## Example

```flexscript
systemwindow(1)
```

