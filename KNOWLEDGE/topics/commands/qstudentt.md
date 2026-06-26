---
id: qstudentt
name: "qstudentt"
kind: command
module: ""
signature: "qstudentt(num confidence [0.9/0.95/0.99], num degreesoffreedom) -> num"
aliases: []
tags: ["command", "group-statistics"]
deprecated: false
replacedBy: null
description: "FlexScript command: qstudentt."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# qstudentt

```flexscript
qstudentt(num confidence [0.9/0.95/0.99], num degreesoffreedom) -> num
```

For developer use. Returns the quantity of the studentt distribution value for the given percent confidence and degrees of freedom.

## Example

```flexscript
qstudentt(0.95, nrofsamples - 1)
```

