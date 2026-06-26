---
id: closeallip
name: "closeallip"
kind: command
module: ""
signature: "closeallip(obj object)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Close all input ports of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeallip

```flexscript
closeallip(obj object)
```

Close all input ports of object

Close all input ports of object. Closing all input ports of an object will block entry to the object. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to closeinput() and stopinput().

## Example

```flexscript
closeallip(current)
```

