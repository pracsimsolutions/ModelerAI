---
id: time
name: "time"
kind: command
module: ""
signature: "time() -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Model.time"
description: "Deprecated, use the Model class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# time

> ⚠ **DEPRECATED.** Use `Model.time` instead.

```flexscript
time() -> num
```

Deprecated, use the Model class

This command is deprecated. Use Model.time instead.

Returns the current simulation time.

## Example

```flexscript
item.TimeIn = time();
This sets the label called "TimeIn" on the object referenced by item to the current simulation clock time.
```

