---
id: setframe
name: "setframe"
kind: command
module: ""
signature: "setframe(obj object, num/str toframe)"
aliases: []
tags: ["command", "group-object-data"]
deprecated: false
replacedBy: "of"
description: "Sets the shape frame of an object"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# setframe

```flexscript
setframe(obj object, num/str toframe)
```

Sets the shape frame of an object

Sets the frame attribute of the object to the framenumber. The frame defines which 3D image to display.
This is a nice way of changing the 3D shape of an object on the fly. There are two ways to define frames on an object.
First, you can do it through the naming of various 3D files. For example, if the object was assigned Operator.3ds as
its default shape, and there are additional files in the same directory as
Operator.3ds named OperatorFRAME1.3ds, OperatorFRAME2.3ds and OperatorFRAME3.3ds, then the frame attribute could be
set to 0,1,2, or 3 which would change the displayed shape to each of the 3ds files respectively. Secondly, you can
add sub-nodes to an obejct's shape attribute, each with a path to the 3D file associated with that frame. Subsequently,
frame 1 would be associated with the shape attribute's first sub-node, etc. This is the method used within FlexSim's
environment to define shape frames. If toframe is a string, then the latter method is assumed, and it will find the
shape attribute sub-node with the corresponding name. One use of this command
would be to change the 3d shape in the OnDown trigger to show a "broken" shape.

## Example

```flexscript
if(getframe(current) == 3)) setframe(current, 5);
```

