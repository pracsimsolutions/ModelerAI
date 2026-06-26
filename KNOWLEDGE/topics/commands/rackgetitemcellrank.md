---
id: rackgetitemcellrank
name: "rackgetitemcellrank"
kind: command
module: ""
signature: "rackgetitemcellrank(obj rack, obj item) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the item's rank within the rack's cell"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetitemcellrank

```flexscript
rackgetitemcellrank(obj rack, obj item) -> num
```

Gets the item's rank within the rack's cell

Returns the rank of the item within its given cell in a rack.

## Example

```flexscript
int theRank = rackgetitemcellrank(current, item);
```

