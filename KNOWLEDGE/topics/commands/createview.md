---
id: createview
name: "createview"
kind: command
module: ""
signature: "createview(str viewpath[, str objectfocus, str viewfocus]) -> node"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: createview."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createview

```flexscript
createview(str viewpath[, str objectfocus, str viewfocus]) -> node
```

For developer use. This command creates a new GUI window in the active node of the views tree. This window is a copy of the window whose path is given in viewpath. The window will have an object focus of objectfocus and a view focus of viewfocus. Objectfocus and viewfocus are generally the same for most GUI windows. They should be the path of the node that this GUI window is representing.

## Example

```flexscript
createview("VIEW:/standardviews/other/FindReplace","MAIN:/project/model/Tools/precision","MAIN:/project/model/Tools/precision");
This creates the Find Replace GUI.
```

