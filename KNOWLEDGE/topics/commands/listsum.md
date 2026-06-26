---
id: listsum
name: "listsum"
kind: command
module: ""
signature: "listsum(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listsum

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listsum(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the + operator instead.

Sum of values. The sum of all the values in the list, of length qty, is returned by this function.

## Example

```flexscript
listsum(5,2,1,2,3,4)
```

