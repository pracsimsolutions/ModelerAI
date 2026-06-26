---
id: reference_processflowobjects_visual_finishkinematics_finishkinematics
name: "Finish Kinematics"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Visual","FinishKinematics","FinishKinematics"]
tags: ["manual","reference","processflowobjects","visual"]
source: "manual/Reference/ProcessFlowObjects/Visual/FinishKinematics/FinishKinematics.html"
---

# Finish Kinematics

## Overview and Key Concepts

 

The Finish Kinematics activity is used to remove object's from kinematics and deactivates kinematics nodes from being updated.

 

A kinematics data node can reference multiple 3D objects in order to update their location and rotation. You can remove one or all of the 3D objects to stop their updates. If any 3D objects are still using the kinematics data node then the data node will remain active. When the model is reset all data nodes are deactivated.

 

  

#### Finshing Kinematics

 

Once kinematics are created and applied, the 3D object's position/rotation are updated continuously as the 3D view is drawn. In order to remove this update at the end of a set of kinematics, the kinematics node must be finished.

   

#### Updating Kinematics

 

If a 3D object is being updated by kinematics, you will not be able to drag the object in the 3D view.

  

 

 

## Connectors

 

The Finish Kinematics activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Finish Kinematics activity:

 

 

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

 

The **Object(s)** property specifices which 3D object(s) will be doing the movement. Specify an array of objects to have multiple 3D objects can perform the same movement. activity.
