---
id: closeop
name: "closeop"
kind: command
module: ""
signature: "closeop(obj object, num outputportnum)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Close one output port of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeop

```flexscript
closeop(obj object, num outputportnum)
```

Close one output port of object

Close just outputportnum of object. Closing an output port of an object will block the exit from the object through that port. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to closeoutput() and stopoutput().

## Example

```flexscript
closeop(current,1)
```

