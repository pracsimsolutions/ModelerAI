---
id: getoutput
name: "getoutput"
kind: command
module: ""
signature: "getoutput(obj involved) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stats.output.value"
description: "Deprecated, use Object.stats.output.value"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getoutput

> ⚠ **DEPRECATED.** Use `Object.stats.output.value` instead.

```flexscript
getoutput(obj involved) -> num
```

Deprecated, use Object.stats.output.value

This command is deprecated. Use Object.stats.output.value instead.

Returns the output statistic of the involved object.

## Example

```flexscript
double outval = getoutput(current.centerObjects[1]);
```

