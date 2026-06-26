---
id: windowshow
name: "windowshow"
kind: command
module: ""
signature: "windowshow(HWND/node thewindow , num show)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: windowshow."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowshow

```flexscript
windowshow(HWND/node thewindow , num show)
```

For developer use. Show or hide a window. If the second parameter is 1, then the window will be shown. Otherwise the window will be hidden.

## Example

```flexscript
windowshow(windowfromnode(c))
```

