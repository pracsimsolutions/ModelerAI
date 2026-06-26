---
id: listinlist
name: "listinlist"
kind: command
module: ""
signature: "listinlist(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listinlist

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listinlist(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the Array class instead.

Index of occurrence of value in list. The list, of length qty, is searched and the position in the list of the value val is returned by this function.

## Example

```flexscript
listinlist(5,3,2,1,2,3,4)
This returns the index number 4 because 3 is the fourth number in the list of 5 numbers.
```

