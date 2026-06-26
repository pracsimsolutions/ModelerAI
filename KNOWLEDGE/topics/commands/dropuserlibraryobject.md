---
id: dropuserlibraryobject
name: "dropuserlibraryobject"
kind: command
module: ""
signature: "dropuserlibraryobject(obj object [, obj ontoobject, num x, num y, num z, obj ontoview, num viewalreadyset]) -> node"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "FlexScript command: dropuserlibraryobject."
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# dropuserlibraryobject

```flexscript
dropuserlibraryobject(obj object [, obj ontoobject, num x, num y, num z, obj ontoview, num viewalreadyset]) -> node
```

For developer use. Drops the specified object as if it were dragged from a user library into the model.

## Example

```flexscript
dropuserlibraryobject(node("MAIN:/project/userlibrary/1/1"));
```

