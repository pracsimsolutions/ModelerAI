---
id: listsub
name: "listsub"
kind: command
module: ""
signature: "listsub(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the - operator"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listsub

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listsub(num qty, num val [, num a,...]) -> num
```

Deprecated, use the - operator

This command is deprecated. Use the - operator instead.

Subtraction of list. The list, of length qty, is traversed and the first value has all subsequent values subtracted from it and then is returned by this function.

## Example

```flexscript
listsub(5,2,1,2,3,4)
```

