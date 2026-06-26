---
id: groupsetnames
name: "groupsetnames"
kind: command
module: ""
signature: "groupsetnames(node namenode, num startnum, num append)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: groupsetnames."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# groupsetnames

```flexscript
groupsetnames(node namenode, num startnum, num append)
```

For developer use. Sets the names of all of the selected objects in the model to the string data on namenode. If append is 1, then a number is appended to the end of each name, starting at startnum and increasing by 1 for each object.

## Example

```flexscript
groupsetnames("WeldStation",1,1);
```

