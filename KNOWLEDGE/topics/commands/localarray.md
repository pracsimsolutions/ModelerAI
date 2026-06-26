---
id: localarray
name: "localarray"
kind: command
module: ""
signature: "localarray(num qty[, num datatype])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# localarray

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
localarray(num qty[, num datatype])
```

Deprecated, use Array class

This command is deprecated. Use the Array class instead.

Allocate general purpose local array with the given quantity of local variables. Type can be: 1 = num, 2 = str, 3 = coupling, 4 = obj. May only be used in flexscript.

## Example

```flexscript
localarray(1,4)
```

