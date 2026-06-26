---
id: getstaterecordvalue
name: "getstaterecordvalue"
kind: command
module: ""
signature: "getstaterecordvalue(num record, num value) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstaterecordvalue

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getstaterecordvalue(num record, num value) -> num
```

Deprecated

This command is deprecated. Returns one of the values from the state change record that is passed in as the first parameter to the function. This record is the return value from a call to getstaterecord(). The value passed to this function as the second parameter indicates which value from the state change record the user wishes to retrieve:
0 = The record ID. This is the record\'s row number in the movement table.
1 = The time of the state change.
2 = The state that the object changed to.
3 = The object that changed state. This is returned as an int and should be converted to a treenode using tonode().

## Example

```flexscript
See the example for getnumstaterecords().
```

