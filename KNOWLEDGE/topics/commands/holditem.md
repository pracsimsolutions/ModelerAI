---
id: holditem
name: "holditem"
kind: command
module: ""
signature: "holditem(obj item)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Holds the item if it has already been released"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# holditem

```flexscript
holditem(obj item)
```

Holds the item if it has already been released

Holds the specified item if it has already been released. This in effect "unreleases" the item so that it will not leave. This only applies if the item is in a FixedResource and it has already been released and has not been "allocated" to a downstream object (it is not waiting for a transport).

## Example

```flexscript
holditem(item)
```

