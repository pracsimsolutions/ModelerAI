---
id: inheritcode
name: "inheritcode"
kind: command
module: ""
signature: "inheritcode()"
aliases: []
tags: ["command", "group-object-functions"]
deprecated: false
replacedBy: null
description: "FlexScript command: inheritcode."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# inheritcode

```flexscript
inheritcode()
```

For developer use. This command is used for subclass function inheritance. Call this in an event function to call that same event function on the object's classobject. In C++ or DLL code, call _inheritcode(callpoint);

## Example

```flexscript
Flexscript:inheritcode();
C++ or DLL:_inheritcode(callpoint);
```

