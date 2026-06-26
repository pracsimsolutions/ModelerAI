---
id: rackgetlevelsize
name: "rackgetlevelsize"
kind: command
module: ""
signature: "rackgetlevelsize(obj rack, num bay, num level) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the y or z size of the level of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetlevelsize

```flexscript
rackgetlevelsize(obj rack, num bay, num level) -> num
```

Gets the y or z size of the level of the rack

Returns the y or z size of the given level of the rack.If the rack is a floor storage rack, this returns the y size. If the rack is a normal rack, this returns the z size.

## Example

```flexscript
rackgetlevelsize(current, 5, 4);
Returns the y or z size of bay 5, level 4.
```

