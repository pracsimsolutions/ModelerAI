---
id: while
name: "while"
kind: command
module: ""
signature: "while(num expression) {}"
aliases: []
tags: ["command", "group-language"]
deprecated: false
replacedBy: null
description: "FlexScript command: while."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# while

```flexscript
while(num expression) {}
```

Repeatedly execute the following block {} while the expression returns a non-zero value. Be careful with this function because it can cause FlexSim to freeze if the value never becomes false.

## Example

```flexscript
while(current.subnodes.length > 4) { current.last.destroy(); }
```

