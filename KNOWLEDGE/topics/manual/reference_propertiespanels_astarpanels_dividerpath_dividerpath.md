---
id: reference_propertiespanels_astarpanels_dividerpath_dividerpath
name: "Divider / Path"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","AStarPanels","DividerPath","DividerPath"]
tags: ["manual","reference","propertiespanels","astarpanels"]
source: "manual/Reference/PropertiesPanels/AStarPanels/DividerPath/DividerPath.html"
---

# Divider / Path

The Divider / Path panel defines layout and logic of the divider, path, or bridge.

 

 

The following properties are on the Divider / Path panel:

 

### Conditional Rule

 

Define the condition that is required for the divider to be applied. When a divider is conditional, its condition is reevaluated at the beginning of every travel task. If the condition returns false (0), then the divider will not be applied in the route calculation algorithm, and the traveler will travel as if the divider did not exist. Conditional rules let you define certain dividers as only applicable for certain travelers, or based on certain model conditions.

 

You can add conditional rules through the A* Properties window.

 

### Two Way

 

Check this box to make the divider/path apply in two directions instead of just one.

 

### Reverse

 

| Icon | Description |
| --- | --- |
|  | Reverse the direction of the divider/path. |

 

### Points (Dividers/One-Way Dividers/Preferred Paths)

 

Define x,y positions of each point.

 

| Icon | Description |
| --- | --- |
|  | Add a point to the end. |
|  | Remove the selected point. |
|  | Reorder the selected point up or down in the list. |

 

### Path Weight

 

Preferred Paths only. This is the path weight added to the nodes associated with the path. Valid values are numbers between 0 and 1. However, setting the value to 0 will cause the path to use the Default Preferred Path Weight.

 

### Virtual Distance

 

Bridges only. When checked, this is the distance that will use for traversing the Bridge instead of the actual distance.
