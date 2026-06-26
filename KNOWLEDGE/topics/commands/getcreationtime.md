---
id: getcreationtime
name: "getcreationtime"
kind: command
module: ""
signature: "getcreationtime(obj object) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the time that a flow item was created"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getcreationtime

```flexscript
getcreationtime(obj object) -> num
```

Gets the time that a flow item was created

Returns the time that a flowitem was first created.

## Example

```flexscript
time() - getcreationtime(item)
```

