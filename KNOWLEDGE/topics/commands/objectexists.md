---
id: objectexists
name: "objectexists"
kind: command
module: ""
signature: "objectexists(obj object) -> num"
aliases: []
tags: ["command", "group-tree-node-data"]
deprecated: false
replacedBy: null
description: "Check if an object exists"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# objectexists

```flexscript
objectexists(obj object) -> num
```

Check if an object exists

If you're trying to test if an object exists when using dot syntax you can omit the objectexists. For example, if (current.labels["MyLabel"]) or if (current.next).

This command returns true (1) if the object specified as object exists in the tree. If the object does not exist, this command returns false (0).

## Example

```flexscript
if (objectexists(next(current))) { moveobject(item, next(current), 1); }
This checks to see if there is an object in the tree after current. If there is, the object referenced by item is moved into it through port number 1.
```

