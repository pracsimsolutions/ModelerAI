---
id: toptr
name: "toptr"
kind: command
module: ""
signature: "toptr(thing)"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: "in"
description: "FlexScript command: toptr."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# toptr

```flexscript
toptr(thing)
```

For developer use. Assert type as pointer. Assert that the argument thing is of type void*. Only serves a real purpose in C++, but is safe to use in flexscript.

## Example

```flexscript
toptr(so())
```

