---
id: tonum
name: "tonum"
kind: command
module: ""
signature: "tonum(thing) -> num"
aliases: []
tags: ["command", "group-conversions"]
deprecated: false
replacedBy: null
description: "Assert type as a number"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# tonum

```flexscript
tonum(thing) -> num
```

Assert type as a number

Assert type as an arithmetic number. Only serves a real purpose in C++.

## Example

```flexscript
return tonum(current);
This returns a pointer to the current object, casting it as a number.
```

