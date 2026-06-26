---
id: userfunction_n
name: "userfunction_n"
kind: command
module: ""
signature: "userfunction_n(obj object, num index [, num param_a,...])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "nodefunction"
description: "Deprecated, use nodefunction()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# userfunction_n

> ⚠ **DEPRECATED.** Use `nodefunction` instead.

```flexscript
userfunction_n(obj object, num index [, num param_a,...])
```

Deprecated, use nodefunction()

This command is deprecated. Use nodefunction() instead because it will work on any node.
Executes code associated with a node found in the object's userfunctions group having the specified rank index. Use param() within the called function node to retrieve the arguments passed into the function.

## Example

```flexscript
userfunction_s(current,4,1,2,3)

Executes the code associated with a node ranked 4th in the userfunctions group of the current object.
```

