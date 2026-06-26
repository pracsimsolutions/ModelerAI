---
id: switch_showobject
name: "switch_showobject"
kind: command
module: ""
signature: "switch_showobject(obj object[, num off/on]) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Expand/collaps the object data of an object node"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# switch_showobject

```flexscript
switch_showobject(obj object[, num off/on]) -> num
```

Expand/collaps the object data of an object node

Expand or collapse the object data of an object node in a tree view. This is analogous to clicking the > next to the object node in a tree view. A value of 1 is expand, a value of 0 is collapse, a value of -1 gets the current value.

## Example

```flexscript
switch_showobject(so(),1)
```

