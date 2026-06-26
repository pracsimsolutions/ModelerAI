---
id: receiveitem
name: "receiveitem"
kind: command
module: ""
signature: "receiveitem(obj station [, num port ] )"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Tells the FixedResource to open inputs and receive a flowitem"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# receiveitem

```flexscript
receiveitem(obj station [, num port ] )
```

Tells the FixedResource to open inputs and receive a flowitem

Tells the FixedResource station to open its inputs and receive a flowitem.
The port parameter is optional, and should only be used if you know exactly which port you
want to receive from. If the port parameter is specified, this object will try to receive
any item from the upstream object if its corresponding output port is open, ignoring any
items' previous send-to return values (similarly to how pull logic works). This function
should be executed only once to receive an item, and it should only executed again once you
have received (or have confirmed reception of) that item.See also releaseitem() command.
This function should generally only be used from within a BasicFR's code fields,
as other objects implement receiveitem() on their own. If the object receives an
item immediately, then it will return 1, otherwise 0.

## Example

```flexscript
receiveitem(current)
```

