---
id: reference_processflowobjects_objects_moveobject_moveobject
name: "Move Object"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Objects","MoveObject","MoveObject"]
tags: ["manual","reference","processflowobjects","objects"]
source: "manual/Reference/ProcessFlowObjects/Objects/MoveObject/MoveObject.html"
---

# Move Object

## Overview and Key Concepts

 

The Move Object activity moves an object or multiple objects to another place in a simulation model. When the object moves it will be instantaneous, meaning that it will instantly appear in its new location. It functions the same as calling the `moveobject()` command.

 

  

#### Max Content

 

Moving an object using this activity does not take into account Max Content on a task executer or fixed resource object. It will force the object(s) into the destination object regardless of that object's availability.

   

## Connectors

 

The Move Object activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Move Object activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Object(s)

 

Use the **Object(s)** box to specify the object you want to move. To move multiple objects at once, pass a treenodearray of object references into this property.

 

### Destination

 

Use the **Destination** box to determine where the object or objects should move to. See Create Object - Destination for a list of possibilities.

 

### Preserve Global Position

 

When checked the **Preserve Global Position** check box will preserve the object's global location and rotation. The object will be moved into the destination and then its local location and rotation will be adjusted so its global location and rotation are the same as before the move.
