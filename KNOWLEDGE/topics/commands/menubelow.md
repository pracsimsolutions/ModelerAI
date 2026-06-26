---
id: menubelow
name: "menubelow"
kind: command
module: ""
signature: "menubelow(HMENU menu, num index) -> var"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: menubelow."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# menubelow

```flexscript
menubelow(HMENU menu, num index) -> var
```

For developer use. Get the menu item with the specified index (starting with 0) in the specified menu.

## Example

```flexscript
var menu = menubelow(menumain(),4);
```

