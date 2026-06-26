---
id: iterate
name: "iterate"
kind: command
module: ""
signature: "iterate(num start, num end, num step) {}"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "for"
description: "Deprecated, use for() or while() commands"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# iterate

> ⚠ **DEPRECATED.** Use `for` instead.

```flexscript
iterate(num start, num end, num step) {}
```

Deprecated, use for() or while() commands

This command is deprecated. Use for() or while() commands instead.

Repeatedly execute the following block {} with counter value traversing from start to end being incremented by step. The count command can be used to access the current counter value on each iteration. The return value of count will depend on which iteration is currently being executed.

## Example

```flexscript
iterate(1,9,2){ pd(count);pr(); }
Prints the following numbers to the outputconsole: 1,3,5,7,9
```

