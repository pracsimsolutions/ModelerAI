---
id: ipopno
name: "ipopno"
kind: command
module: ""
signature: "ipopno(obj object, num inputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "Output port number that is connected to inputportnum of object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ipopno

```flexscript
ipopno(obj object, num inputportnum) -> num
```

Output port number that is connected to inputportnum of object

This command returns the output port number that is connected to inputportnum of object.

## Example

```flexscript
int otherport = ipopno(current,1);
This sets otherport to the port number of the output port that the current object's first input port is connected to.
```

