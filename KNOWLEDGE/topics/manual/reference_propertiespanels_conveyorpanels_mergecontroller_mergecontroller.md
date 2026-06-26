---
id: reference_propertiespanels_conveyorpanels_mergecontroller_mergecontroller
name: "Merge Controller"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","ConveyorPanels","MergeController","MergeController"]
tags: ["manual","reference","propertiespanels","conveyorpanels"]
source: "manual/Reference/PropertiesPanels/ConveyorPanels/MergeController/MergeController.html"
---

# Merge Controller

The Merge Controller panel defines how merges are handled.

 

 

The following properties are on the Merge Controller panel:

 

### Release Strategy

 

Use this box to determine the logic the merge controller will use to determine when it will release slugs.

 

### Edit Lane Clear Table

 

Opens the Lane Clear Table window.

 

 

 

## The Lane Clear Table Window

 

 

The The Lane Clear Table window has the following properties:

 

### Auto-Fill

 

The Lane Clear Table will attempt to calculate the correct release strategy for the lanes based on the position of the conveyors and decision points it is connected to. Press this button to refresh the Lane Clear Table if needed.

 

### Lane Clear Table

 

The X's in the Lane Clear table represent a specific decision point that must be cleared by a slug before this specific lane can be released. You can add and remove X's manually if you would like. Be aware that the Lane Clear Table treats any value that isn't null (empty) the same as an X.
