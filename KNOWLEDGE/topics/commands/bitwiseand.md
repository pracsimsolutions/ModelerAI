---
id: bitwiseand
name: "bitwiseand"
kind: command
module: ""
signature: "bitwiseand(num a, num b) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated, use & operator"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bitwiseand

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
bitwiseand(num a, num b) -> num
```

Deprecated, use & operator

This command is deprecated. Use & operator instead.

Returns a bitwise AND operation on the two 32 bit integer values.

## Example

```flexscript
int bwa = bitwiseand(0x460, 0x040);This example sets bwa to the value 0x020.
```

