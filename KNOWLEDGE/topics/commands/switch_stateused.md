---
id: switch_stateused
name: "switch_stateused"
kind: command
module: ""
signature: "switch_stateused(obj thenode[, num off/on]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_stateused."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_stateused

```flexscript
switch_stateused(obj thenode[, num off/on]) -> num
```

For developer use. Get or set where a node inside a state profile is used by the object. This flag is automatically set in the setstate() command,
so developers can know which states are used by an object, for UI development.

## Example

```flexscript
switch_stateused(so(),1)
```

