---
id: getvarnode
name: "getvarnode"
kind: command
module: ""
signature: "getvarnode(obj object, str/num var) -> node"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets a node reference to an object's variable"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getvarnode

```flexscript
getvarnode(obj object, str/num var) -> node
```

Gets a node reference to an object's variable

Returns a reference to the specified variable's node in the specified object.

## Example

```flexscript
treenode schedule = getvarnode(current,"schedule");
```

