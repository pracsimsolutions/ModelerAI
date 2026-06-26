---
id: rackgetbayofitem
name: "rackgetbayofitem"
kind: command
module: ""
signature: "rackgetbayofitem(obj rack, obj item) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the bay number of the rack that the item is placed in"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetbayofitem

```flexscript
rackgetbayofitem(obj rack, obj item) -> num
```

Gets the bay number of the rack that the item is placed in

Returns the bay number of the rack that the item is placed in

## Example

```flexscript
int baynum = rackgetbayofitem(current, item);
Sets baynum to the bay number containing item in the current rack.
```

