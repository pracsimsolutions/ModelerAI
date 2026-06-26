---
id: setitemsendto
name: "setitemsendto"
kind: command
module: ""
signature: "setitemsendto(obj item, num value)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: "this"
description: "FlexScript command: setitemsendto."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setitemsendto

```flexscript
setitemsendto(obj item, num value)
```

This sets the value of the sendto for the item if it is in a FixedResource. Only use this command if you are using an explicit FixedResource class object and want to store a sendto value on the item before releasing it, and then later use the releaseitem() command, passing the port in. If you want to change the evaluated sendto of an item that has already been released, execute holditem(), and then releaseitem() again, instead of this command.

## Example

```flexscript
setitemsendto(item, current.currentsendto)
```

