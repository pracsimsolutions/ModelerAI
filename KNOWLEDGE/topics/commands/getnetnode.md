---
id: getnetnode
name: "getnetnode"
kind: command
module: ""
signature: "getnetnode(obj object, num nodenr) -> obj"
aliases: []
tags: ["command", "group-networknodes"]
deprecated: false
replacedBy: null
description: "Gets the nth network node connected to object with an A connection line"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getnetnode

```flexscript
getnetnode(obj object, num nodenr) -> obj
```

Gets the nth network node connected to object with an A connection line

Returns the nth network node connected to the object with a "blue" or "station" connection line

## Example

```flexscript
getnetnode(current, 1)
```

