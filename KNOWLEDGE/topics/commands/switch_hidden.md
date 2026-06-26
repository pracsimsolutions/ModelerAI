---
id: switch_hidden
name: "switch_hidden"
kind: command
module: ""
signature: "switch_hidden(obj object[, num off/on]) -> num"
aliases: []
tags: ["command"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_hidden."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_hidden

```flexscript
switch_hidden(obj object[, num off/on]) -> num
```

Not implemented

Get or set the "is hidden" flag of the node object. A value of 1 is enable, a value of 0 is disable, and a value of -1 gets the current value. The "is hidden" flag is not currently implemented.

## Example

```flexscript
switch_hidden(so(),1)
```

