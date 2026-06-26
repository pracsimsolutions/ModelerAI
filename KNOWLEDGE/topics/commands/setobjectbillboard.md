---
id: setobjectbillboard
name: "setobjectbillboard"
kind: command
module: ""
signature: "setobjectbillboard(obj object, num mode)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Sets the billboard attribute of the visual tool object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setobjectbillboard

```flexscript
setobjectbillboard(obj object, num mode)
```

Sets the billboard attribute of the visual tool object

This command sets the billboard attribute of the visual tool object. The following modes are available:
0: No Billboard, the visual tool will display based on its set rotation.
1: Vertical Billboard (plane only), this will cause the plane to stand vertically in the model and always face toward the camera along the same viewing plane.
2: All Around Billboard (plane only), this will cause the plane always present its face to you no matter what the viewing angle is.
3: Screen Locked Billboard, locks the Visual Tool in front of the view.

## Example

```flexscript
setobjectbillboard(current, 1);
```

