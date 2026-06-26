---
id: local
name: "local"
kind: command
module: ""
signature: "local(num index) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "local"
description: "Deprecated, use local variables"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# local

> ⚠ **DEPRECATED.** Use `local` instead.

```flexscript
local(num index) -> node
```

Deprecated, use local variables

This command is deprecated. Use local variables like treenode, int, double, string instead.

Gets a reference to an indexed local variable as a treenode. You must use localarray() to define how many local variables are used and what data type they should have. Can only be used in flexscript.

## Example

```flexscript
setnodenum(local(1),4); Sets the 1st variable in the localarray to 4.
```

