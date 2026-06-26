---
id: listremove
name: "listremove"
kind: command
module: ""
signature: "listremove(obj entryOrBackOrder)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "List.backOrders"
description: "Deprecated, use List.backOrders()[backOrderNum].remove() or List.entries()[enterNum].remove()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listremove

> ⚠ **DEPRECATED.** Use `List.backOrders` instead.

```flexscript
listremove(obj entryOrBackOrder)
```

Deprecated, use List.backOrders()[backOrderNum].remove() or List.entries()[enterNum].remove()

This command is deprecated. Use List.backOrders()[backOrderNum].remove()
or List.entries()[entryNum].remove() instead.

Removes an entry or back order from its owning list.

