---
id: reference_propertiespanels_visualpanels_prim_prim
name: "Prim"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","VisualPanels","Prim","Prim"]
tags: ["manual","reference","propertiespanels","visualpanels"]
source: "manual/Reference/PropertiesPanels/VisualPanels/Prim/Prim.html"
---

# Prim

The Prim panel displays options for working with prims within USD stages.

 

To select individual prims, you must first check the Select Prims box on the Stage Panel. Clicking on a prim in the 3D view will then show this panel.

 

 

The following properties are on the Prim panel:

 

### Name

 

The prim's type and name as shown at the top of the panel. The name can be modified.

 

### Prim Path

 

The prim's path in the usd hierarchy. To view the hierarchy in a tree view, press the tree button.

 

### Transform

 

The prim's location, rotation, and size can be seen and modified.

  

#### USD xforms

 

A usd prim's transform is defined by a series of xform operations in a particular order. FlexSim reads those transformations as a single 4x4 transformation matrix, which it then decomposes into location, rotation, and size vectors. This may not work perfectly for all prims, depending on their configuration.
