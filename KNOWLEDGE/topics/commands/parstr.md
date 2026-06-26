---
id: parstr
name: "parstr"
kind: command
module: ""
signature: "parstr(num index) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "param"
description: "Deprecated, use param()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# parstr

> ⚠ **DEPRECATED.** Use `param` instead.

```flexscript
parstr(num index) -> str
```

Deprecated, use param()

This command is deprecated. Use param instead.
This command is used inside a function that is called by the nodefunction() command. It returns the parameter passed to nodefunction specified by index as a string. The first additional parameter passed to nodefunction() is parameter 1, the second is parameter 2, etc. Parameters can also be retrieved using the param() command.

## Example

```flexscript
If a trigger/field is called with: nodefunction(thefuncnode, passingstr) then within the trigger/field, getting access to
the passingstr string passed as the first additional parameter would be done with:string passedstr = parstr(1);
```

