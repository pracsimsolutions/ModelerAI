---
id: getframe
name: "getframe"
kind: command
module: ""
signature: "getframe(obj object) -> num"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: null
description: "Gets the current Shape Frame being drawn for object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# getframe

```flexscript
getframe(obj object) -> num
```

Gets the current Shape Frame being drawn for object

Returns the current 3D shape frame that is being drawn for the object. If the 3D shape assigned to the object was named myshape.3ds, and if two additional 3D shapes with file names myshapeFRAME1.3ds and myshapeFRAME2.3ds resided in the same directory as myshape.3ds, then the two additional files would be loaded into FlexSim automatically at the same time myshape.3ds was loaded, and setting the object's frame from 0 to 1 to 2 would change the object's shape to each of the shapes defined by the three files in order.

## Example

```flexscript
if(getframe(current) == 3)) setframe(current, 5);
```

