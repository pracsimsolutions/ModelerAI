---
id: activeview
name: "activeview"
kind: command
module: ""
signature: "activeview() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "nodefromwindow"
description: "Gets the currently active window as an HWND"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# activeview

```flexscript
activeview() -> num
```

Gets the currently active window as an HWND

Returns the currently active window as an HWND. To get the node associated with the window, use nodefromwindow()

## Example

```flexscript
treenode viewnode = nodefromwindow(activeview());
```

