---
id: reference_propertiespanels_agvpanels_curvedpath_curvedpath
name: "Curved Path"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","AGVPanels","CurvedPath","CurvedPath"]
tags: ["manual","reference","propertiespanels","agvpanels"]
source: "manual/Reference/PropertiesPanels/AGVPanels/CurvedPath/CurvedPath.html"
---

# Curved Path

The Curved Path panel defines the layout and logic of the path.

 

 

The following properties are on the Curved Path panel:

 

### Start and End Position

 

Specify the X, Y, and Z position of the start and end points of the path.

 

### Radius

 

Define the radius of the path. The radius of the path relative to the midpoint of the hypothetical circle around which the path is drawn. Changing the radius will affect the length of the path because it makes this hypothetical circle larger.

 

### Start Angle

 

Rotate the start point of the path to the specified angle. The angle of the start end of the path relative to the simulation model grid. For example, if the start angle is set to 90, the edge of the path's start direction will be perpendicular to the x-axis of the model.

 

### Sweep Angle

 

Define the length of the curved path as an angle. The angle of the end of the path relative to the start angle. For example, if the start angle is set to 45 and the sweep angle is set to 90, the edge of the path's end direction will be 135 degrees offset from the x-axis of the model.

 

### Two Way

 

Allow travelers to move along the path in both directions.

 

### Switch Direction

 

Flip the direction of travel.

 

### Path Class

 

Assign a path class to this path.

 

### Accumulation

 

Assign an Accumulation Type to this path.

 

### AGV Orientation

 

This defines how the AGV must face while on the path, irrespective of the AGV's travel direction. The default is any, meaning the AGV may face any direction while on this path.
