---
id: createglobaltasksequence
name: "createglobaltasksequence"
kind: command
module: ""
signature: "createglobaltasksequence(str name, obj dispatcher [, obj dp1, obj dp2, obj dp3, obj dp4, obj dp5]) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Process"
description: "Deprecated, use Process Flow instead"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# createglobaltasksequence

> ⚠ **DEPRECATED.** Use `Process` instead.

```flexscript
createglobaltasksequence(str name, obj dispatcher [, obj dp1, obj dp2, obj dp3, obj dp4, obj dp5]) -> node
```

Deprecated, use Process Flow instead

This command is deprecated. Use Process Flow instead.

Creates an instance of the named global task sequence with the given dynamic parameters 1-5 and
dispatches it to the specified dispatcher. Returns a reference to the created global task sequence. Note that
this does not dispatch the task sequence. You need to dispatch it yourself using dispatchtasksequence().

## Example

```flexscript
treenode ts = createglobaltasksequence("GlobalTS", current.centerObjects[1])
dispatchtasksequence(ts)
```

