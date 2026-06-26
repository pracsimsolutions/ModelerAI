---
id: reference_3dobjects_fluid_labels_labels
name: "The Labels Tab"
kind: manual
breadcrumb: ["Reference","3DObjects","Fluid","Labels","Labels"]
tags: ["manual","reference","3dobjects","fluid"]
source: "manual/Reference/3DObjects/Fluid/Labels/Labels.html"
---

# The Labels Tab

## About the Labels Tab

 

Labels are custom variables that you can specify on the object. For example, if you want to keep track of the number of flow items of itemtype 3 that have entered an object, you can use a label to keep a record of this value. For information on using labels, see the [Label Properties](../../../../Reference/CodingInFlexSim/FlexScriptAPIReference/FlexScriptClassReference.html?Class=treenode&Property=labelProperties) documentation.

 

 

The main panel shows a list of the labels on this object.

 

| Icon | Description |
| --- | --- |
|  | Adds a new label with number or string data to the end of the list of labels. |
|  | Duplicates the selected label(s). |
|  | Deletes the selected label(s). |
|  | Moves the selected label(s) up or down in the list. |
|  | Pins the selected label(s) (or all labels if there is no selection) to a Dashboard as either a table of values, bar chart or line graph. Note: Pinning labels with string data will display 0 for its value. |

 

### Label Types

 

When you add a label, you can choose from one of the following kinds of labels:

 

- **Number Label** - a label that stores number data
- **String Label** - a label that stores text data
- **Pointer Label** - a label that stores a reference to another object
- **Array Label** - a label that stores an array of values
- **FlexScript Label** - a label that stores text, and is toggled as FlexScript
- **Bundle Label** - a label that stores a Bundle (a kind of table)
- **Tracked Variable Label** - a label that stores a Tracked Variable.

 

Once you have added a label, you can change its type using the context menu.

 

The way to edit the value of each label depends on the type of label. The following list describes how to edit the label's value, depending on its type:

 

- **Number Label** - Double click on the value, and enter a new value.
- **String Label** - Double click on the value, and enter a new value.
- **Pointer Label** - Click on the value. A Sampler button will appear, allowing you to sample an object, which will be referenced by this label.
- **Array Label** - Double click on the value. This opens a table view of the array. Add elements to the array by adding rows to the table. You can edit each value by double-clicking on the value in the table view. Close the table view when you are finished editing array values.
- **FlexScript Label** - Click on the value. A Code button will appear, allowing you to enter the FlexScript code for this label.
- **Bundle Label** - Double click on the value. This will open a table view, which will allow you to edit the table stored in the bundle.
- **Tracked Variable Label** - Click on the value. An Edit button will appear, which will allow you to configure the Tracked Variable stored on this label.

 

Labels can also be edited in Quick Properties.

 

### Automatically Reset Labels

 

If this option is checked, then the object will automatically reset its labels back to their initial values on reset. When you apply the window, the values shown will be saved as the reset values. The reset values will also be automatically set when directly editing the labels on this tab while the model is reset.
