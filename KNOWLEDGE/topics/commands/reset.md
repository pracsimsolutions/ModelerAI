---
id: reset
name: "reset"
kind: command
module: ""
signature: "reset()"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "resetmodel"
description: "Deprecated, use resetmodel()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# reset

> ⚠ **DEPRECATED.** Use `resetmodel` instead.

```flexscript
reset()
```

Deprecated, use resetmodel()

This command is deprecated. Use resetmodel() instead.

For developer use. This command resets the model. The simulation clock is set to 0, and the OnReset event function is called for every object in the model. The OnReset events generally call the ResetTrigger functions that the users define. The model must be compiled before it can be reset. The resetmodel() command should typically be used instead of reset().

## Example

```flexscript
reset(); go(); This resets then runs the model.
```

