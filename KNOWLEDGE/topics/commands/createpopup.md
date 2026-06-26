---
id: createpopup
name: "createpopup"
kind: command
module: ""
signature: "createpopup(str viewpath, str objectfocus, str viewfocus, obj relativeto, num x, num y, num sx, num sy)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "this"
description: "Creates a new popup window"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createpopup

```flexscript
createpopup(str viewpath, str objectfocus, str viewfocus, obj relativeto, num x, num y, num sx, num sy)
```

Creates a new popup window

Similar to the createview command in that it creates a new window. However, FlexSim manages this window so that whenever the user clicks outside of the window, the window will be closed. Code template pop-ups use this command to create a drop-down in which you can edit the template code. The pop-up view must contain a style attribute with the WS_POPUP style. You also specify a view node "relative to" which the popup's location will be set (usually whatever view you're writing code on). Also specify an offset location in pixels from relativeto's top-left corner, and a size of the pop-up.

## Example

```flexscript
treenode combobox = c.find("../pickoptions");
createpopup(
"VIEW:/standardviews/picklisttemplate",
combobox.find(">pickprimary+").getPath(0,1),
combocurrentnode(combobox).getPath(0,1),
combobox,
0,20,combobox.location.x, 150
);
This code is similar to what is used to create the template code pop-up for many of FlexSim's edit fields.
```

