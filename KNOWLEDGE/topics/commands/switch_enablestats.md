---
id: switch_enablestats
name: "switch_enablestats"
kind: command
module: ""
signature: "switch_enablestats(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Get or set the \"stats\" flag of the object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_enablestats

```flexscript
switch_enablestats(obj object[, num off/on]) -> num
```

Get or set the "stats" flag of the object

Get or set an object's "stats" flag. A value of 1 is enable, a value of 0 is disable, and a value of -1 gets the current value.

## Example

```flexscript
switch_enablestats(so(),1)
```

