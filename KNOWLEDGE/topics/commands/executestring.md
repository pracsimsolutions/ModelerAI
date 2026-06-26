---
id: executestring
name: "executestring"
kind: command
module: ""
signature: "executestring(str thestring[, node c, node i, obj eventdata])"
aliases: []
tags: ["command", "group-strings"]
deprecated: false
replacedBy: null
description: "Executes a string as flexscript"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# executestring

```flexscript
executestring(str thestring[, node c, node i, obj eventdata])
```

Executes a string as flexscript

Executes the given string as flexscript. Parameters 2-4 are the same as executefsnode.

## Example

```flexscript
executestring("profileevents()");
```

