---
id: closeip
name: "closeip"
kind: command
module: ""
signature: "closeip(obj object, num inputportnum)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Close one input port of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeip

```flexscript
closeip(obj object, num inputportnum)
```

Close one input port of object

Close just inputportnum of object. Closing an input port of an object will block entry to the object through that port. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to closeinput() and stopinput().

## Example

```flexscript
closeip(current,1)
```

