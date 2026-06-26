---
id: bcgetitemposition
name: "bcgetitemposition"
kind: command
module: ""
signature: "bcgetitemposition(obj basicconveyor, obj item)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated, legacy conveyors are no longer supported"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bcgetitemposition

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
bcgetitemposition(obj basicconveyor, obj item)
```

Deprecated, legacy conveyors are no longer supported

Deprecated, Legacy conveyors are no longer supported.
This command is used in conjunction with the BasicConveyor object
The bcgetitemposition command returns the current position of an item on a BasicConveyor object. The position is measured along the conveyor's length from the head of the conveyor to the flowitem's edge furthest from the head of the conveyor.
See bcsetitemconveystate command documentation for a general description of the BasicConveyor object.

## Example

```flexscript
bcgetitemposition(current,current.first);
```

