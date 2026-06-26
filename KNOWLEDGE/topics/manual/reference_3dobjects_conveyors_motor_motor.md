---
id: reference_3dobjects_conveyors_motor_motor
name: "Motor"
kind: manual
breadcrumb: ["Reference","3DObjects","Conveyors","Motor","Motor"]
tags: ["manual","reference","3dobjects","conveyors"]
source: "manual/Reference/3DObjects/Conveyors/Motor/Motor.html"
---

# Motor

## Overview and Key Concepts

 

Motors can be used to control whether the conveyor systems are on or off at a given time. The motor can also be used to sync dog gaps on a power and free chain loop when simulating a power and free conveyor system.

 

 

Currently the motor only supports being on or off, and starts the simulation on. To turn the motor off, call [Object.stop()](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object&Method=stop). Turning the motor off will stop all conveyors connected to the motor. To turn the motor back on, call [Object.resume()](../../../CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=Object&Method=resume).

 

 

 

## Events

 

For information on events, see the Event Listening page.

 

The motor does not have any events specific to motor operation.

 

 

 

## States

 

The motor does not implement any states.

 

 

 

## Statistics

 

The motor does not track any statistics.

 

 

 

## Properties Panels

 

The Motor uses the following properties panels:

 

- Statistics
- Visuals
- Labels
- Motor
- Ports
- Triggers

 

 

 

## Properties

 

The Motor object uses the following properties:

 

- CenterObjects
- InObjects
- OutObjects
