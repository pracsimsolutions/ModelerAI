---
id: closeallop
name: "closeallop"
kind: command
module: ""
signature: "closeallop(obj object)"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: "this"
description: "Close all output ports of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# closeallop

```flexscript
closeallop(obj object)
```

Close all output ports of object

Close all output ports of object. Closing all output ports of an object will block the exit of that object. Modelers should only use this command when working with fluid objects because the internal behavior of discrete objects controls the opening and closing of their ports, and therefore this command may be overridden. For discrete objects the modeler is referred to closeoutput() and stopoutput().

## Example

```flexscript
closeallop(current)
```

