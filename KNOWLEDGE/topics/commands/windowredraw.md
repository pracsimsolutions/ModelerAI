---
id: windowredraw
name: "windowredraw"
kind: command
module: ""
signature: "windowredraw(HWND window, num x, num y, num sx, num sy, int flags)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: windowredraw."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowredraw

```flexscript
windowredraw(HWND window, num x, num y, num sx, num sy, int flags)
```

For developer use. A wrapper around the RedrawWindow() windows API command. The parameters x, y, sx, and sy define the client rectangle that the
call applies to, and flags defines the flags that will be passed to RedrawWindow().

## Example

```flexscript
windowredraw(windowfromnode(c),10,10,200,200, RDW_INVALIDATE);
```

