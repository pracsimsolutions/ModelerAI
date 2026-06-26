---
id: insertcopy
name: "insertcopy"
kind: command
module: ""
signature: "insertcopy(obj originalobject, obj containerobject)"
aliases: []
tags: ["command", "group-miscellaneous"]
deprecated: false
replacedBy: null
description: "Creates a copy of the flowitem and moves it into the container object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# insertcopy

```flexscript
insertcopy(obj originalobject, obj containerobject)
```

Creates a copy of the flowitem and moves it into the container object

Creates a copy of the flowitem and moves it into the container object, causing the OnReceive event of the container to execute.

## Example

```flexscript
insertcopy(item, current);
Creates a duplicate of the flowitem and moves it into the current object.
```

