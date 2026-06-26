---
id: getstatestr
name: "getstatestr"
kind: command
module: ""
signature: "getstatestr(obj object) -> str"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Object.stats.state"
description: "Deprecated, use Object.stats.state().valueString"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getstatestr

> ⚠ **DEPRECATED.** Use `Object.stats.state` instead.

```flexscript
getstatestr(obj object) -> str
```

Deprecated, use Object.stats.state().valueString

This command is deprecated. Use Object.stats.state().valueString instead.

Returns the state of an object as a text string.

## Example

```flexscript
getstatestr(current);
```

