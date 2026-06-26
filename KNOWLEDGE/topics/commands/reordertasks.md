---
id: reordertasks
name: "reordertasks"
kind: command
module: ""
signature: "reordertasks(obj dispatcher)"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Causes the dispatcher to reorder all its task sequences according to its queue strategy"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# reordertasks

```flexscript
reordertasks(obj dispatcher)
```

Causes the dispatcher to reorder all its task sequences according to its queue strategy

Causes the involved Dispatcher/TaskExecuter to reorder all its task sequences according to its queue strategy.

## Example

```flexscript
reordertasks(c)
```

