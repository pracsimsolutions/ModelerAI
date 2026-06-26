---
id: hideports
name: "hideports"
kind: command
module: ""
signature: "hideports(obj object[, num off/on])"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "switch_hideconnectors"
description: "Deprecated, use switch_hideconnectors()"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# hideports

> ⚠ **DEPRECATED.** Use `switch_hideconnectors` instead.

```flexscript
hideports(obj object[, num off/on])
```

Deprecated, use switch_hideconnectors()

This command is deprecated. Use switch_hideconnectors() instead.

This command sets the specified object to show or hide its ports and connections. A value of 1 is show and a value of 0 is hide. If the ports and connections are hidden, they will not be visible in any view window.

## Example

```flexscript
hideports(current,1);
This sets the current object to show its ports and connections in all view windows.
```

