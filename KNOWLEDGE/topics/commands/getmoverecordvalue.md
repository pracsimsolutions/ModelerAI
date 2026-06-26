---
id: getmoverecordvalue
name: "getmoverecordvalue"
kind: command
module: ""
signature: "getmoverecordvalue(num record, num value) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getmoverecordvalue

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
getmoverecordvalue(num record, num value) -> num
```

Deprecated

This command is deprecated. Returns one of the values from the movement record that is passed in as the first parameter to the function. This record is the return value from a call to getmoverecord(). The value passed to this function as the second parameter indicates which value from the movement record the user wishes to retrieve:
0 = The record ID. This is the record\'s row number in the movement table.
1 = The ItemID of the flowitem whose movement is recorded by the record.
2 = The time the flowitem moved.
3 = The object that the flowitem moved from. This is returned as an int and should be converted to a treenode using tonode().
4 = The object that the flowitem moved to. This is returned as an int and should be converted to a treenode using tonode();
5 = The itemtype of the flowitem at the time it moved.

## Example

```flexscript
See the example for getnummoverecords().
```

