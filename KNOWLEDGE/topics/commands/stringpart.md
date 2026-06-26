---
id: stringpart
name: "stringpart"
kind: command
module: ""
signature: "stringpart(str string, num start, num length) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "string.substr"
description: "Deprecated, use string.substr()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# stringpart

> ⚠ **DEPRECATED.** Use `string.substr` instead.

```flexscript
stringpart(str string, num start, num length) -> str
```

Deprecated, use string.substr()

This command is deprecated. Use string.substr() instead.

This command will return a portion of a string. The new string is defined with a starting character number and a length. A start value of 0 refers to the first character in the original string.

## Example

```flexscript
stringpart("Hello",2,3) returns the sub string "llo".
```

