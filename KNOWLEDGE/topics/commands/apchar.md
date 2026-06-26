---
id: apchar
name: "apchar"
kind: command
module: ""
signature: "apchar(str string) -> str"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: null
description: "Asserts string as a char*"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# apchar

```flexscript
apchar(str string) -> str
```

Asserts string as a char*

Assert type as C style string (e.g. char*). Alias for stringtopchar

## Example

```flexscript
apchar("hello")
```

