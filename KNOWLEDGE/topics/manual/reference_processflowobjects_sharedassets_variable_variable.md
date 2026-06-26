---
id: reference_processflowobjects_sharedassets_variable_variable
name: "Variable"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","Variable","Variable"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/Variable/Variable.html"
---

# Variable

## Overview and Key Concepts

 

This shared asset allows you to store any kind of data and then read or change that data.

 

 

The Variable is unlike the other shared assets in that it isn't a limited asset. Tokens can read and set a Variable freely without any delay or interruption. The Variable can be used just like a label on a token or other object but now you have a central reference to that value that can be easily accessed throughout your process flow. Additionally you get the added feature of being able to listen to the Variable being changed or even when it is read.

 

A Variable is changed by a Set Variable activity and read by a Get Variable activity.

 

### Variable Values

 

The Variable is very flexible in what it can point at or what its value can be. Here are some possibilities:

 

- **Number** - If a number is entered into the Variable or Value box, this will be the initial value of the variable on reset. The value of the Variable is stored internally. You can change the value to another number or even change the value to a string, object, array etc.
- **String** - Behaves the same as a Number.
- **Array** - Behaves the same as a Number.
- **Object** - Behaves the same as a Number.
- **Node** - If the Variable or Value box references a node with data on it, then the value will be the value of the node. When you set the Variable, the value of the node will be set. As with the number, you can set the Variable to a number, string, object etc. and then value of the node will reflect the new value.
- **Tracked Variable** - You can reference a Global Tracked Variable, a tracked variable stored on a node or table cell or you can create an Internal Tracked Variable. Internal Tracked Variables that have their **Type** set to *Local* will create a variable for each instance of the process flow.
- **Table Cell** - Behaves the same as a node. For example: `Table("GlobalTable1").cell(1, 1)`
- **Table Value** - If you get the value from a table using `Table("GlobalTable1")[1][1]` then the Variable will use this as the initial value and store it internally. This will behave just like a Number. Setting the Variable will only set the internal value and will not alter the value in the table.
- **Global Variable** - You can reference a Global Variable by using the and sampling the global variable from the Toolbox. This will give a reference to the Global Variable's definition in the MODEL:/Tools/GlobalVariables folder. Getting the value of the Variable will get the current value of the global variable. Setting the value will set the Global Variable. NOTE: Using a Wait for Event or Event-Triggered Source on a Variable that is referencing a Global Variable will not allow you to listen to the Global Variable being set using code. It will only listen to changes that occur using the Set Variable activity.

  

#### Listening for Changes

 

If you are using a Wait for Event or Event-Triggered Source to listen for changes to a Variable, the Variable must be either set using a Set Variable activity, or the node that is being referenced must have Tracked Variable data. For example, if the Variable or Value box contained `Table("GlobalTable1").cell(1, 1)` and cell 1,1 had Tracked Variable data, then setting the value of that cell using `Table("GlobalTable1")[1][1] = 4;` anywhere in your model will trigger the Wait for Event or Event-Triggered Source.

  

 

 

## Connectors

 

Only Get Variable and Set Variable activities can be connected to a Variable. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Variable shared asset:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Variable or Value

 

This box will only be evaluated on reset. See the Overview and Key Concepts section above for more information.

 

### Type

 

Variables can be defined globally or locally. If the Variable is in a general process flow or connected to an object, table cell, global variable or other node, the Variable will be globally available and this property will be unavailable. Otherwise, it can be set to Local so only tokens in the same Process Flow instance will be using the same Variable.

 

### View Variable...

 

Opens a viewer window displaying the current value of the Variable.

 

 

 

 

## Internal Variable Properties

 

The following variables can set to *Internal* which will create a new variable for each instance of the process flow if its type is set to *Local*.

 

### Internal Tracked Variable

 

If the option *Internal Tracked Variable* is selected from the drop down of the **Variable or Value box**, the properties will change to:

 

 

These properties allow you to define what type of tracked variable is used, what its initial value is on reset, and various options for the tracked variable.

 

The **View Variable...** will also display a different viewer:
