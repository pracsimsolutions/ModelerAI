---
id: redo
name: "redo"
kind: command
module: ""
signature: "redo(obj view)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: redo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# redo

```flexscript
redo(obj view)
```

Redoes the previously undone action for the specified view. The specified view must have an undohistory attribute to record undoable actions.

## Example

```flexscript
redo(view);
```

