---
id: racksetcellcontent
name: "racksetcellcontent"
kind: command
module: ""
signature: "racksetcellcontent(obj rack, num bay, num level, num contentval)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: "this"
description: "Sets the virtual content of the rack's cell to contentval"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# racksetcellcontent

```flexscript
racksetcellcontent(obj rack, num bay, num level, num contentval)
```

Sets the virtual content of the rack's cell to contentval

Sets the "virtual" content of the rack's cell to the specified value. This should only be used if you are recycling and restoring items to the rack with rackrecycleitem() and rackrestoreitem(). You will usually use this command to set an initial "virtual" content on reset.

## Example

```flexscript
racksetcellcontent(current, 5, 3, 2);
```

