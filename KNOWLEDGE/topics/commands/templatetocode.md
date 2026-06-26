---
id: templatetocode
name: "templatetocode"
kind: command
module: ""
signature: "templatetocode(obj code, obj edit)"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: templatetocode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# templatetocode

```flexscript
templatetocode(obj code, obj edit)
```

For developer use. Apply template code to c++. This is used in a picklist's template code drop-down to apply edited template code back to the C++/Flexscript code that created it.

## Example

```flexscript
templatetocode(so(),sv())
```

