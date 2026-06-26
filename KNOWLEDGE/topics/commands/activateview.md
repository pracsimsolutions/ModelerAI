---
id: activateview
name: "activateview"
kind: command
module: ""
signature: "activateview(obj viewnode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "makeactive"
description: "FlexScript command: activateview."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# activateview

```flexscript
activateview(obj viewnode)
```

For developer use. This command makes a GUI window active and visible. It should be used to open a window that is stored in the view tree without creating a copy of the view in the active tree, in order to make direct changes that will be saved in the view tree instead of being deleted when the window is closed. Generally, the command createview() should be used to open GUI windows. Use makeactive() if you want to make an already open view the active, or top, window.

## Example

```flexscript
activateview(views.find("standardviews/other/FindReplace"));
This opens the node called FindReplace in the folder called standardviews/other in the view tree.
```

