---
id: keyboardfocus
name: "keyboardfocus"
kind: command
module: ""
signature: "keyboardfocus([HWND/node focus]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "windowfromnode"
description: "Gets/Sets keyboard focus"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# keyboardfocus

```flexscript
keyboardfocus([HWND/node focus]) -> num
```

Gets/Sets keyboard focus

Used in GUI development. Gets/Sets the keyboard focus. If the focus parameter is left out, then this command will
return a reference to the current keyboard focus as an HWND handle.
Otherwise, it will set the focus to the keyboard focus to the passed parameter. Use windowfromnode() or getviewhwnd() to get access
to the HWND handle of a view node.

## Example

```flexscript
keyboardfocus(windowfromnode(edit));
```

