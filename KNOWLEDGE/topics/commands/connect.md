---
id: connect
name: "connect"
kind: command
module: ""
signature: "connect(obj object1, num port1, obj object2, num port2)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "contextdragconnection"
description: "Deprecated, use contextdragconnection()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# connect

> ⚠ **DEPRECATED.** Use `contextdragconnection` instead.

```flexscript
connect(obj object1, num port1, obj object2, num port2)
```

Deprecated, use contextdragconnection()

This command is deprecated. Use contextdragconnection() instead.

Connect ports of two objects. The two objects are connected by checking whether the ports exist on the objects in question and then connecting them. The output port of the first object will be connected to the input of the second object. If the ports do not exist, or are already connected then the command will abort. To connect two objects by automatically adding ports, see the command objectconnect().

## Example

```flexscript
connect(model().subnodes[2],1,model().subnodes[3],2)
```

