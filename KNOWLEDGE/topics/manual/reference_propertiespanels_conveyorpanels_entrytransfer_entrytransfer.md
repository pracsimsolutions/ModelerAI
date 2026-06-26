---
id: reference_propertiespanels_conveyorpanels_entrytransfer_entrytransfer
name: "Entry Transfer"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","EntryTransfer","EntryTransfer"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/EntryTransfer/EntryTransfer.html"
---

# Entry Transfer

The Entry Transfer panel defines the position and type of the entry transfer.

 

 

The following properties are on the Entry Transfer panel:

 

### Position X, Y, Z

 

Changes the position of the entry transfer in the 3D model.

 

### Distance Along

 

Defines how far along the decision point's conveyor the decision point is positioned. Changing this value will update the **Position**.

 

### Entry Orientation

 

Defines how items should be oriented when they enter the conveyor.

 

### Max Transports In

 

Defines the maximum number of items that can be "in transit" to the entry transfer at a time. In other words, it is the max number of task executer requests that can be simultaneously made to transport items into the entry transfer.

 

### Item Insertion Mode

 

Determines how items entering the conveyor through the entry transfer will be inserted onto the conveyor. Options are:

 

- **Simple** - Items must wait until the entry location is clear. Items will be visually placed at the side of the conveyor until the space is available.
- **Block Upstream Items** - If the entry location is blocked when the item attempts to enter, upstream items will be stopped until the space is clear and the item can enter on.
- **Clear Available Space** - If the entry location is blocked when the item attempts to enter, upstream items will be pushed backward until the required space is available. The time to perform the push-back is based on the items being pushed back at twice the conveyor speed. Note that the push-back time is only important if transports are held when space is blocked. In this case, the transports are held for the push-back time, and then they finish the unload. Note also that if this option is chosen, there should be enough "clear" space on the conveyor to push items back. Behavior is undefined if items must be pushed back off the conveyor, or if they are pushed past conveyor transfers. If they are pushed back over decision points or photo eyes, the triggers for those objects will NOT be fired again.

 

### Hold Transports if Space is Blocked

 

If checked, task executers dropping the item off to the entry transfer must wait until the entry point is available before finishing unloading the item onto the conveyor.
