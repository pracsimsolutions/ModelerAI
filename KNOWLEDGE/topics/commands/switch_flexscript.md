---
id: switch_flexscript
name: "switch_flexscript"
kind: command
module: ""
signature: "switch_flexscript(obj object[, num on/off/query]) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: switch_flexscript."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_flexscript

```flexscript
switch_flexscript(obj object[, num on/off/query]) -> num
```

For developer use. This command switches the flexscript toggle of a node.

0 = toggle node non-flexscript
1 = toggle node as flexscript
-1 = query whether node is toggled (1) or not (0)

## Example

```flexscript
switch_flexscript(var_s(current, "code"), 1);
```

