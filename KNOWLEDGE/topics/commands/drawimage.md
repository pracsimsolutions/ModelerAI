---
id: drawimage
name: "drawimage"
kind: command
module: ""
signature: "drawimage(obj view, str imagepath, num x, num y, num width, num height, num scaling)"
aliases: []
tags: ["command", "group-drawing"]
deprecated: false
replacedBy: null
description: "Draws an image"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# drawimage

```flexscript
drawimage(obj view, str imagepath, num x, num y, num width, num height, num scaling)
```

Draws an image

Draws the image at imagepath in the view. The scaling value specifies
how the image will be positioned and scaled.
NO_SCALE (or 0): Draws the image at location x,y with the width and height being
based on the size of the image.
SCALE_DOWN: Draws the image centered in the rectangle created by x,y,width,height
and scales the image proportionally down to fit if necessary.
SCALE_UP_DOWN: Draws the image centered in the rectangle created by x,y,width,
height and scales the image proportionally up or down to fit.
SCALE_INDEPENDENT: Draws the image at x,y with the size of width and height.

## Example

```flexscript
drawimage(c, "buttons\\visible_eye.png", 0, 10, 0, 0, 0);
```

