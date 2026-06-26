---
id: parval
name: "parval"
kind: command
module: ""
signature: "parval(num index) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "param"
description: "Deprecated, use param()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# parval

> ⚠ **DEPRECATED.** Use `param` instead.

```flexscript
parval(num index) -> num
```

Deprecated, use param()

This command is deprecated. Use param instead.
This command is used inside a function that is called by the nodefunction() command. It returns the parameter
passed to nodefunction specified by index as a number. The first additional parameter passed to nodefunction() is parameter 1, the second is
parameter 2, etc. Parameters can also be retrieved using parnode() and parstr() for casting them as a treenode and string respectively.

## Example

```flexscript
If a trigger/field is called with: nodefunction(thefuncnode, passingval) then within the trigger/field, getting access to
the passingval passed as the first additional parameter would be done with:double passedval = parval(1);
```

