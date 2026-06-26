---
id: addsphere
name: "addsphere"
kind: command
module: ""
signature: "addsphere(obj object, num x, num y, num z, num radius)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "FlexScript command: addsphere."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# addsphere

```flexscript
addsphere(obj object, num x, num y, num z, num radius)
```

For developer use. Adds a collision sphere with the specified radius to the specified object at location x, y, z relative to the object. TaskExecuters allow users to add collision spheres and draw collision spheres through the object's Parameters window, but commands are needed to add and draw collision spheres to other objects when needed.

## Example

```flexscript
addsphere(Processor1, 2, -1.25, .75, 1);
```

