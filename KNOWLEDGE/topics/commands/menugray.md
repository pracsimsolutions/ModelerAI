---
id: menugray
name: "menugray"
kind: command
module: ""
signature: "menugray(HMENU menu, num index, num on/off)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: menugray."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# menugray

```flexscript
menugray(HMENU menu, num index, num on/off)
```

For developer use. Set the gray state of the menu item with the specified index (starting with 0) in the specified menu.

## Example

```flexscript
menugray(menumain(), 2, 1)
```

