---
id: findundorecordbyid
name: "findundorecordbyid"
kind: command
module: ""
signature: "findundorecordbyid(obj view, num id) -> obj"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "FlexScript command: findundorecordbyid."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# findundorecordbyid

```flexscript
findundorecordbyid(obj view, num id) -> obj
```

Returns the undo record with the specified id for the specified view.
See Miscellaneous Concepts > Advanced Undo for more information.

## Example

```flexscript
int id = createundorecord(ownerobject(c), selectedobject(ownerobject(c)), UNDO_CUSTOM);
treenode record = findundorecordbyid(ownerobject(c),id);
```

