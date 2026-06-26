---
id: c
name: "c"
kind: command
module: ""
signature: "c<no parentheses> -> node"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: c."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# c

```flexscript
c<no parentheses> -> node
```

This command is used to reference the active node during a function's execution. If the function was called using nodefunction(), c returns a reference to the node on which the function is written. If the function is an event function, c returns a reference to the object that contains the event.

## Example

```flexscript
treenode current = ownerobject(c);
```

