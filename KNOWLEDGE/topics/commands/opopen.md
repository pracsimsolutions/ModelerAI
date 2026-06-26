---
id: opopen
name: "opopen"
kind: command
module: ""
signature: "opopen(obj object, num outputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: opopen."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# opopen

```flexscript
opopen(obj object, num outputportnum) -> num
```

Returns a 1 if the outputportnum on object is open. Otherwise returns zero.

## Example

```flexscript
opopen(current,1)
```

