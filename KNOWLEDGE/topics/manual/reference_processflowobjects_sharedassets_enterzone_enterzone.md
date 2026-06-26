---
id: reference_processflowobjects_sharedassets_enterzone_enterzone
name: "Enter Zone"
kind: manual
breadcrumb: ["Reference","ProcessFlowObjects","SharedAssets","EnterZone","EnterZone"]
tags: ["manual","reference","processflowobjects","sharedassets"]
source: "manual/Reference/ProcessFlowObjects/SharedAssets/EnterZone/EnterZone.html"
---

# Enter Zone

## Overview and Key Concepts

 

This activity allows tokens to enter a Zone shared asset.

 

 

When this activity is first created, a red exclamation mark shows up to the right of the activity notifying you that a link to a Zone is required for this activity to function. This link may be a direct pointer which can be created by clicking on the exclamation mark and then clicking on a Zone object, or the reference may be dynamic by setting the value for the **Zone Reference** field.

 

 

 

## Connectors

 

The Enter Zone activity allows multiple connector out. However, tokens automatically released from this activity will be released through the first connector. Only manually released tokens have the opportunity to exit out a different connector. See Adding and Connecting Activities for more information.

 

 

 

## Properties

 

The following image shows properties for the Enter Zone activity:

 

 

Each of these properties will be explained in the following sections.

 

### Name

 

Used to change the name of the activity. See Name for more information about this property.

 

### Font

 

The **Font** button opens a window to edit the activity's background color and font properties. See Font for more information about this property.

 

### Statistics

 

The **Statistics** button opens the activity's statistics window. See Statistics for more information about this property.

 

### Zone Reference

 

The reference to the Zone that the token is trying to enter.

 

### Enter For

 

You can control which tokens will enter the Zone when the entering token arrives at this activity using the **Enter For** box.

 

### Use Max Wait Timer

 

The max wait timer will be evaluated if the token has not entered the zone after the specified time. See Max Wait Timer for more information.
