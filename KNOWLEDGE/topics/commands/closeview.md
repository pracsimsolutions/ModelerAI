---
id: closeview
name: "closeview"
kind: command
module: ""
signature: "closeview(node viewnode)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "destroyobject"
description: "FlexScript command: closeview."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeview

```flexscript
closeview(node viewnode)
```

For developer use. This command closes the view specified as viewnode. If viewnode is not a viewwindow or GUI, this command does nothing. The view is not removed from the active views folder, however. Use destroyobject() to remove it.

## Example

```flexscript
treenode activewindows = views().find("active"); while (activewindows.subnodes.length != 0) { closeview(activewindows.first); }
This closes all of the windows that are in the active node of the views tree. This will generally close all of the windows that are currently open.
```

