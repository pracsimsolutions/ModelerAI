---
id: dynamicsplineall
name: "dynamicsplineall"
kind: command
module: ""
signature: "dynamicsplineall(node starttree)"
aliases: []
tags: ["command", "group-kinematics-and-splines"]
deprecated: false
replacedBy: null
description: "FlexScript command: dynamicsplineall."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dynamicsplineall

```flexscript
dynamicsplineall(node starttree)
```

For developer use. Un-cache all splines for objects under starttree. Any splines in the model become editable or "unlocked". Spline rendering becomes slower in editable mode. In editable mode, splines can be edited dynamically.

## Example

```flexscript
dynamicsplineall(model())
```

