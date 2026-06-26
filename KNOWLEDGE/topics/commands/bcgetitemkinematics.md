---
id: bcgetitemkinematics
name: "bcgetitemkinematics"
kind: command
module: ""
signature: "bcgetitemkinematics(obj basicconveyor, obj item, num kinematictype {0-main kinematic : 1-time dependent : 2-convey dependent}) -> node"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "bcgetitemkinematics"
description: "Deprecated, legacy conveyors are no longer supported"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bcgetitemkinematics

> ⚠ **DEPRECATED.** Use `bcgetitemkinematics` instead.

```flexscript
bcgetitemkinematics(obj basicconveyor, obj item, num kinematictype {0-main kinematic : 1-time dependent : 2-convey dependent}) -> node
```

Deprecated, legacy conveyors are no longer supported

Deprecated, Legacy conveyors are no longer supported.
This command is used in conjunction with the BasicConveyor object.
The bcgetitemkinematics command is used to obtain a reference to one of the three kinematics associated with a flowitem traveling on a BasicConveyor:

0 = The main convey state kinematics. You may get kinematic information from this, but do not change the kinematic by adding kinematics or initializing it.

1 = A user-definable kinematics that is dependent on simulation time.

2 = A user-definable kinematics that is dependent on the convey position of the flowitem on the conveyor.

The two user-definable kinematics are available for the user to set up customized kinematic motion in addition to traveling down the length of the conveyor. Use bcgetitemkinematics to get a reference to the desired kinematics node, and then use standard initkinematics() and addkinematic() commands as normal. You will need to use the alternative initkinematics parameter list: initkinematics(node, x, y, z, rx, ry, rz, managerots, localcoords). Also, if you add kinematics to the type 2 convey depend kinematic node, bear in mind that all time units for that kinematic no longer apply to actual simulation time units. Instead, they are interpretted as "convey units". For example, a speed of 5 is no longer 5 units per time unit, but is instead 5 units per conveyed unit. Acceleration is not distance per time squared, but distance per conveyed unit squared. Start times are no longer a time to start the kinematic, but a position along the conveyor to start the kinematic at. As an example, let's say you add a kinematic to the convey dependent kinematic node to travel 1 unit in the y direction with "start time" 5 and "max speed" 0.5. What that translates to for the convey dependent kinematic is that the item will start traveling in the y direction once it reaches 5 units along the conveyor. Also, for every unit that it travels down the length of the conveyor, it will travel 0.5 units in the y direction (0.5 y units per convey unit). Thus it will finish the y kinematic once it has travelled 7 units down the conveyor. If the item is blocked in the middle because of accumulation, say at convey position 6, then its y kinematic will also stop, and not resume until the item resumes along the conveyor.

See bcsetitemconveystate command documentation for a general description of the BasicConveyor object.

## Example

```flexscript
treenode kin = bcgetitemkinematics(current, item, 2);
treenode kin2 = bcgetitemkinematics(current, item, 1);

initkinematics(kin, 0,0,0, 0,0,0, 0,0);
addkinematic(kin, 0,1,1,0.5,1,1,0,0, 5, KINEMATIC_TRAVEL);

initkinematics(kin2, 0,0,0, 0,0,0, 0,0);
addkinematic(kin2, 0,0,720,360,0,0,0,0, time()+5.0, KINEMATIC_ROTATE);
```

