---
id: userfunction_s
name: "userfunction_s"
kind: command
module: ""
signature: "userfunction_s(obj object, str name [, num param_a,...])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "nodefunction"
description: "Deprecated, use nodefunction()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# userfunction_s

> ⚠ **DEPRECATED.** Use `nodefunction` instead.

```flexscript
userfunction_s(obj object, str name [, num param_a,...])
```

Deprecated, use nodefunction()

This command is deprecated. Use nodefunction() instead, because it will work on any node.
Executes code associated with a node found in the object's userfunctions group having the specified name. Use param() within the called function node to retrieve the arguments passed into the function.

## Example

```flexscript
userfunction_s(current,"myfunc",1,2,3)

Executes the code associated with a node named "myfunc" in the userfunctions group of the current object.
```

