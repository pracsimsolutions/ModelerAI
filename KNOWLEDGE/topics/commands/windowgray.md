---
id: windowgray
name: "windowgray"
kind: command
module: ""
signature: "windowgray(HWND window, num graystate)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: windowgray."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowgray

```flexscript
windowgray(HWND window, num graystate)
```

For developer use. Enables or disables the window based on the graystate parameter (1 or 0).

## Example

```flexscript
graywindow(windowfromnode(c),1);
```

