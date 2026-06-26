---
id: reference_propertiespanels_conveyorpanels_exittransfer_exittransfer
name: "Exit Transfer"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","ExitTransfer","ExitTransfer"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/ExitTransfer/ExitTransfer.html"
---

# Exit Transfer

The Exit Transfer panel defines the position and type of the exit transfer.

 

 

The following properties are on the Exit Transfer panel:

 

### Position X, Y, Z

 

Changes the position of the exit transfer in the 3D model.

 

### Distance Along

 

Defines how far along the decision point's conveyor the decision point is positioned. Changing this value will update the **Position**.

 

### Stop at End

 

If checked, the item will be stopped if it reaches the end of the exit transfer's range before exiting. Then it will wait until it exits. If this is not checked, the item will be "unreleased" at the end of the exit transfer (it will no longer exit through the exit transfer) and move on.

 

### Continuous Pick Prediction

 

If checked, Task Executers who are picking up an item from the exit transfer will re-evaluate the estimated pick up point for the item at defined intervals.

 

### Adjustment Interval

 

If Continuous Pickup Point Prediction is checked, this defines the time interval between pickup point prediction updates.

 

### Ignore Picks if Predicting Late Pickup

 

This is only valid if Use Transport is checked, Stop Item at Transfer End is not checked, and the exit transfer has an expanded range along the conveyor. If checked, Task Executers who predict the pickup point to be after the item has passed the end of the exit transfer will abort their transport task sequence.
