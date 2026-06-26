---
id: reference_propertiespanels_dashboardpanels_dashboardcontrol_dashboardcontrol
name: "Dashboard Control"
kind: manual
breadcrumb: ["Reference","PropertiesPanels","DashboardPanels","DashboardControl","DashboardControl"]
tags: ["manual","reference","propertiespanels","dashboardpanels"]
source: "manual/Reference/PropertiesPanels/DashboardPanels/DashboardControl/DashboardControl.html"
---

# Dashboard Control

The Dashboard Control panel contains options for dashboard controls.

 

 

The following properties are on the Dashboard Control panel:

 

### Text

 

The text to display on this control.

 

### Link

 

Specify the path to a node in the model. If the widget is an Edit or Dynamic Text, the Link can also be to a Global Variable.

 

### Hover Hint Text

 

The text that will be displayed when the cursor is hovered over the control.

 

### Image

 

Displays an image instead of text for the control.

 

### Font

 

Specifies the font name, size, properties and color of the control's text

 

### Triggers

 

Some model input objects have triggers that fire, allowing you to execute your own custom code. It has the following properties:

 

- **OnPress** - Fires when a button, checkbox or radio button are pressed.
- **OnApply** - Fires when the enter key is pressed in an edit field and when you click off of an edit field (focus is removed).
- **OnDrag** - Fires as a tracker is clicked/dragged
- **OnSelect** - Fires when an item in the combobox/listbox is selected.

 

### ID

 

A Dashboard Control's ID is a string that allows you to easily reference the control through code or picklist options. Use the getdashboardcontrol() command to get a reference to the model input object (the field, button, etc).
