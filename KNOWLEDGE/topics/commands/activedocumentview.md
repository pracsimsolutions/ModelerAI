---
id: activedocumentview
name: "activedocumentview"
kind: command
module: ""
signature: "activedocumentview() -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "activedocumentnode"
description: "Gets the currently active document window as an HWND"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# activedocumentview

```flexscript
activedocumentview() -> num
```

Gets the currently active document window as an HWND

Returns the currently active document window as an HWND. To get the node associated with the window, use activedocumentnode().

## Example

```flexscript
treenode activedoc = nodefromwindow(activedocumentview());
```

