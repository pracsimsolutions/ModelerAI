---
id: getitemsendto
name: "getitemsendto"
kind: command
module: ""
signature: "getitemsendto(obj item) -> num"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Gets the value of the evaluated sendto for the item"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getitemsendto

```flexscript
getitemsendto(obj item) -> num
```

Gets the value of the evaluated sendto for the item

This returns the value of the evaluated sendto for the item if it is in a FixedResource and has been released.

## Example

```flexscript
getitemsendto(item)
```

