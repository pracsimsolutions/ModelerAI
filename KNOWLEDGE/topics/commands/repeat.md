---
id: repeat
name: "repeat"
kind: command
module: ""
signature: "repeat(num nr, expression)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "FlexScript command: repeat."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# repeat

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
repeat(num nr, expression)
```

Executes expression the specified number of times. The expression can use "count" to get the 1-based number of the iteration being executed.

## Example

```flexscript
repeat(current.outObjects.length, current.outObjects[count].input.close);
This example will close the input of all objects connected to the output ports of current.
```

