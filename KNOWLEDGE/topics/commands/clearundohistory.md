---
id: clearundohistory
name: "clearundohistory"
kind: command
module: ""
signature: "clearundohistory([obj view])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: clearundohistory."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# clearundohistory

```flexscript
clearundohistory([obj view])
```

Deletes all undo records in the undohistory of the specified view. If no view is specified, it will clear the global undo history used by the 3D and tree windows.

## Example

```flexscript
clearundohistory(view);
```

