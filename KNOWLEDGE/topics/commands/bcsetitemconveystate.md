---
id: bcsetitemconveystate
name: "bcsetitemconveystate"
kind: command
module: ""
signature: "bcsetitemconveystate(obj basicconveyor, obj item, num startpoint, num startspeed, num targetspeed, num accdec)"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "the"
description: "Deprecated, legacy conveyors are no longer supported"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# bcsetitemconveystate

> ⚠ **DEPRECATED.** Use `the` instead.

```flexscript
bcsetitemconveystate(obj basicconveyor, obj item, num startpoint, num startspeed, num targetspeed, num accdec)
```

Deprecated, legacy conveyors are no longer supported

Deprecated, Legacy conveyors are no longer supported.
This command is used in conjunction with the BasicConveyor object.

The BasicConveyor object allows flowitems to be moved in and out of the conveyor at any time and at any location. Each flowitem on the
BasicConveyor can be assigned and re-assigned its own kinematic profile defining how the flowitem is to move along the conveyor. When one
flowitem overtakes another flowitem on the conveyor which is either stopped or traveling at a slower speed, it will slow down immediately
to match that flowitem's speed. Although flowitems may travel in either direction along the conveyor, all flowitems will travel in the same
direction at any given time. The travel direction of the conveyor is set with the bcsetdirection() command. As flowitems are moved in and
out of the conveyor, other flowitems on the conveyor are aware of the increase or decrease in available space along the conveyor, and will
accumulate naturally. After a flowitem enters the conveyor, either indirectly with the receiveitem() command or directly with the
moveobject() command; it is necessary to define the flowitem's initial kinematic profile. This profile is refered to as the flowitem's
"convey state", and it is set with the bcsetitemconveystate() command. The convey state for a flowitem can be set multiple times if
desired. The following are definitions for the parameters used to define the convey state of a flowitem:

basicconveyor = A reference to the conveyor.

item = A reference to the flowitem.

startpoint = The starting location for the flowitem as measured from the beginning of the conveyor, along the conveyor's length to the flowitem's edge furthest from the head of the conveyor (the leading edge if the conveyor's direction is forward, the trailing edge if the conveyor's direction is backward).

startspeed = The initial speed the flowitem will begin traveling at.

targetspeed = The target speed the flowitem will try to achieve. If the targetspeed is greater than the startspeed, then the flowitem will accelerate up to the target speed; otherwise it will decelerate down to the target speed. If both the startspeed and the targetspeed are set to 0, the flowitem will be positioned at the startpoint, and will not move.

accdec = The acceleration or deceleration used to achieve the target speed over time. A value of 0 indicates infinite acceleration/deceleration; or in other words the flowitem will reach it's targetspeed immediately.

No destination or termination speed is required, because the flowitems continue traveling along the conveyor until they either reach the end of the conveyor, run into another flowitem, or their convey state is changed again. The BasicConveyor allows multiple "Decision Points" to be defined along its length. These trigger points are convenient locations to update the convey state of a flowitem. In addition to the main convey state kinematic assigned to the flowitem with the bcsetitemconveystate() command, a flowitem has two other kinematic structures associated with it that may be used to apply custom kinematic motion (translation or rotation) in addition to the travel motion along the conveyor. One of the kinematic structures is based on simulated time units, and the other is based on distance units along the length of the conveyor. To access these kinematic structures, use the bcgetitemkinematics() command, then use the initkinematics() and addkinematic() commands to create the custom kinematic motion desired. See bcgetitemkinematics() command documentation for more information.

## Example

```flexscript
bcsetitemconveystate(current, item, position, 0, 60, 0);
```

