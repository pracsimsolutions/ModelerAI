---
id: undo
name: "undo"
kind: command
module: ""
signature: "undo(obj view)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: undo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# undo

```flexscript
undo(obj view)
```

Undoes the previously recorded action for the specified view. The specified view must have an undohistory attribute to record undoable actions.

## Example

```flexscript
undo(view);
```

