---
id: draginfo
name: "draginfo"
kind: command
module: ""
signature: "draginfo(num operation[, num val])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets or set values related to draggin the mouse"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# draginfo

```flexscript
draginfo(num operation[, num val])
```

Gets or set values related to draggin the mouse

Get or set values related to dragging the mouse.

Operations:
DRAG_INFO_DX 1 - gets dx
DRAG_INFO_DY 2 - gets dy
DRAG_INFO_DZ 3 - gets dz
DRAG_INFO_MDX 4 - gets mdx
DRAG_INFO_MDY 5 - gets mdy
DRAG_INFO_BUTTON_STATE 6 - gets the button state
DRAG_INFO_SET_DX 7 - sets dx to val
DRAG_INFO_SET_DY 8 - sets dy to val
DRAG_INFO_SET_DZ 9 - sets dz to val
DRAG_INFO_SET_MDX 10 - sets mdx to val
DRAG_INFO_SET_MDY 11 - sets mdy to val
DRAG_INFO_SET_BUTTON_STATE 12 - sets the button state to val

## Example

```flexscript
double dx = draginfo(DRAG_INFO_DX);
```

