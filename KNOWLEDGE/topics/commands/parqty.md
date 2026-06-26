---
id: parqty
name: "parqty"
kind: command
module: ""
signature: "parqty() -> num"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: parqty."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# parqty

```flexscript
parqty() -> num
```

For developer use. Get the number of parameters passed into the nodefunction. This is used together with the nodefunction command. The value returned only applies when using Flexscript, not C++.

## Example

```flexscript
parqty()
```

