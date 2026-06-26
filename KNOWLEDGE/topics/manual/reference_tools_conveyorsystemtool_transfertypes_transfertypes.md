---
id: reference_tools_conveyorsystemtool_transfertypes_transfertypes
name: "Transfer Types"
kind: manual
breadcrumb: ["Reference","Tools","ConveyorSystemTool","TransferTypes","TransferTypes"]
tags: ["manual","reference","tools","conveyorsystemtool"]
source: "manual/Reference/Tools/ConveyorSystemTool/TransferTypes/TransferTypes.html"
---

# Transfer Types

## Overview and Key Concepts

 

Transfers are connections between conveyor objects, represented by a white box on a conveyor in the simulation model. Transfer types are global settings that you can import to any transfer that connects one conveyor to another conveyor. Conveyor-to-conveyor transfers are different from entry transfer types or exit transfer types, which are for transfers to non-conveyor objects.

 

Before using the Transfer Type Properties dialog box, you need to be aware of the differences between a Side Transfer and an Inline Transfer:

 

|  | Side Transfer | Inline Transfer |
| --- | --- | --- |
| Default Definition | Any transfer angle that is greater than 45 will be treated like a side transfer by default. | Any angle that is less than 45 will be treated as an inline transfer by default. |
| Item Orientation | The orientation of the flow items will change after the transfer. The item's leading edge in the upstream conveyor becomes the side edge on the downstream conveyor and vice versa. | The orientation of flow items remain the same after the transfer. |
| Gapping | When a conveyor is transferring items from the side of the conveyor, it will add gaps between flow items. The width (transverse size) of an item on the upstream conveyor becomes the length (inline size) of the item on the downstream conveyor. | No gaps will be added between flow items. Items will transfer more smoothly from one conveyor to another because the angle of entry is smaller and more closely matches the direction of the merging conveyor. |
| Movement | While the item is being transferred, its movement is in-line with one of the conveyors, but is stopped relative to the other conveyor. | On an inline transfer, the movement of the item is in-line with both the upstream conveyor and the downstream conveyor throughout the transfer of the item. |

 

You will need to define the angles at which transferring conveyors will be considered an Inline Transfer or a Side Transfer, as discussed below.

 

 

 

## Properties

 

The Transfer Type Properties window has the following properties:

 

 

### Max Angle

 

This setting determines the maximum angle the incoming conveyor must be relative to the receiving conveyor in order to be considered an Inline Transfer. For example, if you enter `45` as the max angle and the incoming conveyor is greater than a 45 degree angle relative to the receiving conveyor, it will act like a Side Transfer rather than an Inline Transfer.

 

### Use Speed of

 

Defines which conveyor's speed the item will take as it crosses the transfer. Options include:

 

- **Receiving Conveyor** - The item will take the speed of the destination conveyor.
- **Sending Conveyor** - The item will take the speed of the origin conveyor.
- **Faster Conveyor** - The item will take the speed of the faster conveyor.
- **Slower Conveyor** - The item will take the speed of the slower conveyor.
- **Conveyor Under Item Center** - The item will take the speed of the origin conveyor for the first half of its length, and then take the speed of the destination conveyor for the second half of its length.

 

### Side Transfer Settings

 

When an item moves over a side transfer, it will go through the following steps.

 

1. If the transfer type has a non-zero **Start Time**, the item will stop and wait for the **Start Time** at the point where the transfer begins.
1. The item will move across the transfer. Use the **Transfer Time** field to define the time it will take. - **Defined Speed** - If selected, the item will use the **Defined Speed** to move across the transfer. - **Defined Time** - If selected, the item will use the **Defined Time** to move across the transfer. - **Motion-Aligned Conveyor Speed** - If selected, the item will take the speed of the conveyor whose motion is aligned with the transfer motion of the item. For a diverting side transfer, this is the speed of the destination conveyor, or the conveyor that the item is being diverted to. For a merging side transfer, it is the speed of the origin conveyor, or the conveyor pushing the item onto the destination merge conveyor.
1. If the transfer type has a non-zero **Finish Time**, the item will stop and wait for the **Finish Time** at the point where the transfer ends.

 

### Pop-Up Distance

 

If non-zero, the item will move this distance vertically during the start and finish times. It will move up for the start time, and back down for the finish time.

 

### Do Smooth Transitions

 

Check this box if you want an item to gradually change its orientation and position so that it matches the angle of the receiving conveyor as it moves across the transfer. This is most applicable for inline transfers that are not exactly inline (they have a transfer angle not equal to zero), for Inline Transfers where the location of the transfer is not in the center of the conveyor, or for Side Transfers where the transfer is not an exact 90-degree transfer.
