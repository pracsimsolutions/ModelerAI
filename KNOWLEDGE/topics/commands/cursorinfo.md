---
id: cursorinfo
name: "cursorinfo"
kind: command
module: ""
signature: "cursorinfo(obj/num view, num operation, num x/y/z, num pos/move) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: cursorinfo."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cursorinfo

```flexscript
cursorinfo(obj/num view, num operation, num x/y/z, num pos/move) -> num
```

For developer use. This command represents a lot of functionality wrapped up into one command.
It is a way of getting information regarding the position/state of the mouse for a window control.

view: the associated view, such as an ortho or planar view. This can also be an HWND (stored in a double).

operation: the type of operation you want to do:
1 - gets x or y positions of the cursor in a planar view
2 - get orthographic or perspective view coordinates
3 - get current mouse button state.
return value:
0 : no buttons down
1 : left down
2 : right down
3 : right and left down
4 : double left click
4 - update the cursor position
5 - gets x or y positions of the cursor relative to the view
6 - gets x or y positions of the cursor in screen coordinates
7 - sets the x or y position of the cursor

xyz: 1(x), 2(y), or 3(z); not used for operation 3. For operation 4, if xyz=1, the cursor position will be updated with objects being drawing, if xyz=2 it will be updated with only the floor drawn.

pos/move: 1(pos) or 2(move); 1 gets absolute mouse position, and 2 gets the change since last update. only used for operations 1-2.

The best way to test this functionality out is just by trial and error. Add an OnMouseMove attribute to the ortho view (make sure you rebind the view's attributes after you've added it: Build|Rebind Object Attributes), then print information to the output console.

## Example

```flexscript
cursorinfo(c,2,1,1)
```

