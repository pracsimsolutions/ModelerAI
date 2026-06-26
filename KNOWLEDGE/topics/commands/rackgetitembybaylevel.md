---
id: rackgetitembybaylevel
name: "rackgetitembybaylevel"
kind: command
module: ""
signature: "rackgetitembybaylevel(obj rack, num bay, num level, num itemrank) -> obj"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets a reference to an item in the bay and level of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetitembybaylevel

```flexscript
rackgetitembybaylevel(obj rack, num bay, num level, num itemrank) -> obj
```

Gets a reference to an item in the bay and level of the rack

Returns a reference to an item in the specified bay and level of the rack. Itemrank specifies the rank of the item in that cell.

## Example

```flexscript
treenode theitem = rackgetitembybaylevel(current, 5, 4, 2);
Sets theitem pointer to the second flowitem in bay 5, level 4 of the current object.
```

