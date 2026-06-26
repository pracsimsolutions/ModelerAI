---
id: openallip
name: "openallip"
kind: command
module: ""
signature: "openallip(obj object)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Open all input ports of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# openallip

```flexscript
openallip(obj object)
```

Open all input ports of object

Open all input ports of object. If an input port has a pass-through state as a result (both connected ports are open), both objects will be sent a notification message that the port has become "ready". The object with the input port will receive an OnInOpen message and the object with an output port will receive an OnOutOpen message. During traversal of the ports, any given port will only allow pass-through evaluation once. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to openinput() and resumeinput().

## Example

```flexscript
openallip(current)
```

