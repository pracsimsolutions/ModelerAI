---
id: fgldebuglevel
name: "fgldebuglevel"
kind: command
module: ""
signature: "fgldebuglevel(num level [0-3])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: fgldebuglevel."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fgldebuglevel

```flexscript
fgldebuglevel(num level [0-3])
```

For developer use. Set the OpenGL debug level. The higher the number, the more scrutiny is put on the system, and the more reporting is done. 0 is none, 3 is maximum.

## Example

```flexscript
fgldebuglevel(3)
```

