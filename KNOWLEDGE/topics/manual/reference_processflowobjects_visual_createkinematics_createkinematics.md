---
id: reference_processflowobjects_visual_createkinematics_createkinematics
name: "Create Kinematics"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Visual","CreateKinematics","CreateKinematics"]
tags: ["manual","reference","processflowobjects","visual"]
source: "manual/Reference/ProcessFlowObjects/Visual/CreateKinematics/CreateKinematics.html"
---

# Create Kinematics

## Overview and Key Concepts

 

The Create Kinematics activity initializes a kinematics data node for moving objects around in 3D space. See Apply Kinematics for more information.

 

 

 

 

## Connectors

 

The Create Kinematics activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Create Kinematics activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Data Node

 

The **Data Node** property defines a node to store kinematics data on. This can be a token or object label or some other node. This data node will be referenced by the Apply Kinematics and Finish Kinematics activities.

 

### Flags

 

The **Flags** property let's you change the default functionality of the kinematics. The following flags are availalbe:

 

1. **Manage Rotations** - This flag will cause the object to always point in the direction of movement.
1. **Relative Locations** - By default the kinematics coordinates are based on the object's container's coordinate system. This flag will cause the object to travel according to its local coordinate system, based on the rotation of the object at the time you create the kinematics. An example of this would be defining joint movement/rotations of an robot object.
1. **Do Not Prune** - By default kinematics will be "pruned" as they go, meaning when you update the kinematics to a time that is after the end time of a given kinematic, then it will remove that kinematic from the kinematics list. Use this flag to cause kinematics to not be removed.
1. **Reset Buffer** - By default, the kinematic's memory allocation is optimized for speed, i.e. it will only reallocate memory when it needs it, and it will not resize back down when you initialize the kinematic. When this flag is set, the kinematics will resize its data buffer to an initial size when initialized.

 

### Use Object's Position

 

This property specifies how the initial location and rotation of the kinematics will be define. This can be defined by a 3D object or manually. This will be where the 3D object starts when kinematics are applied to the data node.

 

### Object

 

If **Use Object's Position** is selected, this **Object** property will be display. This property is used to reference a 3D object that will define the initial location and rotation of the created kinematics.

 If **Define an Initial Position** is selected, a series of propertis will be display to manually enter each location and rotation component. 

 

### X, Y, Z Location and Rotation

 

If **Define an Initial Position** is selected, these properties will be displayed. These define the initial location and rotation of the created kinematics.
