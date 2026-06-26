---
id: getcurrentundorecord
name: "getcurrentundorecord"
kind: command
module: ""
signature: "getcurrentundorecord(obj view[, int flags = 0x1]) -> obj"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: getcurrentundorecord."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getcurrentundorecord

```flexscript
getcurrentundorecord(obj view[, int flags = 0x1]) -> obj
```

Returns a pointer to the treenode associated with the current undo record in the undohistory of the specified view. This is
the record that would be undone first when you perform an undo operation. It is also the most recently created record
(potentially deep in the undo aggregation hierarchy) when and aggregated undo operation is being created.The flags
parameter lets you optionally adjust what should be returned. 0x1 means get the top-most record for a
move-size-rotate undo. 0x2 indicates get the top-most record in a normal aggregated undo.
See Miscellaneous Concepts >
Advanced Undo
for more information.

## Example

```flexscript
treenode record = getcurrentundorecord(view);
```

