---
id: conveyor__motor
name: "Conveyor::Motor"
kind: class
module: "Conveyor"
signature: "Conveyor::Motor"
aliases: []
tags: ["class", "conveyor-module"]
deprecated: false
replacedBy: null
description: "A class that provides properties and methods for controlling a conveyor motor."
seeAlso: []
source: "help\\FlexScriptAPIReference\\Conveyor\\Conveyor.Motor.xml"
---

# Conveyor::Motor

A class that provides properties and methods for controlling a conveyor motor.

## Properties

### speedRatio

```flexscript
double Conveyor::Motor.speedRatio
```

Gets/sets conveyor speeds as a ratio of their default speeds.

The motor's speed ratio allows you to control proportional speeds of the conveyors that are attached
to the motor. Generally you will set this to a value between 0.0 and 1.0, although this range is not
required. The value always resets to 1.0 when the simulation starts. When you set it, it
sets each attached conveyor's targetSpeed property to
that conveyor's default Speed
times the speed ratio.

For example, if you have a motor that is attached to two conveyors with default speeds of 2.0 and 3.0 m/s respectively, then
if you set that motor's speed ratio to 0.7, it will set the first conveyor's speed to 1.4, and the second conveyor's
speed to 2.1.

