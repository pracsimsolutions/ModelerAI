---
id: createundorecord
name: "createundorecord"
kind: command
module: ""
signature: "createundorecord(obj view, obj object, num undocode[, num p1, num p2, num p3]) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: createundorecord."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createundorecord

```flexscript
createundorecord(obj view, obj object, num undocode[, num p1, num p2, num p3]) -> num
```

Creates an undo record for the specified view. Returns the uniqueid for the created record.

Undo codes:
UNDO_AGGREGATED 1
UNDO_MOVE_SIZE_ROTATE 2
UNDO_MOVE_SIZE_ROTATE_GROUP 3
UNDO_SELECTION_CHANGE 4
UNDO_CREATE_OBJECT 5
UNDO_DELETE_OBJECT 6
UNDO_CHANGE_DATA 7
UNDO_CHANGE_RANK 8
UNDO_CHANGE_NAME 9
UNDO_MOVE_OBJECT 10
UNDO_DELETE_COUPLING 11
UNDO_AGGREGATED_FIRST_RECORD 12
UNDO_DESTROY_BRANCH 13
UNDO_CHANGE_FLAGS 14
UNDO_REPLACE_OBJECT 15
UNDO_CUSTOM 16
UNDO_CALLBACK 17
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
int id = createundorecord(ownerobject(c), selectedobject(ownerobject(c)), UNDO_CUSTOM);
```

