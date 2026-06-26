---
id: reference_processflowobjects_objects_destroyobject_destroyobject
name: "Destroy Object"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","Objects","DestroyObject","DestroyObject"]
tags: ["manual","reference","processflowobjects","objects"]
source: "manual/Reference/ProcessFlowObjects/Objects/DestroyObject/DestroyObject.html"
---

# Destroy Object

## Overview and Key Concepts

 

The Destroy Object activity removes one or more objects in the 3D simulation model.

 

 

 

 

## Connectors

 

The Destroy Object activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Destroy Object activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Object(s)

 

Use the **Object(s)** box to define which objects to destroy.

 

### Asynchronous

 

If checked, there will be a 0 time delay before the object is destroyed. This is important if this activity is triggered within the context of some other model event associated with the object. You don't want to destroy the object before that context finishes.
