---
id: openip
name: "openip"
kind: command
module: ""
signature: "openip(obj object, num inputportnum)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Open one input port of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# openip

```flexscript
openip(obj object, num inputportnum)
```

Open one input port of object

Open just inputportnum of object. Opening an input port of an object will allow entry to the object through that port. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to openinput() and resumeinput().

## Example

```flexscript
openip(so(),1)
```

