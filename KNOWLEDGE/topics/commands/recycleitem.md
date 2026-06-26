---
id: recycleitem
name: "recycleitem"
kind: command
module: ""
signature: "recycleitem(obj item , num binrank )"
aliases: []
tags: ["command", "group-fixedresources"]
deprecated: false
replacedBy: null
description: "Recycles the flowitem to the flowitem bin"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# recycleitem

```flexscript
recycleitem(obj item , num binrank )
```

Recycles the flowitem to the flowitem bin

Recycles the specified flowitem to the flowitem bin. Binrank should be the rank in the flowitem bin to recycle to (texturedcoloredbox = 5, etc.)

## Example

```flexscript
recycleitem(item, 5);
```

