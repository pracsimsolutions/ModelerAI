---
id: getentrytime
name: "getentrytime"
kind: command
module: ""
signature: "getentrytime(obj object) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the time that a flow item entered the current object it is in"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getentrytime

```flexscript
getentrytime(obj object) -> num
```

Gets the time that a flow item entered the current object it is in

Returns the last entry time of the item (the time the flowitem last entered another object).

## Example

```flexscript
time() - getentrytime(item)
```

