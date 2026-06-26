---
id: workingwithtasks_agvnetworks_addingoffsettravel_addingoffsettravel
name: "Adding AGV Offset Travel"
kind: manual
breadcrumb: ["WorkingWithTasks","AGVNetworks","AddingOffsetTravel","AddingOffsetTravel"]
tags: ["manual","workingwithtasks","agvnetworks","addingoffsettravel"]
source: "manual/WorkingWithTasks/AGVNetworks/AddingOffsetTravel/AddingOffsetTravel.html"
---

# Adding AGV Offset Travel

## Overview and Key Concepts

 

The AGV system enables you to define custom offset travel operations. This is useful especially in warehousing situations, when AGVs may pick and place at many points along a rack. It may be tedious to place a control point at every possible pick/place location along the rack.

 

 

Alternately, if a single rack represents the destination to place an item, even when you do define specific dropoff/pickup slots, the actual bay that an item should be placed in is not always chosen until the AGV arrives at the rack.

 

 

The AGV system lets you dynamically determine offset destinations. As part of an offset travel operation, you can send the AGV to any point on the AGV path system, not just directly to control points.

 

 

 

## Setting Up Offset Travel

 

To setup AGV offset travel:

 

1. Make sure the AGVs you have connected to the AGV network, have their Travel Offsets setting set to *Use navigator for offset travel*.
1. In the *Toolbox*, open the *AGV Network* properties window.
1. Select the *Triggers* tab, then press the Add button and select *On AGV Offset Travel* from the drop-down menu.

 

### Sending to a Dynamic Location on the Path

 

What you do next is dependent on how you want your AGV to perform the offset travel. If you want the AGV to travel to a dynamic location along the destination path (the "Many Possible Locations" scenario described above), then do the following:

 

1. In the *Triggers* tab, under *On AGV Offset Travel*, press the Add button and choose *Send AGV to Closest Point along Control Point Path*.
1. Click off of the popup, then press *OK* to close the AGV Network properties window.

 

This will cause the AGV to travel to an offset distance along the destination control point's path.

 

### Sending to the Closest Control Point

 

If you want the AGV to travel to the control point closest to the offset location (the "Late Slot Assignment" scenario described above), then do the following:

 

1. In the *Triggers* tab, under *On AGV Offset Travel*, press the Add button and choose *Send AGV to Closest Connected Control Point*.
1. Click off of the popup, then press *OK* to close the AGV Network properties window.
1. In the 3D model, add *DropoffPoints* connections to the points that may be dropoff points, and add *PickupPoints* connections to the points that may be pickup points. If a point may be both, connect that point with both connection types.

 

This will cause the AGV to travel to the connected control point that is closest to the target offset location.

 

 

 

## Configuring Travel End Speed

 

When you first set up offset travel, you will notice that AGVs will decelerate to a stop at the destination control point before performing the offset travel operation. If you want them to end their initial travel task at full speed before continuing to offset, see Customizing AGV Travel Tasks > Deceleration and End Speed for more information.
