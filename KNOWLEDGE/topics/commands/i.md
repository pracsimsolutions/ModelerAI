---
id: i
name: "i"
kind: command
module: ""
signature: "i<no parentheses> -> node"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: i."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# i

```flexscript
i<no parentheses> -> node
```

For developer use. This command is used only in event functions. It is used to reference the object that was passed to the value of 0 is) command as the involved object.

## Example

```flexscript
string involvedname = i.name;
This sets involvedname to the name of the object that was passed to value of 0 is) as the involved object.
```

