---
id: finishundorecord
name: "finishundorecord"
kind: command
module: ""
signature: "finishundorecord(obj view, num uniqueid)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: finishundorecord."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# finishundorecord

```flexscript
finishundorecord(obj view, num uniqueid)
```

Finishes an undo record for the specified view.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
finishundorecord(view, getundorecordid(record));
```

