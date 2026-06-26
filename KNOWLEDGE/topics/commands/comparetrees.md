---
id: comparetrees
name: "comparetrees"
kind: command
module: ""
signature: "comparetrees(obj tree1, obj tree2) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "FlexScript command: comparetrees."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# comparetrees

```flexscript
comparetrees(obj tree1, obj tree2) -> num
```

For developer use. Test trees for equality.

## Example

```flexscript
comparetrees(node("MAIN:/project/model/1"),node("MAIN:/project/model/2"))
```

