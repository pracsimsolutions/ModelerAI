---
id: inititem
name: "inititem"
kind: command
module: ""
signature: "inititem(obj item)"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Initializes the FixedResource variables of a flowitem"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inititem

```flexscript
inititem(obj item)
```

Initializes the FixedResource variables of a flowitem

Initializes the FixedResource variables of a flowitem, usually done when the flowitem first enters the fixed resource. This is usually already done for you, but may be used in some exceptional cases when developing your own BasicFR object behavior.

## Example

```flexscript
inititem(item)
```

