---
id: standardpreopen
name: "standardpreopen"
kind: command
module: ""
signature: "standardpreopen(obj object[, int nobuild])"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: standardpreopen."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# standardpreopen

```flexscript
standardpreopen(obj object[, int nobuild])
```

For developer use. Queries if there are any open windows that are the same as the current window being opened. If so, then the current window is closed.

## Example

```flexscript
standardpreopen(c)
```

