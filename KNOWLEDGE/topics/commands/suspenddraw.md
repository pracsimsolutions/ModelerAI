---
id: suspenddraw
name: "suspenddraw"
kind: command
module: ""
signature: "suspenddraw(num bitmask) -> num"
aliases: []
tags: ["command", "group-development"]
deprecated: false
replacedBy: null
description: "FlexScript command: suspenddraw."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# suspenddraw

```flexscript
suspenddraw(num bitmask) -> num
```

For developer use. Bit mask. Disables draw in window types. Bits: 0 - TREE; 1 - PLANAR; 2 - SPATIAL

## Example

```flexscript
suspenddraw(1)
```

