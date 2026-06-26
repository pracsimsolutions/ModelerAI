---
id: reference_peopleobjects_processflowactivities_subflows_moveequipment_moveequipment
name: "Move Equipment"
kind: manual
breadcrumb: ["Reference","PeopleObjects","ProcessFlowActivities","SubFlows","MoveEquipment","MoveEquipment"]
tags: ["manual","reference","peopleobjects","processflowactivities"]
source: "manual/Reference/PeopleObjects/ProcessFlowActivities/SubFlows/MoveEquipment/MoveEquipment.html"
---

# Move Equipment

## Overview and Key Concepts

 

The Move Equipment activity will cause a staff to move an equipment or transport to a destination.

 

 

The staff will exit their current location, travel to the equipment (or transport), pick it up, and then travel to the destination. If the destination is an object, the staff will drop off the equipment and enter the destination.

 

  

#### Sub Flow Activity

 

The Move Equipment activity is a Sub Flow Activity and its Inner Flow can be viewed if you wish to learn more about how it works, debug its behavior, or copy its logic.

  

 

 

## Connectors

 

The Move Equipment activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Move Equipment activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Equipment

 

The **Equipment** defines the equipment or transport to be moved.

 

### Destination

 

The **Destination** defines the object or position to walk to.

 

### Staff

 

The **Staff** defines staff that will be moving the equipment. If an array of staff is specified, the first will move the equipment while the rest follow along.
