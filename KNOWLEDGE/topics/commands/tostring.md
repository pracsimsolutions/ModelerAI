---
id: tostring
name: "tostring"
kind: command
module: ""
signature: "tostring(thing) -> str"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: "in"
description: "FlexScript command: tostring."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# tostring

```flexscript
tostring(thing) -> str
```

For developer use. Assert as pchar string. Assert that the number val is of type char*. Only serves a real purpose in C++, but is safe to use in Flexscript.

## Example

```flexscript
tostring(eventdata)
```

