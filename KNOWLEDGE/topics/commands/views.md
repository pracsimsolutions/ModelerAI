---
id: views
name: "views"
kind: command
module: ""
signature: "views() -> node"
aliases: []
tags: ["command", "group-tree-access"]
deprecated: false
replacedBy: null
description: "Reference to the views tree"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# views

```flexscript
views() -> node
```

Reference to the views tree

This command returns a reference to the views tree. The views tree contains all of the information about the windows that are currently open as well as all of the available GUIs. When defining string paths to nodes in the view tree, VIEW can be used as in the following example: node("VIEW:/standardviews/ortho");

## Example

```flexscript
activateview(views().find("standardviews/runcontrol"));
This activates the GUI called "runcontrol" which is stored in the node called "standardviews" of the view tree.
```

