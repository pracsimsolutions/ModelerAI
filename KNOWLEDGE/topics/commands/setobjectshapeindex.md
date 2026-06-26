---
id: setobjectshapeindex
name: "setobjectshapeindex"
kind: command
module: ""
signature: "setobjectshapeindex(obj object, num index)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: "View"
description: "Sets the index number for the 3D shape of an object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setobjectshapeindex

```flexscript
setobjectshapeindex(obj object, num index)
```

Sets the index number for the 3D shape of an object

Sets the index number for the 3D shape of an object. For the new index to take effect, the shape node of the object must be removed or renamed, so this command renames the shape node to "_shape" for you. The index number must be a valid index number of a currently imported 3D shape. To view currently imported 3D shapes and to import more shapes, use View -> Media Files. See setframe() as another solution to changing the 3D shape of an object on the fly.

## Example

```flexscript
setobjectshapeindex(item,getshapeindex("fs3d\\Processor.3ds"));
```

