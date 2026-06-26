---
id: rackgetcellcontent
name: "rackgetcellcontent"
kind: command
module: ""
signature: "rackgetcellcontent(obj rack, num bay, num level) -> num"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Gets the number of flow items in bay and level of the rack"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackgetcellcontent

```flexscript
rackgetcellcontent(obj rack, num bay, num level) -> num
```

Gets the number of flow items in bay and level of the rack

Returns the number of flow items in the specified bay and level of the rack

## Example

```flexscript
int numitems = rackgetcellcontent(current, 5, 3);
Sets numitems to the number of flowitems in bay 5, level 3 of the current rack.
```

