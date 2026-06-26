---
id: asciistr
name: "asciistr"
kind: command
module: ""
signature: "asciistr(str string [, num index]) -> num"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "FlexScript command: asciistr."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# asciistr

```flexscript
asciistr(str string [, num index]) -> num
```

This command returns the ascii value of the character at the specified index (0-based) of the string. If index is not specified, the first character of the string is used.

## Example

```flexscript
asciistr("fred",1) returns 114, the ascii value of 'r'
```

