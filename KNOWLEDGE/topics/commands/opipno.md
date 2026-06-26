---
id: opipno
name: "opipno"
kind: command
module: ""
signature: "opipno(obj object, num outputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "Input port number that is connect to outputportnum of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# opipno

```flexscript
opipno(obj object, num outputportnum) -> num
```

Input port number that is connect to outputportnum of object

This command returns the input port number that is connected to outputportnum of object.

## Example

```flexscript
int otherport = opipno(current,1);
This sets otherport to the number of the input port that current's first output port is connected to.
```

