---
id: viewdist
name: "viewdist"
kind: command
module: ""
signature: "viewdist(obj view, obj object) -> num"
aliases: []
tags: ["command", "group-user-interface"]
deprecated: false
replacedBy: null
description: "Gets the distance from the focus point of view to object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# viewdist

```flexscript
viewdist(obj view, obj object) -> num
```

Gets the distance from the focus point of view to object

Returns the distance from the focus point of the view to object.

## Example

```flexscript
if(viewdist(view, current) > 100) ;
This command queries whether the distance between the point that the view is looking at and the location of the current object is greater than 100.
```

