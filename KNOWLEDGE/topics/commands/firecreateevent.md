---
id: firecreateevent
name: "firecreateevent"
kind: command
module: ""
signature: "firecreateevent(obj object, num x, num y, num z, num iscopy [, num libraryfunctiononly])"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: firecreateevent."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# firecreateevent

```flexscript
firecreateevent(obj object, num x, num y, num z, num iscopy [, num libraryfunctiononly])
```

For developer use. Fires the creation event of the specified object. x, y, and z are the rz the object was dropped at.

## Example

```flexscript
firevalue of 0 islast(model()),0,0,0, 1);
```

