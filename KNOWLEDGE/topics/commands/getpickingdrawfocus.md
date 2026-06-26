---
id: getpickingdrawfocus
name: "getpickingdrawfocus"
kind: command
module: ""
signature: "getpickingdrawfocus(obj theview, num whattoget, num pickedid)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets the picking draw focus"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getpickingdrawfocus

```flexscript
getpickingdrawfocus(obj theview, num whattoget, num pickedid)
```

Gets the picking draw focus

Gets the picking draw focus. Used in OnClick to know what was clicked.
The whattoget parameter can be PICK_TYPE, PICK_OBJECT, or PICK_SECONDARY_OBJECT.
pickedid should be PICK_PRESSED for the "pressed" pick, PICK_HOVERED for the "hovered" pick, PICK_RELEASED for the "released" pick, or PICK_MOVED for "mouse moved" pick.

## Example

```flexscript
int picktype = getpickingdrawfocus(view, PICK_TYPE, 0);
```

