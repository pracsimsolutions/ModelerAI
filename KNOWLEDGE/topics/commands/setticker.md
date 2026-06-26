---
id: setticker
name: "setticker"
kind: command
module: ""
signature: "setticker(node thenode[, num tickspersecond])"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: "setticker"
description: "FlexScript command: setticker."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setticker

```flexscript
setticker(node thenode[, num tickspersecond])
```

For developer use. Set the ticker script node. If tickspersecond is specified, the code will be executed that many times per second, otherwise the code associated with the node will be executed at each iteration of the windows event loop. The node must be toggled FlexScript or C++. No model events are created in association with the ticker node execution. To turn off ticker execution, use setticker(0).

## Example

```flexscript
setticker(so())
```

