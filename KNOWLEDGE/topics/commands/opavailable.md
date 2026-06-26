---
id: opavailable
name: "opavailable"
kind: command
module: ""
signature: "opavailable(obj object, num outputportnum) -> num"
aliases: []
tags: ["command", "group-object-ports"]
deprecated: false
replacedBy: null
description: "FlexScript command: opavailable."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# opavailable

```flexscript
opavailable(obj object, num outputportnum) -> num
```

Returns a 1 if the downstream object through the specified object's output port is available. Particularly, it checks if the downstream object exists, if it is stopped, if its input is open, and if the connected input port is open. This function returns 0 if the downstream object isn't available.

## Example

```flexscript
opavailable(current,1)
```

