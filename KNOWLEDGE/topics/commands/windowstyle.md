---
id: windowstyle
name: "windowstyle"
kind: command
module: ""
signature: "windowstyle(HWND/obj window [, num style, num useexstyle]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets the windowstyle of window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# windowstyle

```flexscript
windowstyle(HWND/obj window [, num style, num useexstyle]) -> num
```

Gets the windowstyle of window

Used in GUI development. Dynamically gets/sets the window style of the given window.
If style is 0, this will simply return the current style of the window. Otherwise, it will set it.
If useexstyle is 1, it will apply/get the extended style of the window. Refer to Windows SDK documentation for more information on
window styles.

## Example

```flexscript
As an example, this command is used in the perspective view's "Full Screen" context menu option to change
the style of the perspective window so that it no longer has a title bar when it is in full screen mode.
```

