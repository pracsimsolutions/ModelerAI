---
id: rackrestoreitem
name: "rackrestoreitem"
kind: command
module: ""
signature: "rackrestoreitem(obj rack, num binrank, num bay, num level, num position)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Restores a flowitem from the flowitem bin into the bay and level"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackrestoreitem

```flexscript
rackrestoreitem(obj rack, num binrank, num bay, num level, num position)
```

Restores a flowitem from the flowitem bin into the bay and level

Restores a flowitem from the flowitem bin into the given bay and level.

## Example

```flexscript
rackrestoreitem(current, 5, 7, 3, 1);
This restores a flowitem from the 5th ranked flowitem of the flowitem bin (usually the textured colored box) into bay 7, level 3, position 1 of the rack.
```

