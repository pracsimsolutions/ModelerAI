---
id: getviewhwnd
name: "getviewhwnd"
kind: command
module: ""
signature: "getviewhwnd(obj view, int windowtype) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "several"
description: "Gets the HWND handle for a certain window of a view"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getviewhwnd

```flexscript
getviewhwnd(obj view, int windowtype) -> num
```

Gets the HWND handle for a certain window of a view

Returns a windows HWND handle for a certain window of a view. Each view may use several windows to display data properly. For example, a tree view contains the main window that displays the tree as well as a window that displays the string or number data on a node for editing, as well as a window that displays/edits the name of a node. The windowtype parameter specifies which window handle you want returned, as follows:

1 - the main control (same as using windowfromnode())
2 - datacontrol - The scintilla code control that displays data of a node in a tree, and in a script view
3 - control - the control that displays/edits a highlighted node's name in the tree
4 - statusbar - the statusbar of the view if it has one
5 - tooltip - the tooltip control of the view
6 - pointercontrol - I think this is only used in a tree that displays list heads

## Example

```flexscript
sendwindowmessage(getviewhwnd(treeview,2), SCI_SETSEL, 4, 8);
```

