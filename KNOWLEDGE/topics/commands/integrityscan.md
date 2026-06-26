---
id: integrityscan
name: "integrityscan"
kind: command
module: ""
signature: "integrityscan(obj class, obj start)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: integrityscan."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# integrityscan

```flexscript
integrityscan(obj class, obj start)
```

For developer use. Checks that attributes have not been deleted from model objects, based on their class object. A search is done starting at the given starting node.

## Example

```flexscript
integrityscan(rank(library(1)),model())
```

