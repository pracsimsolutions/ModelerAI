---
id: fpd
name: "fpd"
kind: command
module: ""
signature: "fpd(num value)"
aliases: []
tags: ["command", "group-output"]
deprecated: false
replacedBy: null
description: "Prints an integer value to the open file"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# fpd

```flexscript
fpd(num value)
```

Prints an integer value to the open file

Prints an integer value to the open file. Floating point values will be truncated.

## Example

```flexscript
fpd(3) prints 3 fpd(3.23) prints 3 fpd(3.954) prints 3 fpd(4) prints 4
```

