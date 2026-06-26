---
id: getallocatedfromkey
name: "getallocatedfromkey"
kind: command
module: ""
signature: "getallocatedfromkey(obj coordtasksequence, num key) -> obj"
aliases: []
tags: ["command", "group-taskexecuter-object"]
deprecated: false
replacedBy: null
description: "Gets a reference to the object allocated for the allocate task for the given key"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getallocatedfromkey

```flexscript
getallocatedfromkey(obj coordtasksequence, num key) -> obj
```

Gets a reference to the object allocated for the allocate task for the given key

Returns a reference to the object that has been allocated for the allocate task specified by the given key.

## Example

```flexscript
treenode allocatedobj = getallocatedfromkey(getcoordinatedtasksequence(current.centerObjects[1], 0), mykey);
```

