---
id: reference_propertiespanels_conveyorpanels_motor_motor
name: "Motor"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","Motor","Motor"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/Motor/Motor.html"
---

# Motor

The Motor panel defines the conveyors the motor powers.

 

 

The following properties are on the Motor panel:

 

### Sync Power and Free Dog Positions

 

This checkbox is for use with power and free conveyor systems. If this box is checked, the motor will ensure that the initial dog positions will be aligned on all of the conveyors connected to it. For example, if the motor's first connected conveyor is 10.2 meters long, and the dog gap is 1.0 meters, then it will set the dog position to start at 0.0 on the first conveyor and at 0.8 on the second connected conveyor (the remaining distance to the next dog after the end of the first conveyor).

 

If this box is unchecked, or if Power and Free conveyors do not have an associated motor, the dog positions will be defined automatically when the first item enters the conveyor.

  

#### Motor Connection Order

 

The order of connected conveyors is important when using Sync Power and Free Dog Positions. As mentioned in the above example, the first dog position on the second connected conveyor will be set to to 0.8. This is the second conveyor connected to the motor (as listed in the Conveyors Listbox of the Motor properties) and not necessarily the conveyor connected to the end of the first conveyor. Make sure that the conveyors are connected to the motor in the same order that the conveyors in the loop are connected or the dog positioning will be incorrect. You can adjust the order of connected conveyors by using the up and down arrows above the list of connected conveyors.

  

### Adjust Dog Gap for Continuous Loop

 

When this box is checked, the motor will assume that the conveyors connected to it represent a looping Power and Free chain. The motor will calculate the total length of the loop based on the combined lengths of the connected Power and Free conveyors and will adjust the dog gap of each conveyor so that the total length of the loop divides evenly by the dog gap. For example, if the total length of a motor's conveyors adds up to 100.2 meters, and the dog gap is 1.0 meters, the motor will adjust the dog gap of each conveyor to be 1.002 meters (100.2 / 100), so that the full loop makes up exactly 100 dogs. The algorithm rounds to the nearest number of dogs. This checkbox is only available if the Sync Power and Free Dog Positions checkbox is also checked.

 

### Conveyors

 

You can use this to add, remove, or reorder the conveyors that are connected to the motor.
