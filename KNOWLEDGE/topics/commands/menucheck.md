---
id: menucheck
name: "menucheck"
kind: command
module: ""
signature: "menucheck(HMENU menu, num index, num on/off)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: menucheck."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# menucheck

```flexscript
menucheck(HMENU menu, num index, num on/off)
```

For developer use. Set the check state of the menu item with the specified index (starting with 0) in the specified menu.

## Example

```flexscript
menucheck(menumain(), 2, 1)
```

