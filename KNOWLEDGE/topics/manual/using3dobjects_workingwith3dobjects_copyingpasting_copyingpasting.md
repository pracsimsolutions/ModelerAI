---
id: using3dobjects_workingwith3dobjects_copyingpasting_copyingpasting
name: "Copying and Pasting 3D Objects and Properties"
kind: manual
breadcrumb: ["Using3DObjects","WorkingWith3DObjects","CopyingPasting","CopyingPasting"]
tags: ["manual","using3dobjects","workingwith3dobjects","copyingpasting"]
source: "manual/Using3DObjects/WorkingWith3DObjects/CopyingPasting/CopyingPasting.html"
---

# Copying and Pasting 3D Objects and Properties

## Overview

 

This topic will explain the basics of how to copy and paste objects. It will also discuss how to use the Edit Selected Objects tool for more advanced copying. The Edit Selected tool makes it possible to copy properties from one object to several other objects at the same time.

 

Additionally, this topic will discuss how to create custom object libraries. Custom object libraries are useful if you have customized the properties of a particular object and you'd like to be able to easily add copies of this custom object directly from the Library.

 

 

 

## Basic Copying and Editing

 

The simplest way to copy and edit an object is to click it, press Ctrl+C to copy that object to the clipboard, then press Ctrl+V to paste that object in the model.

 

As an alternative to using these keyboard shortcuts:

 

1. Right-click the object to open a menu. From the menu, point to **Edit**, then select **Copy** to copy that object to the clipboard.
1. To paste the object, right-click a blank area in the model to open a menu. From the menu, point to **Edit**, then select **Paste**.

  

#### Copying and Pasting Multiple Objects

 

To copy and paste several objects at the same time, select all the objects you want to copy. (See Selecting 3D Objects for tips on selecting multiple objects at a time.) Then use Ctrl+C and Ctrl+V.

  

 

 

## Setting Properties on Multiple Objects

 

### Using the Properties Pane

  

#### Highlighting vs. Selecting

 

Before reading this section, make sure you are familiar with the terms *highlighted* and *selected* as they are used in FlexSim. See Selecting 3D Objects for more information.

  

You can use the Properties Pane to change the properties of all selected objects at the same time:

 

1. Press Shift+click the objects onto which you want to copy the properties. A red box will appear around them to indicate they are selected. See Selecting 3D Objects for more tips on selecting multiple objects.
1. Click one of the selected objects to highlight it.
1. In the Properties pane, edit the property you want to change. When you are finished editing the property, the property will update to be the same for all the selected objects.

 

### Using Property Tables

 

While the Properties pane enables simple editing of multiple objects simultaneously, property tables can be much more powerful because they let you both see and compare the property values of multiple objects, as well as change those properties, in an Excel-like table interface that is intuitive and easy to use. See Property Tables for more information.

 

 

### Setting Properties through code

 

Object properties can be set through code by using the Object.setProperty() method. A list of valid properties for each object can be found on that object's help page. For example, Queue Properties.

 

```
Group queues = Group("Queues"); 
for (int i = 1; i <= queues.length; i++)
	queues[i].setProperty("MaxContent", 5);
```

 

 

 

## Creating Custom Object Libraries

 

Consider using a custom object library if you have customized the properties of a particular object and you'd like to be able to easily add copies of this custom object directly from the Library. See Creating Custom Object Libraries for more information.
