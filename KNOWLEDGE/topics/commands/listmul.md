---
id: listmul
name: "listmul"
kind: command
module: ""
signature: "listmul(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the * operator"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listmul

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listmul(num qty, num val [, num a,...]) -> num
```

Deprecated, use the * operator

This command is deprecated. Use the * operator instead.

Product of list. The product of all the values in the list, of length qty, is returned by this function.

## Example

```flexscript
listmul(5,2,1,2,3,4)
This returns the value 48.
```

