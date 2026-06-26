---
id: reference_tools_editselected_editselected
name: "Edit Selected Objects Tool"
kind: manual
breadcrumb: ["Reference","Tools","EditSelected","EditSelected"]
tags: ["manual","reference","tools","editselected"]
source: "manual/Reference/Tools/EditSelected/EditSelected.html"
---

# Edit Selected Objects Tool

## Overview and Key Concepts

 

You can use the Edit Selected Objects tool to quickly copy properties from one object to several objects at the same time. Be aware that you need to copy properties from objects that are the same kind. For example, you can only copy properties from one processor to another processor. See Copying and Pasting Objects and Properties for more information.

 

This window can be accessed through the Quick Properties menu. It appears when there are selected objects in the view and the user highlights one of those selected objects.

 

It offers several options that are performed on the currently selected set of objects in that view window. To select a group of objects, hold the shift or control key down and drag a selection box around a group of objects. Objects in the selection set will have a red box drawn around them. The view's currently highlighted object (the last object you clicked on) will have a yellow box drawn around it.

 

 

 

## Properties

 

The Edit Selected Objects tool has the following properties:

 

 

### Move into Highlighted

 

This option moves the selected objects (the ones with the red box) into the highlighted object (the one with the yellow box). This allows the highlighted object to be used as a container.

 

### Move into Model

 

This option moves the selected objects (the ones with the red box) into the model directory.

 

### Save to file

 

The selected objects will be saved to a file with a .t extension that can later be reopened in FlexSim. This allows users to save and import parts of models as needed.

 

### Load from file

 

This object loads a .t file into the currently highlighted object. The highlighted object then becomes a container for the imported objects.

 

### Duplicate

 

This option creates an identical copy in the model of the selection set. All port connections are kept intact.

 

### Delete

 

This option deletes the selected objects. This can also be done by highlighting one of the selected objects in a view and pressing the Delete or Backspace key.

 

### Select All

 

This option adds all objects in the model to the selection set.

 

### Deselect All

 

This option takes all objects in the model out of the selection set.

 

### Set Reset Position

 

This option sets the reset position of each object in the selection set to its current position. When the model is reset, all objects with recorded reset positions will be moved, rotated, and sized to that position.

 

### Clear Reset Position

 

This option removes the reset position of each object in the selection set.

 

### Copy All Variables

 

This option will copy the value of all variables the highlighted object has onto each object in the selection set. If a selected object does not have a particular variable, the variable is not created and the object is simply skipped for that variable.

 

### Copy

 

This option will copy the value of the selected variable of the highlighted object has onto each object in the selection set. If a selected object does not have that particular variable, the variable is not created and the object is simply skipped.

 

### Rotate Selected

 

This option will rotate the selection set around the z-axis at a user-defined axis point. Upon clicking this button, the cursor will change and the user will be prompted to pick a point in the model view.

 

### Flip Selected

 

This option will flip the selection set across a line in the x-y plane defined by the user. Upon clicking this button, the cursor will change and the user will be prompted to pick two points in the model view, which become the endpoints of the flip line.

 

### Duplicate HO's Inputs

 

This option destroys all the selected objects' input ports and creates copies of the highlighted object's input port connections to all objects in the selection set.

 

### Duplicate HO's Centers

 

This option destroys all the selected objects' center ports and creates copies of the highlighted object's center port connections to all objects in the selection set.

 

### Duplicate HO's Outputs

 

This option destroys all the selected objects' output ports and creates copies of the highlighted object's output port connections to all objects in the selection set.

 

### Add HO's Inputs

 

This option adds copies of the highlighted object's input port connections to all objects in the selection set.

 

### Add HO's Centers

 

This option adds copies of the highlighted object's center port connections to all objects in the selection set.

 

### Add HO's Outputs

 

This option adds copies of the highlighted object's output port connections to all objects in the selection set.
