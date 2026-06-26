---
id: runspeed
name: "runspeed"
kind: command
module: ""
signature: "runspeed(num speed[, num precisionMode]) -> num"
aliases: []
tags: ["command", "group-model-execution"]
deprecated: false
replacedBy: null
description: "Sets the simulation speed"
seeAlso: []
source: "help\\CommandReference\\Commands.xml"
---

# runspeed

```flexscript
runspeed(num speed[, num precisionMode]) -> num
```

Sets the simulation speed

This command sets the speed at which the simulation will run. It is the same as clicking and dragging the
slider bar on the Run Control window, but more precise. The speed that the model will attempt to run at is defined as
the number of simulation time units per real second. The model may not actually achieve this speed if the computer cpu
can not keep up. precisionMode defines the precision to use. If 0, the precision mode will remain what it was before the
call. If you pass RUN_SPEED_LOW_PRECISION, it will set to a low precision run speed. If you pass RUN_SPEED_HIGH_PRECISION,
it will run in high precision mode. This is used for emulation purposes. The precision mode will be reset to low precision
on every reset, so you need to have something in your model set it back on reset or when the model starts to enable high
precision mode.

## Example

```flexscript
runspeed(10); go();
This sets the speed to 10 simulation seconds per real second, then runs the model.
```

