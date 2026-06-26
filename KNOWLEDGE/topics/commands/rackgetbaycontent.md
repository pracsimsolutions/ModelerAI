---
id: rackgetbaycontent
name: "rackgetbaycontent"
kind: command
module: ""
signature: "rackgetbaycontent(obj rack, num bay) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the number of flow items in the bay of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetbaycontent

```flexscript
rackgetbaycontent(obj rack, num bay) -> num
```

Gets the number of flow items in the bay of the rack

Returns the number of flow items in the specified bay of the rack

## Example

```flexscript
int numitems = rackgetbaycontent(current, 5);
Sets numitems to the number of flowitems in bay 5 of the current rack.
```

