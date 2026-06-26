---
id: listmax
name: "listmax"
kind: command
module: ""
signature: "listmax(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listmax

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listmax(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the Array class instead.

Index of highest value. The list, of length qty, is searched and the position in the list of the highest value is returned by this function.

## Example

```flexscript
listmax(5,2,1,2,3,4)
This returns a 5 because 4 is the largest value and it is fifth in the list of five numbers.
```

