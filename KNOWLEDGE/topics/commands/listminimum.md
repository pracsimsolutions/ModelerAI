---
id: listminimum
name: "listminimum"
kind: command
module: ""
signature: "listminimum(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listminimum

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listminimum(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the Array class instead.

Lowest value. The list, of length qty, is searched and the value of the lowest value is returned by this function.

## Example

```flexscript
listminimum(5,2,1,2,3,4)
This returns 1 because that is the lowest value in the list.
```

