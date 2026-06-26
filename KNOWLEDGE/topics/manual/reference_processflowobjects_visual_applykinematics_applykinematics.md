---
id: reference_processflowobjects_visual_applykinematics_applykinematics
name: "Apply Kinematics"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Visual","ApplyKinematics","ApplyKinematics"]
tags: ["manual","reference","processflowobjects","visual"]
source: "manual/Reference/ProcessFlowObjects/Visual/ApplyKinematics/ApplyKinematics.html"
---

# Apply Kinematics

## Overview and Key Concepts

 

The Apply Kinematics activity adds and runs kinematics on a kinematics data node created by a Create Kinematics activity. These kinematics are used to move objects in 3D space.

 

 

Objects can perform one or several movement operations at once by specifying speeds and accelerations/decelerations. The following time-based plot shows two kinematics being performed in sequence:

 

 

This time-based plot shows two kinematics that overlap each other:

 

 

There are 5 types of kinematics:

 

1. **Location** - Used to move the location of object in the x, y and z coordinates. Speeds and accelerations/decelerations are defined in model units.
1. **Rotation** - Used to rotate the object along the x, y and z axes. Rotation values are defined in degrees. Speeds are defined as degrees per unit of time. Accelerations/decelerations are defined as degrees per unit of time squared. u
1. **Turn XY** - Used to turn the object a certain angle with a given radius. This turn type will turn the object around the z axis along the XY plane. Units are the same as the Rotation type.
1. **Turn YZ** - Used to turn the object a certain angle with a given radius. This turn type will turn the object around the x axis along the YZ plane. Units are the same as the Rotation type.
1. **Turn ZX** - Used to turn the object a certain angle with a given radius. This turn type will turn the object around the y axis along the ZX plane. Units are the same as the Rotation type.
1. **None** - If none is chosen, no new kinematics will be applied. This will only cause the passed in object to be be updated by the kinematics.

 

 

 

## Connectors

 

The Apply Kinematics activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Apply Kinematics activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Data Node

 

The **Data Node** property needs to point to a node that has kinematics data on it. This data is created by a Create Kinematics activity.

 

### Object(s)

 

The **Object(s)** property specifices which object(s) will be doing the movement. Specify an array of objects to have multiple objects can perform the same movement. activity.

 

### Type

 

The **Type** property specifices what type of kinematics will be added to the data node. See Types above for more information.

 

### Offset X, Y, Z Location

 

The offset location properties specify an offset in the x, y and z locations. This is an offset from the object's current location, not an absolute position.

 

### Offset X, Y, Z Rotation

 

If the **Type** is set to Rotation, then the **Offset Location** properties will appear. These are rotational offset for the x, y and z axes. This is an offset from the object's current rotation, not an absolute rotation. In degrees.

 

### Start Angle X, Y, Z Axis

 

If the **Type** is set to one of the Turn tyes, then one of the Start Angle X, Y, Z Axis properties will appear. The start angle of 0 will use the object's initial direction of motion. In degrees.

 

### Turn Angle X, Y, Z Axis

 

If the **Type** is set to one of the Turn tyes, then one of the Turn Angle X, Y, Z Axis properties will appear. This is the number of degrees to turn about the axis of rotation.

 

### Turn Radius

 

If the **Type** is set to one of the Turn tyes, then the **Turn Radius** property will appear. This is the radius or distance from the center of rotation to turn the object. In length units.

 

### Target Speed

 

The **Target Speed** property specifies the target travel/rotation speed for the object. In length per time.

 

### Acceleration

 

The object's acceleration. In length per time squared.

 

### Deceleration

 

The object's deceleration. In length per time squared.

 

### Start Speed

 

The object's starting speed of the kinematics. In length per time.

 

### End Speed

 

The object's ending speed of the kinematics. In length per time.

 

### Start Time

 

The model start time of the kinematics.

 

### Assign To

 

Assigns the model finish time of the applied kinematics. See the Assign To section of Common Properties for more information.

 

### Wait For Finish

 

If checked the token will remain in the Apply Kinematics activity until the kinematics are finished.
