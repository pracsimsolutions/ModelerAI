---
id: shapeinfo
name: "shapeinfo"
kind: command
module: ""
signature: "shapeinfo(num info[, p1, p2, p3]) -> var"
aliases: []
tags: ["command", "group-media"]
deprecated: false
replacedBy: null
description: "Gets or sets information about loaded 3D shapes."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# shapeinfo

```flexscript
shapeinfo(num info[, p1, p2, p3]) -> var
```

Gets or sets information about loaded 3D shapes.

## Example

```flexscript
shapeinfo(SHAPEINFO_GET_ANIMATION_COUNT, shapeIndex);
```

