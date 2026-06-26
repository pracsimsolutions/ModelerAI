---
id: reference_propertiespanels_visualpanels_stage_stage
name: "Stage"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","VisualPanels","Stage","Stage"]
tags: ["manual","reference","propertiespanels","visualpanels"]
source: "manual/Reference/PropertiesPanels/VisualPanels/Stage/Stage.html"
---

# Stage

The Stage panel displays options for working with USD stages.

 

 

The following properties are on the Stage panel:

 

### Path

 

Browse to a usd file on disk or enter a destination path on an Omniverse Nucleus server.

 

e.g., omniverse://127.0.0.1/Users/UserName/Folder/FileName.usd

 

### Save

 

Save any modifications to the stage.

 

### Export As...

 

Export the stage to a new usd file.

 

### Reload

 

Reload the usd stage from its source file(s). This will overwrite any unsaved, local modifications to the stage and retrieve any changes made by other applications since the stage was loaded.

 

Note: loaded textures are stored in FlexSim's media list, so if any textures used by the stage are unloaded from the media list, reloading the usd stage data will reload the textures into the media list again.

 

### Draw Plane

 

To improve selecting and dropping objects onto a stage, a simple plane may be optionally drawn at the bottom of this object. Clear this box to stop drawing that plane.

 

**Factor**. Define the polygon offset factor to use when drawing the plane.

 

**Units**. Define the polygon offset units to use when drawing the plane.

 

### Draw Axis Lines

 

A usd stage has defined coordinate axes and measurement units. Unless otherwise specified, a usd stage defaults to having a Y up axis and centimeter units. When this option is checked, axis lines will be drawn to show the scene's origin and direction. The red axis is X, green is Y, and blue is Z. Where the three axis lines cross is the origin of the usd stage.

  

#### USD Stage Size and Location

 

A usd stage's defined measurement units and the FlexSim model's length units are necessary for many of the advanced workflows of the USD Stage object.

 

The USD Stage object cannot be arbitrarily scaled. Its prims will be rendered according to the actual size defined within the stage, based on the stage's units and the model's units. The FlexSim stage object's size does not affect how large the scene is drawn; it is drawn exactly the size defined in the stage.

 

The USD Stage object's location will be the stage's origin point. If prims are defined far away from the stage's origin, then they will also be far away from the object's location in the model.

 

If you want to adjust the scale of an imported usd shape, consider importing the usd shape on a regular object instead of using a USD Stage object. Alternatively, adjust the transforms of individual prims within the stage itself.

  

### Select Prims

 

By default, the USD Stage object is treated as a single object. Check this box to be able to click individual prims drawn in the 3D view. Clear this box to interact with the stage itself as a single object.
