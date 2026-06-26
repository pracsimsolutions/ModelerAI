---
id: listmin
name: "listmin"
kind: command
module: ""
signature: "listmin(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listmin

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listmin(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the Array class instead.

Index of lowest value. The list, of length qty, is searched and the position in the list of the lowest value is returned by this function.

## Example

```flexscript
listmin(5,2,1,2,3,4)
This returns 2 because 1 is the lowest value and it is second in the list.
```

