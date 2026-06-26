---
id: ipavailable
name: "ipavailable"
kind: command
module: ""
signature: "ipavailable(obj object, num inputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: ipavailable."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# ipavailable

```flexscript
ipavailable(obj object, num inputportnum) -> num
```

Returns a 1 if the upstream object through the specified object's input port is available. Particularly, it checks if the upstream object exists, if it is stopped, if its output is open, and if the connected output port is open. This function returns 0 if the upstream object isn't available.

## Example

```flexscript
ipavailable(current,1)
```

