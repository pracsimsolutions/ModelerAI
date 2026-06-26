---
id: rackdrawvirtualcontent
name: "rackdrawvirtualcontent"
kind: command
module: ""
signature: "rackdrawvirtualcontent(obj rack, num bayfillperc, num levelfillperc, num itemdepth, num red, num green, num blue, num virtualonly)"
aliases: []
tags: ["command", "group-rack-object"]
deprecated: false
replacedBy: null
description: "Draw the rack's virtual content"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# rackdrawvirtualcontent

```flexscript
rackdrawvirtualcontent(obj rack, num bayfillperc, num levelfillperc, num itemdepth, num red, num green, num blue, num virtualonly)
```

Draw the rack's virtual content

Draws the rack's "virtual" content. This command will usually only be used if you are recycling flowitems using the rackrecycleitem command. When this command is executed, the rack goes through its content table, and draws a box for every flowitem in the rack, at its appropriate bay and level. If virtualonly is 1, then it will only draw a box in spots where a slot is filled, but no flowitem is present (the flowitem has been recycled). Bayfillperc and levelfillperc are values between 0 and 1, and specify the percent of the cell to fill with the drawn box. Item depth is the size each item takes up in the rack. For a floor storage rack, this the z size of the items. For a regular rack, it is the y size of the items.

## Example

```flexscript
rackdrawvirtualcontent(current, 0.9,0.9,3, 256,0, 0, 1);
```

