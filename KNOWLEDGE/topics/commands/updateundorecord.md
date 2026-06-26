---
id: updateundorecord
name: "updateundorecord"
kind: command
module: ""
signature: "updateundorecord(obj view, num uniqueid[, num updatetype, num p1, num p2])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: updateundorecord."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# updateundorecord

```flexscript
updateundorecord(obj view, num uniqueid[, num updatetype, num p1, num p2])
```

Updates an undo record for the specified view.

The following undo record types should be updated:
UNDO_MOVE_SIZE_ROTATE
UNDO_MOVE_SIZE_ROTATE_GROUP
UNDO_SELECTION_CHANGE
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
updateundorecord(view, getundorecordid(record));
```

