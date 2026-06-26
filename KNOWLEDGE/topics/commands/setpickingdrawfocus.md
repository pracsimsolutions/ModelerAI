---
id: setpickingdrawfocus
name: "setpickingdrawfocus"
kind: command
module: ""
signature: "setpickingdrawfocus(obj theview, obj focus, num what[, obj secondaryfocus, num flags])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "this"
description: "FlexScript command: setpickingdrawfocus."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setpickingdrawfocus

```flexscript
setpickingdrawfocus(obj theview, obj focus, num what[, obj secondaryfocus, num flags])
```

For developer use. Sets the draw code following this call to be picked as the focus when clicking or dragging. Usually you would use this
to draw custom sizers and grippers on your object, and then implement an OnDrag or an OnClick event on the object that would call getpickingdrawfocus()
to figure out what part was clicked.
flags is a bitwise combination of one "DETECT_DRAG" setting, as well as optional "override" bits. The override bits aren't used to define picking focus,
but will cause FlexSim to override its normal drawing of sizers and bounding boxes.
DETECT_DRAG_X, DETECT_DRAG_Y, DETECT_DRAG_Z - will only detect dragging along the given axis, relative to the object being drawn.
Use this with the draginfo() command in your OnDrag event to determine where the object is being dragged.
DETECT_DRAG_XY, DETECT_DRAG_YZ, DETECT_DRAG_XZ - will only detect dragging along the given plane. Again you would use this with the draginfo() command in the OnDrag event.

OVERRIDE_DRAW_SIZER_X, OVERRIDE_DRAW_SIZER_Y, OVERRIDE_DRAW_SIZER_Z, OVERRIDE_DRAW_SIZER_X_NEG, OVERRIDE_DRAW_SIZER_Y_NEG, OVERRIDE_DRAW_SIZER_Z_NEG, OVERRIDE_DRAW_SIZER_ALL
- these flags will cause the engine to not draw the flagged sizers
OVERRIDE_DRAW_YELLOW_BOX - causes FlexSim to not draw the yellow box around the object when selected
OVERRIDE_DRAW_GREEN_BOX - causes FlexSim to not draw the green "statistics-active" box around the object
OVERRIDE_DRAW_REDBOX - causes FlexSim to not draw the red selection box around the object when selected
OVERRIDE_DRAW_HOVERBOX - causes FlexSim to not draw the yellow hover box around the object when hovering over it
OVERRIDE_DRAW_ALL - causes FlexSim to not draw any sizers or boxes around the object

OVERRIDE_SET_MOUSE_DOWN_PICK - explicitly sets the view's "mouse down" pick to the defined values. This is called outside of
the drawing sequence, and updates the pick information "as-if" the mouse had been pressed on the given object.
OVERRIDE_SET_MOUSE_DOWN_PICK - explicitly sets the view's "mouse up" pick to the defined values. This is called outside of
the drawing sequence, and updates the pick information "as-if" the mouse had been released on the given object.
OVERRIDE_SET_MOUSE_MOVE_PICK - explicitly sets the view's "mouse move" pick to the defined values. This is called outside of
the drawing sequence, and updates the pick information "as-if" the mouse had been moved on the given object.
OVERRIDE_SET_MOUSE_HOVER_PICK - explicitly sets the view's "mouse hover" pick to the defined values. This is called outside of
the drawing sequence, and updates the pick information "as-if" the mouse had been hovered over the given object.

## Example

```flexscript
setpickingdrawfocus(view, current, 20, drawsurrogate(current).first.subnodes[3], DETECT_DRAG_Y);
```

