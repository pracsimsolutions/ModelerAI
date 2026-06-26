---
id: windowmove
name: "windowmove"
kind: command
module: ""
signature: "windowmove(HWND window, num x, num y, num sx, num sy[, num refresh])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: windowmove."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowmove

```flexscript
windowmove(HWND window, num x, num y, num sx, num sy[, num refresh])
```

For developer use. Move a window to the specified x,y pixel location and give it the sx,sy size in pixels. If refresh is 1, then the window will repaint itself.

## Example

```flexscript
windowmove(windowfromnode(c),10,10,200,200,1)
```

