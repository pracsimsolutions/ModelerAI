---
id: getviewmenu
name: "getviewmenu"
kind: command
module: ""
signature: "getviewmenu(obj view, int menutype) -> var"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: getviewmenu."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getviewmenu

```flexscript
getviewmenu(obj view, int menutype) -> var
```

For developer use. Returns the HMENU reference of the menu based on the menutype.

Menutypes:
1: floatingtreemainmenubar
2: custommenu
3: popupmenu
4: thecustompopupmenu

## Example

```flexscript
var menu = menubelow(getviewmenu(c, 4), 0);
```

