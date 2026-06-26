---
id: pickcolor
name: "pickcolor"
kind: command
module: ""
signature: "pickcolor()"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: "pickedcolorred"
description: "FlexScript command: pickcolor."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# pickcolor

```flexscript
pickcolor()
```

For developer use. Opens a color picker window to allow the user to pick a color. Use pickedcolorred(), pickedcolorgreen(), pickedcolorblue() commands to retrieve the rgb color component values.

## Example

```flexscript
pickcolor(); int bluecomponent = pickedcolorblue();
The bluecomponent variable will be set to the rgb value for the blue component of the color chosen with pickcolor().
```

