---
id: getinput
name: "getinput"
kind: command
module: ""
signature: "getinput(obj involved) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stats.input.value"
description: "Deprecated, use Object.stats.input.value"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getinput

> ⚠ **DEPRECATED.** Use `Object.stats.input.value` instead.

```flexscript
getinput(obj involved) -> num
```

Deprecated, use Object.stats.input.value

This command is deprecated. Use Object.stats.input.value instead.

Returns the input statistic of the involved object.

## Example

```flexscript
double inval = getinput(current.centerObjects[1]);
```

