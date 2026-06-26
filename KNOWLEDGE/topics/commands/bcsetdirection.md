---
id: bcsetdirection
name: "bcsetdirection"
kind: command
module: ""
signature: "bcsetdirection(obj basicconveyor, num direction) -> num"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: null
description: "Deprecated, legacy conveyors are no longer supported"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bcsetdirection

> ⚠ **DEPRECATED.** See description for replacement.

```flexscript
bcsetdirection(obj basicconveyor, num direction) -> num
```

Deprecated, legacy conveyors are no longer supported

Deprecated, Legacy conveyors are no longer supported.
This command is used in conjunction with the BasicConveyor object.

The bcsetdirection command stops all flowitems on the conveyor and sets the direction of travel to 1=forward and 0=reverse. All flowitems will need to have their convey state reset with the bcsetitemconveystate() command before they will begin moving again. The example below shows how to stop the conveyor, and then have all the flowitems accelerate up to a speed of 1 in the reverse direction. Any additional item kinematics will still be active after a direction change.

See bcsetitemconveystate command documentation for a general description of the BasicConveyor object.

## Example

```flexscript
bcsetdirection(so(),0);
for(int i = 1; i <= so().subnodes.length; i++)
{
treenode item = so().subnodes[i];
bcsetitemconveystate(so(), item, bcgetitemposition(so(), item), 0, 1, 0.2);
}
```

