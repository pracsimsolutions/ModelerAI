---
id: listmaximum
name: "listmaximum"
kind: command
module: ""
signature: "listmaximum(num qty, num val [, num a,...]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, use the Array class"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# listmaximum

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
listmaximum(num qty, num val [, num a,...]) -> num
```

Deprecated, use the Array class

This command is deprecated. Use the Array class instead.

Highest value. The list, of length qty, is searched and the value of the highest value is returned by this function.

## Example

```flexscript
listmaximum(5,2,1,2,3,4)
This returns a 4 because it is the highest value in the list.
```

