---
id: cpcpno
name: "cpcpno"
kind: command
module: ""
signature: "cpcpno(obj object, num port) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: cpcpno."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# cpcpno

```flexscript
cpcpno(obj object, num port) -> num
```

This command returns the center port number that is connected to the specified center port number of object.

## Example

```flexscript
int otherport = cpcpno(current,1);
This sets otherport to the number of the port that current's first center port is connected to.
```

