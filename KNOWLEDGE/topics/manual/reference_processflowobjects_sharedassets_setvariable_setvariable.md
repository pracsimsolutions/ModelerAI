---
id: reference_processflowobjects_sharedassets_setvariable_setvariable
name: "Set Variable"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","SetVariable","SetVariable"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/SetVariable/SetVariable.html"
---

# Set Variable

## Overview and Key Concepts

 

This activity sets the value of a Variable shared asset.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a Variable is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Variable object, or the reference may be dynamic by setting the value for the *Variable Reference* property. Variables can be set to many different types of data using the *Value* property. This includes numbers, strings, node references, arrays, etc. The *Rate* property is specific to kinetic tracked variables and is ignored for all other variable types. If you want to set the rate of a kinetic tracked variable but not change its current value, you can remove the value by clicking the Delete button to the right of the *Value* property.

 

 

 

## Connectors

 

The Get Variable activity only allows one connector out. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Get Variable activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Variable Reference

 

The Variable shared asset this activity is linked to.

 

### Value

 

The value to set the Variable shared asset to.

 

### Rate

 

The rate to set the Variable shared asset's kinetic tracked variable to.
