---
id: changeconveyorspeed
name: "changeconveyorspeed"
kind: command
module: ""
signature: "changeconveyorspeed(obj conveyor, num newspeed )"
aliases: []
tags: ["command", "deprecated", "group-deprecated"]
deprecated: true
replacedBy: "Conveyor.targetSpeed"
description: "Deprecated, use conveyor.targetSpeed"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# changeconveyorspeed

> ⚠ **DEPRECATED.** Use `Conveyor.targetSpeed` instead.

```flexscript
changeconveyorspeed(obj conveyor, num newspeed )
```

Deprecated, use conveyor.targetSpeed

This command is deprecated. Legacy conveyors are no longer supported. Use Conveyor.targetSpeed instead.

Changes the speed of the conveyor to newspeed.

## Example

```flexscript
changeconveyorspeed(current, 4); This example changes the conveyor's speed to 4.
```

