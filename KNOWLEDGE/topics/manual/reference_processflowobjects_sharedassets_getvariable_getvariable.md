---
id: reference_processflowobjects_sharedassets_getvariable_getvariable
name: "Get Variable"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","GetVariable","GetVariable"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/GetVariable/GetVariable.html"
---

# Get Variable

## Overview and Key Concepts

 

This activity gets the value of a Variable shared asset.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a Variable is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Variable object, or the reference may be dynamic by setting the value for the *Variable Reference* property.

 

When getting the value of a Variable, the value (number, string, node array, etc.) will be assigned to a label or node as defined by the *Assign to* property. This value can be used in other activities to affect the logic of the process flow.

 

 

 

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

 

### Assign To

 

Assigns the read value to the specified label/node. See the Assign To section of Common Properties for more information.
