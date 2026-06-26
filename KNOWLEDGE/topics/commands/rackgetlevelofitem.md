---
id: rackgetlevelofitem
name: "rackgetlevelofitem"
kind: command
module: ""
signature: "rackgetlevelofitem(obj rack, obj item) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the level number of the rack that the item is placed in"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetlevelofitem

```flexscript
rackgetlevelofitem(obj rack, obj item) -> num
```

Gets the level number of the rack that the item is placed in

Returns the level number of the rack that the item is placed in

## Example

```flexscript
int levelnum = rackgetlevelofitem(current, item);
Set the levelnum variable to the level number containing item in the current rack.
```

