---
id: ipopen
name: "ipopen"
kind: command
module: ""
signature: "ipopen(object object, num inputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: ipopen."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ipopen

```flexscript
ipopen(object object, num inputportnum) -> num
```

Returns a 1 if the inputportnum on object is open. Otherwise returns zero.

## Example

```flexscript
ipopen(current,1)
```

