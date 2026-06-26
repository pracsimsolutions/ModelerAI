---
id: reference_propertiespanels_windowpanels_treenavigation_treenavigation
name: "Tree Navigation"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","WindowPanels","TreeNavigation","TreeNavigation"]
tags: ["manual","reference","propertiespanels","windowpanels"]
source: "manual/Reference/PropertiesPanels/WindowPanels/TreeNavigation/TreeNavigation.html"
---

# Tree Navigation

The Tree Navigation panel allows you to navigate between sections in the FlexSim Tree and save shortcuts to frequently used parts of the tree for easy reference .

 

 

The following properties are on the Tree Navigation panel:

 

### Views List

 

- **Model** - A subset of the main tree, this contains all of the objects used in the currently open model.
- **Main** - The main tree contains many of the higher level functions in FlexSim.
- **View** - The view tree contains all of the GUIs in the FlexSim interface.

 

| Button | Explanation |
| --- | --- |
|  | Captures the current view and adds that node to the list of preset views. Saving a view allows you to quickly jump back and forth between different sections of the tree. |
|  | Updates the currently selected saved view to point to the current tree location. |
|  | Removes the selected view from the list. |

 

Views that are saved are persistent even after FlexSim closes. Views that are added from the Model get saved into the Tools/TreeNavigation folder and will be available to anyone who loads that model. Views that are added from anywhere else in the main tree or from the view tree are saved in the User Prefs and will be available whenever FlexSim is open under your user. Rename the preset view by entering a new name in the name field.

 

### Name Width

 

Controls the name width of nodes in the tree.
