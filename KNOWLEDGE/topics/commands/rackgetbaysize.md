---
id: rackgetbaysize
name: "rackgetbaysize"
kind: command
module: ""
signature: "rackgetbaysize(obj rack, num bay) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the x size of bay of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetbaysize

```flexscript
rackgetbaysize(obj rack, num bay) -> num
```

Gets the x size of bay of the rack

Returns the x size of the given bay of the rack.

## Example

```flexscript
rackgetbaysize(current, 5);
Returns the size of bay 5.
```

