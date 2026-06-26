---
id: openop
name: "openop"
kind: command
module: ""
signature: "openop(obj object, num outputportnum)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Open one output port of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# openop

```flexscript
openop(obj object, num outputportnum)
```

Open one output port of object

Open just outputportnum of object. Opening an output port of an object will allow exiting from the object through that port. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to openoutput() and resumeoutput().

## Example

```flexscript
openop(so(),1)
```

