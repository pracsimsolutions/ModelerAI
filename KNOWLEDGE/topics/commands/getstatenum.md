---
id: getstatenum
name: "getstatenum"
kind: command
module: ""
signature: "getstatenum(obj object [, num stateprofilenr]) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stats.state"
description: "Deprecated, use Object.stats.state().value"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstatenum

> ⚠ **DEPRECATED.** Use `Object.stats.state` instead.

```flexscript
getstatenum(obj object [, num stateprofilenr]) -> num
```

Deprecated, use Object.stats.state().value

This command is deprecated. Use Object.stats.state().value instead.

Returns the state of an object in numeric form.

## Example

```flexscript
getstatenum(current);
```

