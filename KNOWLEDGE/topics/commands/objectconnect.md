---
id: objectconnect
name: "objectconnect"
kind: command
module: ""
signature: "objectconnect(obj object1, obj object2)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "contextdragconnection"
description: "Deprecated, use contextdragconnection()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# objectconnect

> ⚠ **DEPRECATED.** Use `contextdragconnection` instead.

```flexscript
objectconnect(obj object1, obj object2)
```

Deprecated, use contextdragconnection()

This command is deprecated. Use contextdragconnection() instead.

Connect two objects, adding needed ports. Object1 will have an output port added, and object2 will have an inport added as needed. Then the ports are connected.

## Example

```flexscript
connect(so(),so().next)
```

